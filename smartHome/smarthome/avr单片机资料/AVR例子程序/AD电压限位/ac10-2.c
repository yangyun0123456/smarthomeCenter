/*���ڵ�ѹ���������ѹ����3Vʱ��D2���������ơ�2V~3V֮�䲻��*/
#include <iom16v.h>			
#define uchar unsigned char	
#define uint  unsigned int
#define OUT1_0  (PORTB=PORTB&0xfe) //����D1�͵�ƽ
#define OUT1_1  (PORTB=PORTB|0x01) //����D1�ߵ�ƽ
#define OUT2_0  (PORTB=PORTB&0xfd) //����D2�͵�ƽ
#define OUT2_1  (PORTB=PORTB|0x02) //����D2�ߵ�ƽ
uchar const SEG7[10]={0x3f,0x06,0x5b,
0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar const ACT[4]={0xfe,0xfd,0xfb,0xf7};
uint value,dis_val; 
uchar i,flag; 
/****************************************/
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
/************************************/
void timer0_init(void) 
{
 TCNT0 = 0x83; 	  //1MS
 OCR0  = 0x7D;
 TCCR0 = 0x03; 	  //Ԥ��Ƶ
}
/*************************************/
#pragma interrupt_handler timer0_ovf_isr:10 
void timer0_ovf_isr(void)
{
 TCNT0 = 0x83; 	
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
/************************************/
void timer1_init(void) 	
{
 TCNT1H = 0xE7; 	//50MS
 TCNT1L = 0x96;
 TCCR1B = 0x03; 	
}
/***************************************************/
#pragma interrupt_handler timer1_ovf_isr:9 
void timer1_ovf_isr(void)
{
 TCNT1H = 0xE7; 		
 TCNT1L = 0x96; //��װ��ֵ50MS
 }
/****************************************************/
void adc_init(void) 
{
 ADMUX = 0x07;        //ͨ��7
 ACSR  = 0x80;        //��ADC  
 ADCSR = 0xE9;        //ADC�ж�ʹ�ܣ�ADCת��ʹ�ܣ��Զ�����ʹ��
}
/****************************************************/
#pragma interrupt_handler adc_isr:15 
void adc_isr(void)
{
 //conversion complete, read value (int) using...
  value=ADCL;            //ȡģ��ֵ
  value|=(int)ADCH << 8; 
  flag=1;	         //��־λ��1
}
/***************************************/
void init_devices(void) 
{
 port_init();	
 timer0_init();	
 timer1_init();	
 adc_init();	
 TIMSK = 0x05;  //T0/T1����ж�ʹ��
 SREG=0x80;     //���ж�ʹ��
}
/***************************************/
void delay(uint k) 
{
uint i,j;
	 for(i=0;i<k;i++)
	 {	 
	 for(j=0;j<140;j++); 
	 }
}
/******************************************/
uint conv(uint i) 
{
long x; 	
uint y; 
x=(5000*(long)i)/1023; 
y=(uint)x; 	
return y; 	    //����ת��
} 
/******************************************/
void main(void) 	
{	 				
init_devices();		
  while(1) 	
  {
   		if(flag==1)          //�б�־λ
		{
		dis_val=conv(value); 
		  if(dis_val<2000){OUT2_1;OUT1_0;} 
		  else if(dis_val<3000){OUT1_1;OUT2_1;} 
		  else {OUT2_0;OUT1_1;} 
		flag=0;			
		}
		delay(10);		
  }
} 
