#include<iom16v.h>	
#define uchar unsigned char
#define uint unsigned int
#define BZ_0  (PORTD=PORTD&0xdf)
#define BZ_1  (PORTD=PORTD|0x20)
uint cnt;
//=============================
void delay_ms(uint k)
{
    uint i,j;			
    for(i=0;i<k;i++)
    {
       for(j=0;j<1140;j++)
       ;
    }
}
//=============================
void main(void)
{		
 DDRB=0xff;	
PORTB=0xff;	
DDRD=0xdf;	
PORTD=0xff;	
MCUCR = 0x08;     //INT1�½��ش���
 GICR  = 0x80;    //ʹ���ж�
 SREG=0x80;	  //ʹ�����ж�
   while(1)	
   {		
   PORTB=0x00;
   delay_ms(500);
   PORTB=0xff; 
   delay_ms(500); 
   }			
}				
//***************************************************
#pragma interrupt_handler int1_isr:3  //�ж��Ӻ���
void int1_isr(void)
{
 for(cnt=0;cnt<5000;cnt++)
 {BZ_1;delay_ms(2); BZ_0;}
}
