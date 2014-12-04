#include <iom16v.h>
#include <macros.h>
/*************************************************/
void port_init(void) 
{
 PORTA = 0xFF; 
 DDRA  = 0x00; 
 PORTB = 0xFF; 
 DDRB  = 0xFF; 
 PORTC = 0xFF; 
 DDRC  = 0x00; 
 PORTD = 0xFF; 
 DDRD  = 0x02;
}
/**************************************************/
void uart0_init(void) 
{
 UCSRB = 0x00; 
 UCSRA = 0x02;
 UCSRC = 0x06;
 UBRRL = 0x67; 
 UBRRH = 0x00; 
 UCSRB = 0x18;
}
/********************************************************/
void init_devices(void) 
{
 port_init();
 uart0_init();
 }
//**********************************************
void uart0_send(unsigned char i)
{
while(!(UCSRA&(1<<UDRE)));
UDR=i;
}
/*******************************************/
void str_send(char *s) //����һ���ַ��ĺ���
{
  while(*s)             //�ȴ����͵��ַ�Ϊ��
 {
 uart0_send(*s);        //����1���ַ�
 s++;                   //ָ������һ���ַ�
 }
}
/*********************************************/
unsigned char uart0_receive(void) 
{
while(!(UCSRA&(1<<RXC)));
return UDR;
}
/********************************************/
void main(void)
{
unsigned char temp; 
init_devices();
str_send("��˾���Գ���  ");
str_send("AVR��Ƭ��RS232ͨ�Ų��Գ���  ");
str_send("http://www.hlelectron.com  ");
 while(1) 
 {
 temp=uart0_receive();
 str_send("��ǰ�����ǣ�");
 uart0_send(temp); 
 str_send("  ");
 }	
}
