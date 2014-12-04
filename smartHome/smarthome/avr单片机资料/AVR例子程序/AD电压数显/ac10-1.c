/*�õ�λ����ģ��������AD7����λ����ܵ�ѹ������ʾ*/

#include <iom16v.h>			
#define uchar unsigned char	
#define uint  unsigned int	
uchar const SEG7[10]={0x3f,0x06,0x5b, 
0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar const ACT[4]={0xfe,0xfd,0xfb,0xf7};
uint adc_val,dis_val;	
uchar i,cnt;				
/************************************************/
void port_init(void)	
{						
 PORTA = 0x7F;			
 DDRA  = 0x7F;			
 PORTB = 0xFF;
 DDRB  = 0xFF;		
 PORTC = 0xFF; 
 DDRC  = 0xFF;		
 PORTD = 0xFF;		
 DDRD  = 0xFF;		
}				
/************************************************/
void adc_init(void)	
{					
ADCSRA = 0xE3; 	//AD����ת��ģʽ��8  ��Ƶ	
ADMUX = 0xC7;	//ͨ��7���Ҷ��룬�ڲ�2.56V�ο�	
}			
//***************************
void timer0_init(void)	
{					
TCNT0 = 0x83; 		//1MS 
TCCR0 = 0x03; 		//Ԥ��Ƶ64
TIMSK = 0x01; 		//ʹ��T0�ж�
}				
/*********************************************/
void init_devices(void)	
{							
 port_init();	
 timer0_init();			
 adc_init();			
 SREG=0x80;	       //��ʼ����ʹ�����ж�
}							
//***************************
#pragma interrupt_handler timer0_ovf_isr:10
void timer0_ovf_isr(void)	
{							
 TCNT0 = 0x83; 		//��ֵ1MS		
 cnt++;				
 if(++i>3)i=0;			
 switch(i)	
 {
 case 0:PORTA=SEG7[dis_val%10];PORTC=ACT[0];break;
 case 1:PORTA=SEG7[(dis_val/10)%10];PORTC=ACT[1];break;
 case 2:PORTA=SEG7[(dis_val/100)%10];PORTC=ACT[2];break;
 case 3:PORTA=SEG7[dis_val/1000];PORTC=ACT[3];break;
 default:break;
 }
}
//=========================
uint ADC_Convert(void)		
{uint temp1,temp2;		
 temp1=(uint)ADCL;			
 temp2=(uint)ADCH;
 temp2=(temp2<<8)+temp1;	
 return(temp2);			
}
/**************************/
uint conv(uint i)		
{
long x;				
uint y;				
x=(5000*(long)i)/1023;	
y=(uint)x;	
return y;	
} 
/***********************/
void delay(uint k)	
{
uint i,j;
	 for(i=0;i<k;i++)
	 {	 
	 for(j=0;j<140;j++); 
	 }
}
/***********************/
void main(void)				
{	 						
init_devices();			
  while(1)					
  {							
   		if(cnt>100)		
		{
		adc_val=ADC_Convert();
		dis_val=conv(adc_val);
		cnt=0;				
		}
		delay(10);		
  }
}
