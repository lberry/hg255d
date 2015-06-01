#include <REGX51.H>
//#include "spi.h"
#include "XPT2046.h"
#include "temp.h"
unsigned int ain0=0,ain1=0,ain2=0,ain3=0;
int tttt=0;
char xt;
unsigned char y=0;

void send(int temp);
void send1(int temp);


void IntSerial(void)
{				 //	 MODE  0  1	 2	3
	SCON=0<<7 |  //SM0     0  0	 1	1
	     1<<6 |  //SM1     0  1	 0	1
	     0<<5 |  //SM2
	     1<<4 |  //REN	   ���н�������λ
	     0<<3 |  //TB8	   ��8λ���ͣ�ģʽ2��3	 ֹͣλ
	     0<<2 |  //RB8	   ��8λ���գ�ģʽ2��3
	     0<<1 |  //T1
	     0<<0 ;  //R1
//	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X21;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XFF;		    //��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XFF;
	TH0=0;
	TL0=0;
	ES=1;						//�򿪽����ж�
	ET0=1;
	EA=1;						//�����ж�
	TR1=1;					    //�򿪼�����     
	     
}

/*
void send(char xx)
{
		SBUF=xx;
	while(!TI);TI=0;

}	   */
/* 
 void turnum(int num,unsigned char x)
{
	if(num>100)
	{
		send('1');
		send(num/10%10+'0');
		send(num%10+'0');	
	}

}	  */

void main()
{	unsigned int count=0;
	unsigned char cho=0;
	SPI_Start()	;
	IntSerial();//send(44);
	//send(89);
	while(1)
	{


	  if(count>55)
	  {
	  	count=0; 
		
		switch(cho++)
		{
		case 0:
			ain0 = Read_AD_Data(0x94);
			break;		//   AIN0 ��λ��
		case 1:
			ain2 = Read_AD_Data(0xD4);
			break;		//   AIN1 ��������
		case 2:
			ain1 = Read_AD_Data(0xA4);
			break;		//   AIN2 ��������
		case 3:
			ain3 = Read_AD_Data(0xE4);
			break;		//   AIN3 �ⲿ����
	    case 4:
			tttt=Ds18b20ReadTemp() ;
			//P0=tttt;
			break;
		default:
			cho=0;
		}
	//	cho++;
			
	  }
	  count++;



	/*   if(y==1) {Delay1ms(1000);
		switch(0)
		{
		case 'a':
			send(tttt);
			break;
		case 'b':
			send1(ain0);
			break;
		case 'c':
			send1(ain1);
			break;
		case 'd':
			send1(ain2);
			break;
		case 'e':
			send1(ain3);
			break;
		default:;// P0=t;//send(0);
		}	y=0;
	  }		 */

	  }
}

void dd() interrupt 4
{ char t;
static char a;
	//unsigned char x;
	t=SBUF;
	if(t==a)
	{
		RI=0;
		return; 
	}
	else {a=t;
	//P0=a;
	y=1;	 }
	RI=0; 
	
	if(t>'a'&&t<'z'){TH0=0;TL0=0;TR0=1;xt=t; }
/*	switch(t)
		{
		case 'a':
			send(tttt);
			break;
		case 'b':
			send1(ain0);
			break;
		case 'c':
			send1(ain1);
			break;
		case 'd':
			send1(ain2);
			break;
		case 'e':
			send1(ain3);
			break;
		default: P0=t;//send(0);
		}	y=0;
*/
/*	if(t=='a')send(tttt);
	else if(t=='b')send1(ain0);
	else if(t=='c')send1(ain2);
	else if(t=='d')send1(ain1);
	else if(t=='e')send1(ain3);
	else send(0);
*/	 
	
}

void fsdfas() interrupt 1
{
		 TR0=0;
			switch(xt)
		{
		case 'a':
			send(tttt);
			break;
		case 'b':
			send1(ain0);
			break;
		case 'c':
			send1(ain1);
			break;
		case 'd':
			send1(ain2);
			break;
		case 'e':
			send1(ain3);
			break;
		default:;// P0=t;//send(0);
		}
	
}



