#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>

/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
static int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
static int name_arr[] = { 115200, 57600,  38400,  19200,  9600,  4800, 2400, 1200, 300, };  

static void set_speed(int fd, int speed)
{
  int   i; 
  int   status; 
  struct termios   Opt;
  tcgetattr(fd, &Opt); 
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
  { 
    if  (speed == name_arr[i])
	{     
      tcflush(fd, TCIOFLUSH);     
      cfsetispeed(&Opt, speed_arr[i]);  
      cfsetospeed(&Opt, speed_arr[i]);
	  Opt.c_cflag |= (CLOCAL | CREAD);	  
      status = tcsetattr(fd, TCSANOW, &Opt);  
      if  (status != 0)
	  {        
        perror("tcsetattr fd");  
        return;     
      }    
      tcflush(fd,TCIOFLUSH);   
    }  
  }
}

/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
static int set_Parity(int fd, int databits, int stopbits, int parity)
{ 
	struct termios options; 
	if  ( tcgetattr( fd,&options)  !=  0)
	{ 
		perror("SetupSerial 1");     
		return -1;  
	}
	options.c_cflag &= ~CSIZE; 
	switch (databits) /*设置数据位数*/
	{   
	case 7:		
		options.c_cflag |= CS7; 
		break;
	case 8:     
		options.c_cflag |= CS8;
		break;   
	default:    
		fprintf(stderr,"Unsupported data size\n"); return -1;  
	}
	switch (parity) 
	{   
		case 'n':
		case 'N':    
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
			break;  
		case 'o':   
		case 'O':     
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
			options.c_iflag |= INPCK;             /* Disnable parity checking */ 
			break;  
		case 'e':  
		case 'E':   
			options.c_cflag |= PARENB;     /* Enable parity */    
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S': 
		case 's':  /*as no parity*/   
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;break;  
		default:   
			fprintf(stderr,"Unsupported parity\n");    
			return -1;  
	}  
	/* 设置停止位*/  
	switch (stopbits)
	{   
		case 1:    
			options.c_cflag &= ~CSTOPB;  
			break;  
		case 2:    
			options.c_cflag |= CSTOPB;  
		   break;
		default:    
			 fprintf(stderr,"Unsupported stop bits\n");  
			 return -1; 
	} 
	/* Set input parity option */ 
	if (parity != 'n')   
		options.c_iflag |= INPCK; 
	/*设置硬件流控制无效*/
	//options.c_cflag &= ~CNEW_RTSCTS;
	/*设置原始输入模式*/
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	
	tcflush(fd,TCIFLUSH);
	options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/   
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)   
	{ 
		perror("SetupSerial 3");   
		return -1;  
	} 
	return 0;  
}

/*********************************************************************/
static int OpenDev(char *Dev)
{
	int	fd = open( Dev, O_RDWR | O_NOCTTY | O_NDELAY );
	if (-1 == fd)	
	{ 			
		perror("Can't Open Serial Port");
		return -1;		
	}	
	else	
		return fd;
}

//commands list

#define COMMANDLEN   14
static char toCommand[COMMANDLEN];
static int haveToCommand = 0;

#define COMMANDIDLOGOK			0
#define COMMANDLOGERR			1
#define COMMANDREADITEMNUM		2
#define COMMANDRETURNITEMNUM	3
#define COMMANDREADITEMBYINDEX	4
#define COMMANDRETURNITEM		5
#define COMMANDDELETEALLITEM	6
#define COMMANDINSERTITEM		7
#define COMMANDUNLOCK			8
#define COMMANDRETURNOK			9

#define PASSWORDFLAGS_UNKOWN    0x00
#define PASSWORDFLAGS_ID                0x01
#define PASSWORDFLAGS_PASSWORD  0x02

#define u32 unsigned int
#define u8 unsigned char
#define Read32(h2, h1, l2, l1) ((u32)(((((u32)h2)&0xff)<<24)|((((u32)h1)&0xff)<<16)|((((u32)l2)&0xff)<<8)|(l1)))

