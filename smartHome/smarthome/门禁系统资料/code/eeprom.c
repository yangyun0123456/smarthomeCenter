#include<iom16v.h>
#include <macros.h>

#include "util.h"
#include "iic.h"
#include "eeprom.h"

/* һ��ͨ�õ�24C01��24C256��9��EEPROM���ֽڶ�д��������   
�˳������������������ֱ�Ϊ��д���ݻ�����ָ��,   
���ж�д���ֽ�����EEPROM��ַ��EEPROM�����ֽڣ�   
�Լ�EEPROM���͡��˳���ṹ�����ã����м��õ��ݴ��ԣ����������Ҳ����:   
DataBuffΪ��д�������룯�������������ַ   
Length ΪҪ��д���ݵ��ֽ�����   
Addr ΪEEPROM��Ƭ�ڵ�ַ AT24256Ϊ0��32767   
Control ΪEEPROM�Ŀ����ֽڣ�������ʽΪ(1)(0)(1)(0)(A2)(A1)(A0)(R/W),����R/W=1,   
��ʾ������,R/W=0Ϊд����,A2,A1,A0ΪEEPROM��ҳѡ��Ƭѡ��ַ;   
enumerΪö�ٱ���,��ΪAT2401��AT24256�е�һ��,�ֱ��ӦAT24C01��AT24C256;   
��������ֵΪһ��λ������������1��ʾ�˴β���ʧЧ��0��ʾ�����ɹ�;   
ERRORΪ������������������ERRORCOUNT�β���ʧЧ��������ֹ������������1   
SDA��SCL���û��Զ��壬�����ݶ���ΪP3^0��P3^1; */   
/*����1Kλ��2Kλ��4Kλ��8Kλ��16KλоƬ����һ��8λ�����ֽڵ�ַ�룬����32Kλ����   
�Ĳ���2��8λ�����ֽڵ�ַ��ֱ��Ѱַ����4Kλ��8Kλ��16Kλ���ҳ���ַ��Ѱַ*/   
   
/* ����������  AT24C01��AT24C256 �Ķ�д���� ������������ */

#define AT24C256DEVADDR 0xa0

char rw24c256(unsigned char *data,unsigned char len,unsigned int addr, unsigned char rwFlag)
{
    return rwiic(AT24C256DEVADDR, data, len, addr, rwFlag);
}
