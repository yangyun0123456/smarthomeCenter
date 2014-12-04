#include <iom16v.h>			
#include <macros.h>
//-----------------------------------------------
#define uchar unsigned char	
#define uint unsigned int
//-------------------���ŵ�ƽ�ĺ궨��---------
#define LCM_RS_1 PORTB|=BIT(PB0)	
#define LCM_RS_0 PORTB&=~BIT(PB0) 
#define LCM_RW_1 PORTB|=BIT(PB1)	
#define LCM_RW_0 PORTB&=~BIT(PB1) 
#define LCM_EN_1 PORTB|=BIT(PB2)	
#define LCM_EN_0 PORTB&=~BIT(PB2)  
#define SCL_1 PORTC|=BIT(PC0)
#define SCL_0 PORTC&=~BIT(PC0)
#define SDA_1 PORTC|=BIT(PC1)
#define SDA_0 PORTC&=~BIT(PC1)
//======================================
#define PIN_SDA (PINC&0x02)
#define DataPort PORTA		
#define Busy 0x80			
#define xtal 8   			
#define Some_NOP();  _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
//=====================================
const uchar str0[]={"Write : "};//�����ַ���
const uchar str1[]={"Read : "};//�����ַ���
//========��������=========
void Delay_1ms(void);
void Delay_nms(uint n);
void WaitForEnable(void);
void LcdWriteData(uchar W);
void LcdWriteCommand(uchar CMD,uchar Attribc);
void InitLcd(void);
void Display(uchar dd);
void DisplayOneChar(uchar x,uchar y,uchar Wdata);
void ePutstr(uchar x,uchar y,uchar const *ptr);
void port_init(void);
//**********************��ʾָ�������һ���ַ��Ӻ���**************
void ePutstr(uchar x,uchar y,uchar const *ptr)
{
uchar i,l=0;
	while(ptr[l]>31){l++;}
	for(i=0;i<l;i++){
	DisplayOneChar(x++,y,ptr[i]);
	if(x==16){
		x=0;y^=1;
	}
  }
}
//*******************��ʾ��궨λ�Ӻ���******************
void LocateXY(char posx,char posy)
{
uchar temp;
	temp&=0x7f;
	temp=posx&0x0f;
	posy&=0x01;
	if(posy)temp|=0x40;
	temp|=0x80;
	LcdWriteCommand(temp,0);
}
//*******************��ʾָ�������һ���ַ��Ӻ���*******************
void DisplayOneChar(uchar x,uchar y,uchar Wdata)
{
LocateXY(x,y);
LcdWriteData(Wdata);
}
//*******************LCD��ʼ���Ӻ���*********************
void InitLcd(void) 
{
LcdWriteCommand(0x38,0); 
Delay_nms(5);
LcdWriteCommand(0x38,0); 
Delay_nms(5);
LcdWriteCommand(0x38,0); 
Delay_nms(5);
LcdWriteCommand(0x38,1); 
LcdWriteCommand(0x08,1); 
LcdWriteCommand(0x01,1); 
LcdWriteCommand(0x06,1); 
LcdWriteCommand(0x0c,1); 
}
//********************д���LCM�Ӻ���********************
void LcdWriteCommand(uchar CMD,uchar Attribc)
{
if(Attribc)WaitForEnable();
LCM_RS_0;LCM_RW_0;_NOP();
DataPort=CMD;_NOP();
LCM_EN_1;_NOP();_NOP();LCM_EN_0;
}
//*******************д���ݵ�LCM�Ӻ���********************
void LcdWriteData(uchar dataW)
{
WaitForEnable();
LCM_RS_1;LCM_RW_0;_NOP();
DataPort=dataW;_NOP();
LCM_EN_1;_NOP();_NOP();LCM_EN_0;
}
//*******************���LCDæ�ź��Ӻ���*********************
void WaitForEnable(void)
{
uchar val;
DataPort=0xff;
LCM_RS_0;LCM_RW_1;_NOP();
LCM_EN_1;_NOP();_NOP();
DDRA=0x00;
val=PINA;
while(val&Busy)val=PINA;
LCM_EN_0;
DDRA=0xff;
}
//****************************************
void Delay_1ms(void)		//1mS��ʱ�Ӻ���
{ uint i;
 for(i=1;i<(uint)(xtal*143-2);i++)
    ;
}
//====================================
void Delay_nms(uint n)		//n*1mS��ʱ�Ӻ���
{
 uint i=0;
   while(i<n)
   {Delay_1ms();
    i++;
   }
}
/*****************�˿ڳ�ʼ��********************/
void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0xFF;
 PORTB = 0x00;
 DDRB  = 0xFF;
 PORTC = 0x00; 
 DDRC  = 0x03;
 PORTD = 0xFF;
 DDRD  = 0x00;
}
/*************************************/
char com_data;	
uchar cnt;		
void delay_iic(int n);	
uchar rd_24c01(char a);	
void wr_24c01(char a,char b);
/******************ɨ�谴��***************/
uchar scan_key(void)		
{					
uchar temp;			
temp=PIND;				
return temp;			
}					
/***************��ʱ�Ӻ���******************/
void delay(uint k)		
{					
uint i,j;			
for(i=0;i<k;i++)			
{for(j=0;j<121;j++)		
{;}}				
}					
/*****************������дʱ���Ӻ���******************/
void start(void)			
{DDRC=0x03;					
SDA_1;Some_NOP();
SCL_1;Some_NOP();
SDA_0;Some_NOP();
SCL_0;Some_NOP();
}					
//********************ֹͣ�����Ӻ���*********************
void stop(void)			
{	DDRC=0x03;			
SDA_0;Some_NOP();
SCL_1;Some_NOP();
SDA_1;Some_NOP();
}				
//************Ӧ���Ӻ���*************
void ack(void)			
{	DDRC=0x03;				
SCL_1;Some_NOP();
SCL_0;Some_NOP();
}					
//*************д��8λ�Ӻ���*************
void shift8(char a)		
{					
uchar i,j;			
DDRC=0x03;
com_data=a;			
for(i=0;i<8;i++)	
{	
j=com_data&0x80;
if(j==0)SDA_0;
else SDA_1;

SCL_1;Some_NOP();
SCL_0;Some_NOP();
com_data=com_data<<1;		
}					
}					
//**************��24C01A��a��ַ��Ԫ������************
uchar rd_24c01(char a)		
{					
uchar i,command;		
DDRC=0x03;
SDA_1;Some_NOP();
SCL_0;Some_NOP();
start();				
command=160;			
shift8(command);		
ack();				
shift8(a);			
ack();				
start();				
command=161;			
shift8(command);		
ack();				

SDA_1;Some_NOP();	
for(i=0;i<8;i++)			
{
DDRC=0x01;				
com_data=com_data<<1;		
SCL_1;Some_NOP();	
if(PIN_SDA==0)com_data&=0xfe;
else com_data|=0x01;
SCL_0;Some_NOP();
}					
stop();				
return(com_data);	
}					
//********��RAM��b��ַ��Ԫ������д��24C01A��a��ַ��Ԫ��***********
void wr_24c01(char a,char b)	
{					
uchar command;		
DDRC=0x03;
SDA_1;Some_NOP();
SCL_0;Some_NOP();
start();				
command=160;			
shift8(command);		
ack();				
shift8(a);				
ack();				
shift8(b);				
ack();				
stop();				
Some_NOP();
}					
//**************��ʱ�Ӻ���***********
void delay_iic(int n)		
{					
int i;				
for(i=1;i<n;i++){;}		
}					
//******************************************
void main(void)				
{
 	 uchar key_val,wr_val=0,rd_val=0;	
	 port_init();
    Delay_nms(400);			
	InitLcd();				
	LcdWriteCommand(0x01,1); 
	LcdWriteCommand(0x0c,1);	
	ePutstr(0,0,str0);  
	Delay_nms(10);
	ePutstr(0,1,str1);   
	Delay_nms(10);
	/********************************************/
		while(1)           
		{
		  DisplayOneChar(9,0,wr_val/100+0x30);
		   Delay_nms(10);
		   DisplayOneChar(10,0,(wr_val/10)%10+0x30); 
		   Delay_nms(10); 
		   DisplayOneChar(11,0,wr_val%10+0x30);	  
		   Delay_nms(10); 
	   
		   DisplayOneChar(8,1,rd_val/100+0x30);	  
		   Delay_nms(10); 
		   DisplayOneChar(9,1,(rd_val/10%10)+0x30);	 
		   Delay_nms(10); 
		   DisplayOneChar(10,1,rd_val%10+0x30);	 
		   Delay_nms(10); 
		   
		   key_val=scan_key();
		   switch(key_val)
	   	   {				
	   	   	case 0xef:if(wr_val<255)wr_val++;break;
	   		case 0xdf:if(wr_val>0)wr_val--;break;
	   		case 0xbf:wr_24c01(10,wr_val);delay_iic(2500);
				 DisplayOneChar(15,0,0xef);break;	
	   		case 0x7f:rd_val=rd_24c01(10);delay_iic(2500);
			     DisplayOneChar(15,1,0xef);break;	
	   		default:break;		
	   		}				
			Delay_nms(200); 
			DisplayOneChar(15,0,0x20);Delay_nms(10); 
			DisplayOneChar(15,1,0x20);Delay_nms(10); 
					
  		 }				
	
}		   
