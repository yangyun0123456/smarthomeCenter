/*********************************************************/
/* Target : ATmage16L                                    */
/* Crystal: 8.00MHz                                      */
/* KV-878 PCB                                            */
/*ң���ͺţ�UPD6121    //LCD 1602                        */
/*Ӳ���������ţ�PD6(ICP1) //PIN15                        */
/*���뷽ʽ����ʱ�������ж�                               */
/*********************************************************/
 #define LED_1  PORTD|=BIT(PD1)   //
 #define LED_0  PORTD&=~BIT(PD1)  //����
 
 #define RS_0    PORTC &= ~(1 << PC0)
 #define RS_1    PORTC |= (1 << PC0)
 //RSΪ�Ĵ���ѡ�񣬸ߵ�ƽʱѡ�����ݼĴ������͵�ƽʱѡ��ָ��Ĵ�����
 #define RW_0    PORTC &= ~(1 << PC1)
 #define RW_1    PORTC |= (1 << PC1)
//RWΪ��д�ź��ߣ��ߵ�ƽʱ���ж��������͵�ƽʱ����д������
//��RS��RW��ͬΪ�͵�ƽʱ����д��ָ�������ʾ��ַ����RSΪ
//�͵�ƽRWΪ�ߵ�ƽʱ���Զ�æ�źţ���RSΪ�ߵ�ƽRWΪ�͵�ƽʱ����д�����ݡ�

 #define EN_0    PORTC &= ~(1 << PC2)
 #define EN_1    PORTC |= (1 << PC2)
//EN��Ϊʹ�ܶˣ���E���ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ��ִ�����  
//*************ͷ�ļ�*******************
 #include <iom16v.h>
 #include <macros.h>
 #include <eeprom.h>
 #define  uchar unsigned char 
 #define  uint unsigned int  
////////////////////////////////////////

uchar shukk[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};/*0��9��ASCII��ֵ*/
uchar addr[4]={0x85,0x86,0x87,0x88};/*��ʾ��ʼ��ַ*/
uchar llbit=1;
uchar i,j,unin,head,irsign=0;//unin������־��head������־��irsign�����־
uchar bitnum,data0,data1,data2,data3;//bitnumλ����data0~3�Ĵ����ֽ�����
 //*************I/O������*****************
 void port_init(void)
 {  //--PA,PB����--
    PORTA = 0xff;                /*������*/
    DDRA = 0xff;                /*��������*/
    PORTB = 0xff;                /*��ƽ����*/
    DDRB = 0x00;                /*�������*/
	//--PC���ݣ�PD��ͨѶ
    PORTC = 0xff;
    DDRC = 0xff;
    PORTD = 0xfe;
    DDRD = 0x3f;

 }
 //***************1���뼶��ʱ*************
 void ys1ms (void)
 {
   uint i;
   for (i=1324;i>0;i--);
  }  
//***************n������ʱ****************
 void delaym (uint n)
 {
   uint i=0;
   for (i=0;i<n;i++)
   ys1ms();
  }   
 //************дEEPROM*****************   
   void EEPROM_W(uint eepadd,uchar eepdat)
   {
    CLI();
    delaym(3);
    EEAR=eepadd;            //�͸ߵ�ַ
	EEDR=eepdat;            //������
	EECR=EECR|0x04;         //��дʹ����λ
	EECR=EECR|0x02;         //дʹ����λ
	EECR=0;
	delaym(3);
	SEI();
   }
 //-------------------------------------
 void timer1_init(void)
 {
  TIMSK=0x20; //ʹ��T1�����ж� 
  TCCR1A=0x00;//ʹ���������ƣ��½��ش���
  TCCR1B=0x82;//T/C1ʱ��Ϊϵͳʱ��/8��ʱ������1us
  SEI();  //���ж�
  
  }
//==============T0��ʼ========================
  void  t0_start (void)	 
  {
   TCNT0=0x00;                  //���
   TCCR0=0x05;                  //1024��Ƶ
   TIMSK=(1<<TOIE0)|(1<<TICIE1);//t0����жϿ�
   SEI(); 
  }  	
 //=============RXD===========================
 void rxd_init(void)
 {
 UCSRB=0x00;//��ֹ���պͷ���
 UCSRA=0x02;//����
 UCSRC=0x06;//8λ����
 UBRRL=0x67;
 UBRRH=0x00;//9600b/s������
 UCSRB=0x18;//�������շ���
 }	
