#include<iom16v.h>
#include <macros.h>

#include "util.h"
#include "iic.h"

#define SET_SCL (PORTC|=0x80)
#define CLR_SCL (PORTC&=0x7f)
#define SET_SDA (PORTC|=0x40)
#define CLR_SDA (PORTC&=0xbf)
#define TEST_SDA ((PINC&0x40)?1:0)
#define SDA_OUT (DDRC|=0x40)
#define SDA_IN (DDRC&=0xbf,PORTC|=0x40)

//pc7 scl
//pc6 sda
void iicport_init(void)
{
    //hi-z
    DDRC &= 0x3f;
    PORTC &= 0x3f;
    return;
}

/* * * * * �����Ƕ�IIC���ߵĲ����ӳ��� * * * * */   
/* * * * * * �������� * * * * */   
void start(void)    
{
    //pc6 and pc7 output mode.
    DDRC |= 0xc0;
    PORTC |= 0xc0;
    NOP();
    //SCL=0; /* SCL���ڸߵ�ƽʱ,SDA�Ӹߵ�ƽת��͵�ƽ��ʾ */   
    //CLR_SCL;
    //SDA=1; /* һ��"��ʼ"״̬,��״̬��������������֮ǰִ�� */   
    SET_SDA;
    NOP();
    //SCL=1;
    SET_SCL;
    NOP(); NOP(); NOP();    
    //SDA=0;
    CLR_SDA;
    NOP(); NOP(); NOP(); NOP();    
    //SCL=0;
    CLR_SCL;
    //SDA=1;
    //SET_SDA;

    return;
}    
   
/* * * * * ֹͣIIC���� * * * * */   
void stop(void)    
{     
    //SCL=0; /*SCL���ڸߵ�ƽʱ,SDA�ӵ͵�ƽת��ߵ�ƽ */
    //CLR_SCL;   
    //SDA=0; /*��ʾһ��"ֹͣ"״̬,��״̬��ֹ����ͨѶ */
    CLR_SDA;
    NOP();
    //SCL=1;
    SET_SCL;
    NOP(); NOP(); NOP(); /* �ղ��� */
    //SDA=1;
    SET_SDA;
    NOP(); NOP(); NOP();
    //SCL=0;
    //CLR_SCL;

    //hi-z
    DDRC &= 0x3f;
    PORTC &= 0x3f;
    return;
}    
   
/* * * * * ���Ӧ��λ * * * * */   
unsigned char recAck(void)    
{
    unsigned char result;
     unsigned char i=0;
    
    //SCL=0;
    //CLR_SCL;   
    //SDA=1;
    SET_SDA;
    SDA_IN;
    //SCL=1;
    SET_SCL;
    //change sda input mode.
    NOP(); NOP(); NOP(); NOP();
    //CY=SDA;     /* ��Ϊ����ֵ���Ƿ���CY�е� */
    while(TEST_SDA&&(i>250)) i++;
    result = TEST_SDA;
    //SCL=0;
    CLR_SCL;
    //SDA_OUT;
    SDA_OUT;
    return result;
}    
   
/* * * * *��IIC���߲���Ӧ�� * * * * */   
void ack(void)    
{     
    //SDA=0; /* EEPROMͨ�����յ�ÿ����ַ������֮��, */
    CLR_SDA;
    //SCL=1; /* ��SDA�͵�ƽ�ķ�ʽȷ�ϱ�ʾ�յ���SDA��״̬ */
    SET_SCL;
    NOP(); NOP(); NOP(); NOP();
    //SCL=0;
    CLR_SCL;   
    NOP();
    //SDA=1;
    SET_SDA;

    return;
}    
   
/* * * * * * * * * ����IIC���߲���Ӧ�� * * * * */   
void noAck(void)    
{    
    //SDA=1;
    SET_SDA;
    //SCL=1;
    SET_SCL;
    NOP(); NOP(); NOP(); NOP();    
    //SCL=0;
    CLR_SCL;
    
    return;
}    
   