void send(int temp) 	 //lcd��ʾ
{
    
  	unsigned char datas[] = {0, 0, 0, 0, 0}; //��������
	float tp;  
	if(temp< 0)				    //���¶�ֵΪ����
  	{
	  //	LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
		SBUF='-';               //�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
	   // LcdWriteData('-');  	     //��ʾ��
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//����?.5��������С������档
 
  	}
 	else
  	{			
	  	//LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
	   // LcdWriteData('+'); 		//��ʾ��
	//	SBUF='+';//�����յ������ݷ��뵽���ͼĴ���
	//	while(!TI);			         //�ȴ������������
	//	TI=0;						 //���������ɱ�־λ
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

//	LcdWriteCom(0x82);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[0]); //��λ 
	if(datas[0]!=0){
	SBUF = '0'+datas[0];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;	
	}
	
//	LcdWriteCom(0x83);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[1]); //ʮλ
if(datas[1]!=0){
	SBUF = '0'+datas[1];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;
	}
//	LcdWriteCom(0x84);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[2]); //��λ 
	SBUF = '0'+datas[2];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

//	LcdWriteCom(0x85);		//д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('.'); 		//��ʾ ��.��
	SBUF = '.';             //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			//�ȴ������������
	TI = 0;

//	LcdWriteCom(0x86);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[3]); //��ʾС����  
	SBUF = '0'+datas[3];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

//	LcdWriteCom(0x87);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[4]); //��ʾС���� 
if(datas[4]!=0){
	SBUF = '0'+datas[4];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;
	}
/*	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];       //�����յ������ݷ��뵽���ͼĴ���
		while (!TI);	        //�ȴ������������
		TI = 0;
	}
	 */
	 
}




void send1(int temp) 	 //lcd��ʾ
{
    
  	unsigned char datas[] = {0, 0, 0, 0, 0}; //��������
/*//	float tp;  
	if(temp< 0)				    //���¶�ֵΪ����
  	{
	  //	LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
		SBUF='-';               //�����յ������ݷ��뵽���ͼĴ���
		while(!TI);			         //�ȴ������������
		TI=0;						 //���������ɱ�־λ
	   // LcdWriteData('-');  	     //��ʾ��
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//����?.5��������С������档
 
  	}
 	else
  	{			
	  	//LcdWriteCom(0x80);		//д��ַ 80��ʾ��ʼ��ַ
	   // LcdWriteData('+'); 		//��ʾ��
	//	SBUF='+';//�����յ������ݷ��뵽���ͼĴ���
	//	while(!TI);			         //�ȴ������������
	//	TI=0;						 //���������ɱ�־λ
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	}  */
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

//	LcdWriteCom(0x82);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[0]); //��λ 
	if(datas[0]!=0){
	SBUF = '0'+datas[0];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;	
	}
	
//	LcdWriteCom(0x83);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[1]); //ʮλ
if(datas[1]!=0){
	SBUF = '0'+datas[1];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;
	}
//	LcdWriteCom(0x84);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[2]); //��λ 
	SBUF = '0'+datas[2];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

//	LcdWriteCom(0x85);		//д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('.'); 		//��ʾ ��.��
	SBUF = '.';             //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			//�ȴ������������
	TI = 0;

//	LcdWriteCom(0x86);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[3]); //��ʾС����  
	SBUF = '0'+datas[3];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;

//	LcdWriteCom(0x87);		    //д��ַ 80��ʾ��ʼ��ַ
//	LcdWriteData('0'+datas[4]); //��ʾС���� 
if(datas[4]!=0){
	SBUF = '0'+datas[4];        //�����յ������ݷ��뵽���ͼĴ���
	while (!TI);			    //�ȴ������������
	TI = 0;
	}
/*	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];       //�����յ������ݷ��뵽���ͼĴ���
		while (!TI);	        //�ȴ������������
		TI = 0;
	}
	 */
	 
}