//=============sendһ�ֽ�=============================
 void send(uchar a)
  {
  while(!(UCSRA&(1<<UDRE)));//UDREΪ0˵�������������
  UDR=a;
  } 	
//==============����3�ֽڴ���=========================
void send3(uchar subr)
 {LED_1;
  send(0x55); 
  delaym(2);
  send(subr);
  delaym(2);
  send(0x81);
  delaym(2);
  LED_0;
  } 
//----------------------------------  
  //΢�뼶��ʱ������8MHZ
void delay_us(int time)
{    
 do
  {
   time--;
  }    
 while (time>1);
}
//-----------------------------------      
//���뼶��ʱ������8MHZ 
void delay_ms(uint time)
{
 while(time!=0)
  {        
   delay_us(1000);
   time--;
  }
}
//-------------------------------------
/*��ʾ������д�뺯��*/
void LCD_write_com(uchar com) 
  {
   RS_0;//�͵�ƽ����
   RW_0;//�͵�ƽд
   PORTA = com;
   //PORTB = com;
   EN_1;
   delay_us(20);
   EN_0;//EN�ɸߵ���ʹ��
  }
//-------------------------------------
/*��ʾ������д�뺯��*/
void LCD_write_data(uchar data) 
 {
  RS_1;//�ߵ�ƽ����
  RW_0;//�͵�ƽд
  PORTA = data;
  //PORTB = data;
  EN_1;
  delay_us(200);
  EN_0;//EN�ɸߵ���ʹ��
}
//-----------------------------------
/*��ʾ�������ʾ*/
void LCD_clear(void) 
 {
  LCD_write_com(0x01);//����ʾ��ָ����01H,��긴λ����ַ00Hλ��
  delay_ms(5);
 }
//----------------------------------
/*��ʾ���ַ���д�뺯��*/
void LCD_write_str(uchar x,uchar y,uchar *s)//x��ʼλ�ã�y��λ�ã�*s���� 
 {
  if (y == 0)//1��
  {
   LCD_write_com(0x80 + x);
  }
  else      //2��
  {
   LCD_write_com(0xC0 + x);
  }
    
  while (*s)
  {
   LCD_write_data( *s);
   s ++;
   //delay_ms(5);
  }
}
//------------------------------------
/*��ʾ�����ַ�д�뺯��*/
void LCD_write_char(uchar x,uchar y,uchar data) //x��ʼλ�ã�y��λ�ã�data����
 {
  if (y == 0)//1��
   {
    LCD_write_com(0x80 + x);
   }
  else      //2��
   {
    LCD_write_com(0xC0 + x);
   }
    
    LCD_write_data(data);  
 }
//---------------------------------------
/*��ʾ����ʼ������*/
void LCD_init(void) 
 {
   DDRA = 0xff;                        /*I/O�ڷ�������*/
   DDRC |= (1 << PC0) | (1 << PC1) | (1 << PC2);
   
   delay_ms(15);
   LCD_write_com(0x38);                /*��ʾģʽ����*/
   delay_ms(5);
   LCD_write_com(0x38);
   delay_ms(5);
   LCD_write_com(0x38);
   delay_ms(5);
   LCD_write_com(0x38);                //16*2��ʾ��5*7����8λ����
    
   LCD_write_com(0x08);                /*��ʾ�ر�*/
   LCD_write_com(0x01);                /*��ʾ����*/
   LCD_write_com(0x06);                /*��ʾ����ƶ����ã���д1�ַ������1*/
   LCD_write_com(0x0C);                /*��ʾ��*/
   delay_ms(5);
}

