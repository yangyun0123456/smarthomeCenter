#include <iom16v.h>	
#define uchar unsigned char
#define uint unsigned int
uchar const SEG7[10]={0x3f,0x06,0x5b, 
	  0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar const ACT[4]={0xfe,0xfd,0xfb,0xf7}; 
uint cnt; 
uchar start_flag; 
uchar i; 
#define S1 (PIND&0x10)	

void port_init(void)
{
 PORTA = 0x00; 
 DDRA  = 0xFF; 
 PORTC = 0xFF; 
 DDRC  = 0xFF; 
 PORTD = 0xFF; 
 DDRD  = 0x00; 
}

void timer0_init(void)
{
 TCNT0 = 0x83; 
 TCCR0 = 0x03;         //T0��ʼ��Ϊ��ʱ1MS
}

#pragma interrupt_handler timer0_ovf_isr:10
void timer0_ovf_isr(void)
{
 SREG=0x80;               //���¿������жϣ�ȷ��׼ȷ��ʱ
 TCNT0 = 0x83;           //��װ��ֵ
 if(++i>3)i=0;  
 switch(i)
 {
 case 0: PORTA=SEG7[cnt%10]; PORTC=ACT[i];break;
 case 1: PORTA=SEG7[(cnt/10)%10]; PORTC=ACT[i];break;
 case 2: PORTA=SEG7[(cnt/100)%10]|0x80; PORTC=ACT[i];break;
 case 3: PORTA=SEG7[cnt/1000]; PORTC=ACT[i];break;
 default:break;
 }
}

void timer1_init(void) 
{
 TCNT1H = 0xD8; 
 TCNT1L = 0xF0;           //10MS��ʱ��ֵ
}

#pragma interrupt_handler timer1_ovf_isr:9
void timer1_ovf_isr(void)
{
 TCNT1H = 0xD8; 
 TCNT1L = 0xF0;          //��װ//���ȷ��Ϊ���뼶
 if(++cnt>9999)cnt=0;
}

#pragma interrupt_handler int0_isr:2
void int0_isr(void)
{
if(cnt<10)start_flag=0xff;//��ʱδ��ʼ��������־�ø�
else start_flag=0x00;     //��ʱ��ʼ��������־�õ�
}

void init_devices(void)
{
 port_init();
 timer0_init();
 timer1_init();
 MCUCR = 0x02; //INT0�½��ش���
 GICR  = 0x40;  //ʹ��INTO�ж�
 TIMSK = 0x05; //ʹ��T1/T0�ж�
 SREG=0x80;    //ʹ�����ж�
}

void scan_s1(void)
{
if(S1==0)cnt=0;      //S1���£���0
}

void main(void) 
{
init_devices();         //оƬ��ʼ��
  while(1) 
  {
  if(start_flag==0xff)TCCR1B = 0x02;          //���־��δ��ʱ��ʼ��ʱ
if(start_flag==0x00){TCCR1B = 0x00;scan_s1();}//����ֹͣ��ʱ
							
  }						
}					