/* * * * * * * * * ��IIC����д���� * * * * */   
void sendByte(unsigned char byte)    
{
    unsigned char mask = 0x80;
    for(;mask>0;)    
    {
        //SCL=0;
        CLR_SCL;
        NOP();NOP();
        if(mask&byte)
        {
            //SDA=1;
            SET_SDA;
        }
        else
        {
            //SDA=0;
            CLR_SDA;
        }
        mask >>= 1;
        NOP();NOP();
        //SCL=1;
        SET_SCL;
        NOP();NOP();
    }
    //SCL=0;
    CLR_SCL;

    return;
}
   
/* * * * * * * * * ��IIC�����϶������ӳ��� * * * * */   
unsigned char receiveByte(void)    
{     
    unsigned char receivebyte = 0, i=8;     
    //SCL=0;
    CLR_SCL;
    //SDA = 1;
    SET_SDA;
    SDA_IN;
    NOP();NOP();
    while(i--)    
    {     
        //SCL=1;
        SET_SCL;
        NOP();NOP();
        receivebyte = (receivebyte <<1 ) | TEST_SDA;
        //SCL=0;
        CLR_SCL;
        NOP();
    }
    SDA_OUT;
    return receivebyte;    
}


#define MAXRETRY    10

char rwiic(unsigned char comAddr, unsigned char *data,unsigned char len,unsigned int addr, unsigned char rwFlag)    
{
    unsigned char i = MAXRETRY;
    char err = 1;  /*   �����־   */   
    while(i--)    
    {    
        start();  /*   ��������   */
        sendByte(comAddr |0x00); /*   ��IIC����д���ݣ�������ַ */   
        if(recAck()) continue; /*   ��д����ȷ��������ѭ��   */   
        sendByte((unsigned char)(addr >> 8));//����������ת��Ϊ�ַ������ݣ�����ȡ�ͣ�ֻȡ��8λ.�����������32Kλ��ʹ��16λ��ַѰַ��д��߰�λ��ַ    
        if(recAck())  continue;    
        sendByte((unsigned char)addr); /*   ��IIC����д����   */   
        if(recAck())  continue; /*   ��д��ȷ��������ѭ��   */
        if(rwFlag == RWWRITE)   //�ж��Ƕ���������д����    
        {
            err=0;         /* ���������λ */   
            while(len--)    
            {
                sendByte(*(data++)); /*   ��IIC����д����   */   
                if(!recAck()) continue; /*   ��д��ȷ��������ѭ��   */   
                err=1;    
                break;    
            }    
            if(err==1) continue;    
            break;    
        }    
        else   
        { 
            start();  /*   ��������   */   
            sendByte(comAddr |0x01); /*   ��IIC����д����   */   
            if(recAck()) continue;//����ûӦ��������α���ѭ��    
            //ѭ������Ҫ��һ��
            len--;
            while(len--)  /*   �ֽڳ�Ϊ0����   */   
            {
                *(data++)= receiveByte();    
                ack();   /*   ��IIC���߲���Ӧ��   */   
            }    
            *data=receiveByte(); /* �����һ���ֽ� */   
            noAck();  /*   ����IIC���߲���Ӧ��   */   
            err=0;    
            break;    
        }    
    }    
    stop();  /*   ֹͣIIC����   */   
    if(rwFlag == RWWRITE)    
    {     
        delay_ms(50);    
    }    
    return err;    
}    

/*
unsigned long rwiicInt(unsigned char comAddr, unsigned long data, unsigned int addr, unsigned char rwFlag)
{
    unsigned char tmp[4] = {0, 0, 0, 0};
	unsigned long out = 0;

    if(rwFlag==RWWRITE)
    {
        tmp[0] = (unsigned char)(data&0x000000ff);
        tmp[1] = (unsigned char)((data>>8)&0x000000ff);
        tmp[2] = (unsigned char)((data>>16)&0x000000ff);
        tmp[3] = (unsigned char)((data>>24)&0x000000ff);
        rwiic(comAddr, (unsigned char*)&tmp, 4, addr, RWWRITE);
		out = data;
    }
    else
    {
        rwiic(comAddr, (unsigned char*)&tmp, 4, addr, RWREAD);

        out= 0;
        out |= tmp[0];
        out <<= 8;
        out |= tmp[1];
        out <<= 8;
        out |= tmp[2];
        out <<= 8;
        out |= tmp[3];
    }
    return out;
}
*/

