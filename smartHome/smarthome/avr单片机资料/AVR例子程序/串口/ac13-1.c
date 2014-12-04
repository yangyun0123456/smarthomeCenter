#include <iom16v.h>
#include <macros.h>
//==================================
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
//**********************************************
void uart0_init(void)
{
 UCSRB = 0x00;                //��ֹUART���ͺͽ���
 UCSRA = 0x02;                //����
 UCSRC = 0x06;                //8λ����
 UBRRL = 0x67;                //9600B/S
 UBRRH = 0x00; 
 UCSRB = 0x18;                //����UART���ͺͽ���
}
//**********************************************
void init_devices(void) 
{
 port_init();
 uart0_init();
 }
//**********************************************
void uart0_send(unsigned char i)
{
while(!(UCSRA&(1<<UDRE)));  //�ȴ����ͻ�����Ϊ�ա�
                            //UDREΪ1˵��������Ϊ�գ���׼���ý������ݽ���
UDR=i;                      //����1���ַ�//UDRΪUSART I/O ���ݼĴ���
}
//***********************************************
unsigned char uart0_receive(void) 
{
while(!(UCSRA&(1<<RXC)));  //�ȴ���������
return UDR;                //���ص���������
}
//-----------------------------------------------
void main(void)
{
unsigned char temp;
init_devices();
 while(1) 
 {
 temp=uart0_receive();     //�ȴ���������
 PORTB=~temp;              //���յ�����ת���ɵ͵�ƽ�����LED
 uart0_send(temp);         //�����յ������ݷ��ͳ�ȥ
 }
}
