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
			printf("\nLen %d\n",nread); 
			buff[nread+1] = '\0';   
			printf( "\n%s", buff);   
		}
	}
	close(fd);  
	exit (0);
}


