#include<iom16v.h>	
#define uchar unsigned char	
/************************************/
#define FLASH_0  (PORTB=PORTB&0xfe) //����˿�Ϊ�͵�ƽ
#define FLASH_1  (PORTB=PORTB|0x01) //����˿�Ϊ�ߵ�ƽ
/************************************/
void main(void) 
{			
uchar cnt,status; 
 PORTB = 0x01; 
 DDRB  = 0x01; 
 TCNT1H = 0xCF;
 TCNT1L = 0x2C;              //100ms��ʱ
 TCCR1B = 0x03;              //����Ԥ��Ƶ64
   	for(;;)	
 	{	
		do {status=TIFR&0x04;}while(status!=0x04); //��TOV1�Ƿ����
		TIFR=0x04;              //��ѭ��ֵΪ��ʱ��ֹѭ����˳��ִ��
		TCNT1H = 0xCF; 
		TCNT1L = 0x2C; 
		cnt++;		
		if(cnt==9)FLASH_0;
		if(cnt>=10){cnt=0;FLASH_1;}
		}		
}	
