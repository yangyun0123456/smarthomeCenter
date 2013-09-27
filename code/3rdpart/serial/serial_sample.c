#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = { 115200, 57600,  38400,  19200,  9600,  4800, 2400, 1200, 300, };  

void set_speed(int fd, int speed)
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
int set_Parity(int fd, int databits, int stopbits, int parity)
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
int OpenDev(char *Dev)
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

static char fromCommand[COMMANDLEN];
static int haveFromCommand = 0;

int handle_command(char* cmd, int len)
{
        
}

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


passwordL = passwordL*10 + code;
//passwordH save password hight 9 num.
if(passwordL>999999999)
{
    passwordH = passwordL;
    passwordL = 1;
}

int send_command(int comId, void* param0, void* param1, void* param2, void* param3)
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
        case COMMANDREADITEMBYINDEX;
        {
            toCommand[0] = COMMANDREADITEMBYINDEX;
            toCommand[1] = (unsigned char)(param0&0xff);
            haveToCommand = 1;
            break;
        }
        case COMMANDDELETEALLITEM;
        {
            toCommand[0] = COMMANDDELETEALLITEM;
            toCommand[1] = (unsigned char)(param0&0xff);
            haveToCommand = 1;
            break;
        }
        case COMMANDINSERTITEM;
        {
            toCommand[0] = COMMANDINSERTITEM;
            toCommand[1] = (unsigned char)(param0&0xff);
            //id card
            toCommand[2] = (unsigned int)(param1)
            xxxxxx
                xxxxxx
                xxxxx
            haveToCommand = 1;
            break;
        }
        case COMMANDUNLOCK;
        {
            toCommand[0] = COMMANDUNLOCK;
            toCommand[1] = (unsigned char)(param0&0xff);
            xxxxxx
                xxxxxx
                xxxxx
            haveToCommand = 1;
            break;
        }
        default:
            break;
    }
}

//commands list end.


int main(int argc, char **argv)
{
	int fd;
	int nread;
	char buff[512];
	char *dev  = "/dev/ttySAC1"; //串口二
	fd = OpenDev(dev);
	if(fd==-1)
	{
		printf("open device error!\n");
		exit(0);
	}
	set_speed(fd, 9600);
	if (set_Parity(fd,8,1,'N') == -1)
	{
		printf("Set Parity Error\n");
		exit (0);
	}
	printf("Please post card...\n");
	while (1) //循环读取数据
	{
	    
		while((nread = read(fd, buff, 14))>0)
		{ 
		}
	}
	close(fd);  
	exit (0);
}