static int handle_command(char* cmd, int len)
{
    unsigned char comId = cmd[0];
    unsigned char type = cmd[1];
    unsigned int id = Read32(cmd[2], cmd[3], cmd[4], cmd[5]);
    unsigned int passwordH = Read32(cmd[6], cmd[7], cmd[8], cmd[9]);
    unsigned int passwordL = Read32(cmd[10], cmd[11], cmd[12], cmd[13]);
    
    passwordH %= 10;
    passwordL %= 10;

    switch(comId)
    {
        case COMMANDIDLOGOK:
        {
            if(type==PASSWORDFLAGS_ID)
            {
                printf("Use ID Card: %d\n", id);                
            }
            else if(type==PASSWORDFLAGS_PASSWORD)
            {
                printf("Use password: ");
                if(passwordH) printf("%d", passwordH);
                if(passwordL) printf("%d", passwordL);
                printf("\n");
            }
            else if(type==(PASSWORDFLAGS_ID|PASSWORDFLAGS_PASSWORD))
            {
                printf("Use ID Card: %d\t", id);                
                printf("And use password: ");
                if(passwordH) printf("%d", passwordH);
                if(passwordL) printf("%d", passwordL);
                printf("\n");
            }
            break;
        }
        case COMMANDLOGERR:
        {
            printf("Error input:\n\tID Card: %d\n", id);
            printf("\tPassword: ");
            if(passwordH) printf("%d", passwordH);
            if(passwordL) printf("%d", passwordL);
            printf("\n");
            break;
        }
        case COMMANDRETURNITEMNUM:
        {
            printf("Return Item: %d\n", type);
            if(type==PASSWORDFLAGS_ID)
            {
                printf("Return ID Card: %d\n", id);                
            }
            else if(type==PASSWORDFLAGS_PASSWORD)
            {
                printf("Return password: ");
                if(passwordH) printf("%d", passwordH);
                if(passwordL) printf("%d", passwordL);
                printf("\n");
            }
            else if(type==(PASSWORDFLAGS_ID|PASSWORDFLAGS_PASSWORD))
            {
                printf("Return ID Card: %d\t", id);                
                printf("And Return password: ");
                if(passwordH) printf("%d", passwordH);
                if(passwordL) printf("%d", passwordL);
                printf("\n");
            }
            break;
        }
        case COMMANDRETURNITEM:
        {
            printf("Return Item Number: %d\n", type);
            break;
        }
        case COMMANDRETURNOK:
        {
            printf("Return command OK!\n");
            break;
        }
        default:
            break;
    }
    return 0;    
}

static int send_command(int comId, unsigned char type, unsigned int id, unsigned int passwordH, unsigned int passwordL)
{
    memset(&toCommand, 0, COMMANDLEN);
    haveToCommand = 0;
    
    switch(comId)
    {
        case COMMANDREADITEMNUM:
        {
            toCommand[0] = COMMANDREADITEMNUM;
            haveToCommand = 1;
            break;
        }
        case COMMANDREADITEMBYINDEX:
        {
            toCommand[0] = COMMANDREADITEMBYINDEX;
            toCommand[1] = type;
            haveToCommand = 1;
            break;
        }
        case COMMANDDELETEALLITEM:
        {
            toCommand[0] = COMMANDDELETEALLITEM;
            toCommand[1] = type;
            haveToCommand = 1;
            break;
        }
        case COMMANDINSERTITEM:
        {
            toCommand[0] = COMMANDINSERTITEM;
            toCommand[1] = type;
            //id card
            toCommand[2] = (unsigned char)((id>>24)&0xff);
            toCommand[3] = (unsigned char)((id>>16)&0xff);
            toCommand[4] = (unsigned char)((id>>8)&0xff);
            toCommand[5] = (unsigned char)(id&0xff);
            //password
            toCommand[6] = (unsigned char)((passwordH>>24)&0xff);
            toCommand[7] = (unsigned char)((passwordH>>16)&0xff);
            toCommand[8] = (unsigned char)((passwordH>>8)&0xff);
            toCommand[9] = (unsigned char)(passwordH&0xff);

            toCommand[10] = (unsigned char)((passwordL>>24)&0xff);
            toCommand[11] = (unsigned char)((passwordL>>16)&0xff);
            toCommand[12] = (unsigned char)((passwordL>>8)&0xff);
            toCommand[13] = (unsigned char)(passwordL&0xff);

            haveToCommand = 1;
            break;
        }
        case COMMANDUNLOCK:
        {
            toCommand[0] = COMMANDUNLOCK;
            toCommand[1] = type;
            //id card
            toCommand[2] = (unsigned char)((id>>24)&0xff);
            toCommand[3] = (unsigned char)((id>>16)&0xff);
            toCommand[4] = (unsigned char)((id>>8)&0xff);
            toCommand[5] = (unsigned char)(id&0xff);
            //password
            toCommand[6] = (unsigned char)((passwordH>>24)&0xff);
            toCommand[7] = (unsigned char)((passwordH>>16)&0xff);
            toCommand[8] = (unsigned char)((passwordH>>8)&0xff);
            toCommand[9] = (unsigned char)(passwordH&0xff);

            toCommand[10] = (unsigned char)((passwordL>>24)&0xff);
            toCommand[11] = (unsigned char)((passwordL>>16)&0xff);
            toCommand[12] = (unsigned char)((passwordL>>8)&0xff);
            toCommand[13] = (unsigned char)(passwordL&0xff);

            haveToCommand = 1;
            break;
        }
        default:
            break;
    }
}

