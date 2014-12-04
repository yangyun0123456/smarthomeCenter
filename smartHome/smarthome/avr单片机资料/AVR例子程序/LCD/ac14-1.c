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
//======================================
#define DataPort PORTA	//�ֽڶ���	
#define Busy 0x80		
#define xtal 8   		
//======================================
const uchar str0[]={"-This is a LCD-!"};
const uchar str1[]={"-Design by ZXH-!"};
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
//******************************************
void main(void)				
{
    Delay_nms(400);			
	DDRA=0xff;PORTA=0x00;	
	DDRB=0xff;PORTB=0x00;
	InitLcd();				
	/********************************************/
		while(1)              
		{
		   LcdWriteCommand(0x01,1); 
		   LcdWriteCommand(0x0c,1);	  
		   DisplayOneChar(0,1,0x41); 
		   ePutstr(0,0,str0); 
		   Delay_nms(2000);	
		   LcdWriteCommand(0x01,1);  
		   LcdWriteCommand(0x0c,1);  
		   DisplayOneChar(8,0,0x42);	
		   ePutstr(0,1,str1);  
		   Delay_nms(2000); 
	   }
}		   
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
