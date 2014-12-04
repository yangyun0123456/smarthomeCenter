#include <iom16v.h> 
#define uchar unsigned char	
#define uint unsigned int
uchar const SEG7[10]={0x3f,0x06,0x5b, 
0x4f,0x66,	0x6d,0x7d,0x07,0x7f,0x6f};//������0~9ֵ
uchar const ACT[8]={0xfe,0xfd,0xfb,0xf7, 
0xef,0xdf,0xbf,0x7f};       //λ��
uchar i;			
void main(void) 
 {			
 PORTA = 0x00; 
 DDRA  = 0xFF; 
 PORTC = 0xFF; 
 DDRC  = 0xFF; 
 TCNT0 = 0x83;//1MS��ʱ
 TCCR0 = 0x03;//Ԥ��Ƶ64
 TIMSK = 0x01;//ʹ��T0�ж�
 SREG=0x80; //ʹ�����ж�
while(1);
 }

#pragma interrupt_handler timer0_ovf_isr:10
void timer0_ovf_isr(void)
{
 TCNT0 = 0x83;  //��װ��ֵ
 if(++i>7)i=0;
 PORTA=SEG7[i];
 PORTC=ACT[i];
}
