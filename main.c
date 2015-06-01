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
	     1<<4 |  //REN	   串行接受允许位
	     0<<3 |  //TB8	   第8位发送，模式2，3	 停止位
	     0<<2 |  //RB8	   第8位接收，模式2，3
	     0<<1 |  //T1
	     0<<0 ;  //R1
//	SCON=0X50;			//设置为工作方式1
	TMOD=0X21;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XFF;		    //计数器初始值设置，注意波特率是4800的
	TL1=0XFF;
	TH0=0;
	TL0=0;
	ES=1;						//打开接收中断
	ET0=1;
	EA=1;						//打开总中断
	TR1=1;					    //打开计数器     
	     
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
			break;		//   AIN0 电位器
		case 1:
			ain2 = Read_AD_Data(0xD4);
			break;		//   AIN1 热敏电阻
		case 2:
			ain1 = Read_AD_Data(0xA4);
			break;		//   AIN2 光敏电阻
		case 3:
			ain3 = Read_AD_Data(0xE4);
			break;		//   AIN3 外部输入
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



void send(int temp) 	 //lcd显示
{
    
  	unsigned char datas[] = {0, 0, 0, 0, 0}; //定义数组
	float tp;  
	if(temp< 0)				    //当温度值为负数
  	{
	  //	LcdWriteCom(0x80);		//写地址 80表示初始地址
		SBUF='-';               //将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
	   // LcdWriteData('-');  	     //显示负
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
 
  	}
 	else
  	{			
	  	//LcdWriteCom(0x80);		//写地址 80表示初始地址
	   // LcdWriteData('+'); 		//显示正
	//	SBUF='+';//将接收到的数据放入到发送寄存器
	//	while(!TI);			         //等待发送数据完成
	//	TI=0;						 //清除发送完成标志位
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

//	LcdWriteCom(0x82);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[0]); //百位 
	if(datas[0]!=0){
	SBUF = '0'+datas[0];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;	
	}
	
//	LcdWriteCom(0x83);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[1]); //十位
if(datas[1]!=0){
	SBUF = '0'+datas[1];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;
	}
//	LcdWriteCom(0x84);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[2]); //个位 
	SBUF = '0'+datas[2];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

//	LcdWriteCom(0x85);		//写地址 80表示初始地址
//	LcdWriteData('.'); 		//显示 ‘.’
	SBUF = '.';             //将接收到的数据放入到发送寄存器
	while (!TI);			//等待发送数据完成
	TI = 0;

//	LcdWriteCom(0x86);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[3]); //显示小数点  
	SBUF = '0'+datas[3];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

//	LcdWriteCom(0x87);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[4]); //显示小数点 
if(datas[4]!=0){
	SBUF = '0'+datas[4];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;
	}
/*	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];       //将接收到的数据放入到发送寄存器
		while (!TI);	        //等待发送数据完成
		TI = 0;
	}
	 */
	 
}




void send1(int temp) 	 //lcd显示
{
    
  	unsigned char datas[] = {0, 0, 0, 0, 0}; //定义数组
/*//	float tp;  
	if(temp< 0)				    //当温度值为负数
  	{
	  //	LcdWriteCom(0x80);		//写地址 80表示初始地址
		SBUF='-';               //将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
	   // LcdWriteData('-');  	     //显示负
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
 
  	}
 	else
  	{			
	  	//LcdWriteCom(0x80);		//写地址 80表示初始地址
	   // LcdWriteData('+'); 		//显示正
	//	SBUF='+';//将接收到的数据放入到发送寄存器
	//	while(!TI);			         //等待发送数据完成
	//	TI=0;						 //清除发送完成标志位
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}  */
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

//	LcdWriteCom(0x82);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[0]); //百位 
	if(datas[0]!=0){
	SBUF = '0'+datas[0];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;	
	}
	
//	LcdWriteCom(0x83);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[1]); //十位
if(datas[1]!=0){
	SBUF = '0'+datas[1];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;
	}
//	LcdWriteCom(0x84);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[2]); //个位 
	SBUF = '0'+datas[2];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

//	LcdWriteCom(0x85);		//写地址 80表示初始地址
//	LcdWriteData('.'); 		//显示 ‘.’
	SBUF = '.';             //将接收到的数据放入到发送寄存器
	while (!TI);			//等待发送数据完成
	TI = 0;

//	LcdWriteCom(0x86);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[3]); //显示小数点  
	SBUF = '0'+datas[3];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;

//	LcdWriteCom(0x87);		    //写地址 80表示初始地址
//	LcdWriteData('0'+datas[4]); //显示小数点 
if(datas[4]!=0){
	SBUF = '0'+datas[4];        //将接收到的数据放入到发送寄存器
	while (!TI);			    //等待发送数据完成
	TI = 0;
	}
/*	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];       //将接收到的数据放入到发送寄存器
		while (!TI);	        //等待发送数据完成
		TI = 0;
	}
	 */
	 
}