//-----------------------------------------
void main(void) 
{
 uchar i;
 uchar *p;
 delay_ms(50);
 port_init();
 delay_ms(50);
 timer1_init();     //��ʱ����ʼ��  
 delay_ms(50);
 LCD_init();        //LCD��ʼ��
 delay_ms(200);
 LCD_clear();       //��LCD
 delay_ms(200);
//-------------------------------- 
  {loop:
   i = 4;
   p = "Welcome";
   //LCD_write_str(2,1,"I Love You");
   delay_ms(50);
        
   while (*p) 
    {
     LCD_write_char(i,0,*p);
     i ++;
     p ++;
     delay_ms(180);
    }
	///////////////
	i=2;
	p="haohandianzi";
	 while (*p) 
    {
     LCD_write_char(i,1,*p);
     i ++;
     p ++;
     delay_ms(181);
    }
   delay_ms(500);
   llbit=0;
   //LED_0;
  }
 /*/-------------------------------------
 while(1)
 {
  if(irsign==1)
   {
    irsign=0;
	//----------------------------------
	if(data2==0x58)
	 {
     LCD_clear();
     LCD_write_str(6,0,"REDAY!");//�ո�6��1��
     delay_ms(50);
     LCD_write_str(6,1,"RESET");//�ո�5��2��
     delay_ms(800);
	 LCD_clear();
	 goto loop;
	 }
     //--------------------------------------
	 else if((data2==0xd8)&&(llbit==0))
	  {
	   LCD_clear();
	   llbit=1;
	   LED_1;
	   LCD_write_str(1,0,"background lamp");
	   delay_ms(50);
	   LCD_write_str(6,1,"ON");
	   delay_ms(50);
	  }
	 else if((data2==0xd8)&&(llbit==1))
	  {
	   LCD_clear();
	   llbit=0;
	   LED_0;
	   LCD_write_str(1,0,"background lamp");
	   delay_ms(50);
	   LCD_write_str(6,1,"OFF");
	   delay_ms(50);
	  } 
	//----------------------------------
	if(data2==0x70)
	 {
	  LCD_clear();
      LCD_write_str(2,0,"This code is");
	  delay_ms(50);
	  LCD_write_str(6,1,"0x70");
	 } 
    //----------------------------------
	if(data2==0xf0)
	 {
	  LCD_clear();
      LCD_write_str(2,0,"This code is");
	  delay_ms(50);
	  LCD_write_str(6,1,"0xf0");
	 }     
	//----------------------------------
	if(data2==0x48)
	 {
	  LCD_clear();
      LCD_write_str(2,0,"This code is");
	  delay_ms(50);
	  LCD_write_str(6,1,"0x48");
	 }   
	//----------------------------------
	if(data2==0xc8)
	 {
	  LCD_clear();
      LCD_write_str(2,0,"This code is");
	  delay_ms(50);
	  LCD_write_str(6,1,"0xc8");
	 } 
	//----------------------------------
	if(data2==0x68)
	 {
	  LCD_clear();
      LCD_write_str(2,0,"This code is");
	  delay_ms(50);
	  LCD_write_str(6,1,"0x68");
	 }
   //----------------------------------
	if(data2==0xe8)
	 {
	  LCD_clear();
      LCD_write_str(2,0,"This code is");
	  delay_ms(50);
	  LCD_write_str(6,1,"0xe8");
	 }     	                    
    }
  } */	
}
 