//commands list end.
static int sPortFd = 0;
static pthread_t readWriteThreadId = 0;

static pthread_t ttyInputThreadId = 0;

static int threadRun = 0;
static int threadRunning = 0;

static void readWriteThread(void* param)
{
	int nread = 0;
	char buff[512];

    threadRun = threadRunning = 1;
    while(threadRun)
    {
       //read command.
       nread += read(sPortFd, buff+nread, (COMMANDLEN-nread));
       if(nread == COMMANDLEN)
       {
            handle_command(buff, COMMANDLEN);
            nread = 0;
       }
       //write command.
       if(haveToCommand)
       {
            write(sPortFd, toCommand, COMMANDLEN);
            haveToCommand = 0;
       }
       usleep(100*1000);
    }
    threadRun = threadRunning = 0;
}

static void closeThread(void)
{
    threadRun = 0;
    while(threadRunning)
    {
        printf("Waiting for thread over...\n");
        usleep(400*1000);
    }
    threadRun = threadRunning = 0;
}

static void handle_input(char* buf, int bufLen)
{
    char tmp[512];
    char* s = NULL;
    char* e = NULL;

    memset(tmp, 0, 512);
    s = strchr(buf, ' ');
    if(s)
    {
        memcpy(tmp, buf, s-buf);

        if(!strcmp(tmp, "number"))
        {
            send_command(COMMANDREADITEMNUM, 0, 0, 0, 0);
        }
        else if(!strcmp(tmp, "item"))
        {
            e = strchr(s, ' ');
            if(e)
            {
                int index = 0;

                memset(tmp, 0, 512);
                memcpy(tmp, s, e-s);
                index = atoi(tmp);
                if(index<100)
                {
                    send_command(COMMANDREADITEMBYINDEX, index, 0, 0, 0);
                }
            }
        }
        else if(!strcmp(tmp, "deleteall"))
        {
            e = strchr(s, ' ');
            if(e)
            {
                int num = 0;

                memset(tmp, 0, 512);
                memcpy(tmp, s, e-s);
                num = atoi(tmp);
                if(num<100)
                {
                    send_command(COMMANDDELETEALLITEM, num, 0, 0, 0);
                }
            }
        }
        else if(!strcmp(tmp, "insert"))
        {

        }
        else if(!strcmp(tmp, "unlock"))
        {

        }
        memset(tmp, 0, 512);
    }
     
}

static int ttyInputRun = 0;
static int ttyInputRunning = 0;

static void ttyInput(void* param)
{
    int tty = open("/dev/tty", O_RDONLY);
    char buf[512];
    int bufConut = 0;

    struct termios newt, oldt;
    tcgetattr(tty, &oldt);
    newt.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(tty, TCSANOW, &newt);
    
    fcntl(tty, F_SETFL, FNDELAY);

    ttyInputRun = ttyInputRunning = 1;
    
	printf("Shell>:\n");

    while(ttyInputRun)
    {
        char ch = 0;
        
        read(tty, &ch, 1);
        if(ch)
        {
            if(ch==0x0a)
            {
                printf("You input: %s\n", buf);

                if(!strcmp(buf, "quit"))
                {
                    printf("You input quit, to quit!\n");
                }
                else handle_input(buf, bufConut);
                
                memset(buf, 0, 512);
                bufConut = 0;

                printf("Shell>:\n");
            }
            else if(bufConut==512)
            {
                memset(buf, 0, 512);
                bufConut = 0;
                printf("Shell>:\n");
            }
            else buf[bufConut++] = ch;

            ch = 0;
        }
        usleep(100*1000);
    }
    
    ttyInputRun = ttyInputRunning = 0;
    tcsetattr(tty, TCSANOW, &oldt);
    return;
}

static int mainThreadRun = 0;

int main(int argc, char **argv)
{
	char *dev  = "/dev/ttySAC1"; //串口二
	sPortFd = OpenDev(dev);
	if(sPortFd==-1)
	{
		printf("open device error!\n");
		exit (0);
	}
	set_speed(sPortFd, 9600);
	if (set_Parity(sPortFd,8,1,'N') == -1)
	{
		printf("Set Parity Error\n");
		exit (0);
	}
    //read not wait...
    fcntl(sPortFd, F_SETFL, FNDELAY);
    
    pthread_create(&readWriteThreadId, NULL, readWriteThread, NULL);
    pthread_create(&ttyInputThreadId, NULL, ttyInput, NULL);

	mainThreadRun = 1;

	while(mainThreadRun)
	{
		usleep(500*1000);
	}
    closeThread();
	close(sPortFd);  
	exit (0);
}