//--------------�ж�----------------------
#pragma interrupt_handler timer1_in:6
 void timer1_in(void)
 {
  static uint old_data;
   uint temp,temp0,new_data;//�������
   new_data=ICR1;           //��ֵ
   temp=new_data-old_data;  //��������
   old_data=new_data;       //��������
   
  if(temp>1024&&temp<1225)     //0
   {temp0=0;}
  else if(temp>2145&&temp<2345)//1
   {temp0=1;}
  else if(temp>11150&&temp<11350)//����
   {unin=1;   //�������־��λ
    bitnum=0; //λ��
    data0=0;  //1�ֽ�����					  
    data1=0;  //2�ֽ�����
    data2=0;  //3�ֽ�����
    data3=0;  //4�ֽ�����
    return;   //Ϊuninterrupted���ش��´ν���
   }
  else if(temp>13400&&temp<13600)//����
  {
   head=1;   //�������־��λ
   bitnum=0; //λ��
   data0=0;  //1�ֽ�����					  
   data1=0;  //2�ֽ�����
   data2=0;  //3�ֽ�����
   data3=0;  //4�ֽ�����
   return;   //Ϊheader���ش��´ν���
  }
  else 
  return;    //���������ϣ�Ϊ�������壬�������ؽ���
  
  bitnum++;  //Ϊ0��1ʱλ���ݼ�
 if(bitnum<8)//С��8λʱ����1
 {data0=data0|(uint)temp0;
  data0=data0<<1;}   //����
  else if(bitnum==8) //����8ʱ�����һλ����1�ֽ�
  {data0=data0|(uint)temp0;}
  else if(bitnum<16) //С��16λʱ����2�ֽ�
  {data1=data1|(uint)temp0;
   data1=data1<<1;}
  else if(bitnum==16)//����16ʱ�����һλ����2�ֽ�
  {data1=data1|(uint)temp0;}
  
  else if(bitnum<24) //С��16λʱ����3�ֽ�
  {data2=data2|(uint)temp0;
   data2=data2<<1;}
  else if(bitnum==24)//����16ʱ�����һλ����3�ֽ�
  {data2=data2|(uint)temp0;}
  
   else if(bitnum<32)//С��32λʱ����4�ֽ�
  {data3=data3|(uint)temp0;
   data3=data3<<1;}
   
  else if(bitnum==32)//����32ʱ�����һλ����4�ֽ�
  {data3=data3|(uint)temp0;
   bitnum=0;        //λ����0
   irsign=1;        //IR��ɱ�־λ��λ
   CLI();           //���ж��ݹ�
   } 
 }        
///////////////////////////////////////////////////////
 //-------------�����жϴ���---------------------
 #pragma  interrupt_handler int0:2 //�ⲿ�ж�0
 void  int0 (void)		
   {return;}	
 //------------------------------------------
 #pragma  interrupt_handler int1:3 //�ⲿ�ж�1
 void  int1 (void)		
   {return;
   }
 //------------------------------------------	
 #pragma  interrupt_handler t2_comp:4  //��ʱ��2�Ƚ�
 void  t2_comp (void)		
   {return; }	
 //------------------------------------------
 #pragma  interrupt_handler t2_ovf:5  //��ʱ��2���
 void  t2_ovf (void)		
   {return;}	   
 //------------------------------------------
 #pragma  interrupt_handler t1_compa:7  //��ʱ��1�Ƚ�a
 void  t1_compa (void)		
   {return;    
   }	
 //------------------------------------------  
 #pragma  interrupt_handler t1_compb:8  //��ʱ��1�Ƚ�b
 void  t1_compb (void)		
   {return;    
   }	
 //------------------------------------------
 #pragma  interrupt_handler t1_ovf:9   //��ʱ��1���
 void  t1_ovf (void)		
   {
    return;
   }	  
 //------------------------------------------
 #pragma  interrupt_handler t0_ovf:10  //��ʱ��0��������Զ���OSD
 void  t0_ovf (void)		
   {return;}
 //------------------------------------------
 #pragma  interrupt_handler spi_end:11  //SPI����
 void  spi_end (void)		
   {return;}	  
 //------------------------------------------
 #pragma  interrupt_handler rx_end:12  //rx����
 void  rx_end (void)		
   {return;    
   }	  
 //------------------------------------------
 #pragma  interrupt_handler usart_nop:13  //USART��
 void  usart_nop (void)		
   { return;   
   }	 
 //------------------------------------------
 #pragma  interrupt_handler tx_end:14   //tx����
 void  tx_end (void)		
   {return;    
   }	  
//------------------------------------------
 #pragma  interrupt_handler adc_end:15  //adv����
 void  adc_end (void)		
   { return;   
   }	   
//------------------------------------------
 #pragma  interrupt_handler eep_rdy:16  //eeprom����
 void  eep_rdy (void)		
   { return;   
   }	
//------------------------------------------
 #pragma  interrupt_handler ana_comp:17  //ģ��Ƚ���
 void  ana_comp (void)		
   { return;   
   }	  
//------------------------------------------
 #pragma  interrupt_handler twi:18       //iic
 void  twi (void)		
   {  return;  
   }	  
 //------------------------------------------
 #pragma  interrupt_handler spm_rdy:19   //���򴢴����
 void  spm_rdy (void)		
   {  return;  
   }
   	     
//============================================
//-----------------------end------------------
//============================================  
 
 
  