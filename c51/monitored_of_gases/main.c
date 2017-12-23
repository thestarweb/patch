#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar code display_wei[]={0xF1,0xF2,0xF4,0xF8};
uchar code codes[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uchar code display_table[]={0xBF,0x8c};//-,p
sbit OE=P3^1;
sbit ST=P3^4;
sbit EOC=P3^2;

#define BAOJIN_SPEED 6

//uchar test=2;

#define MAX_SCAN_DELAY 20 
uchar scan_delay=0;

uchar inputs[]={0,0,0,0,0,0,0,0};//存放扫描的结果
uchar input_max[]={10,20,25,100,70,60,24,150};//存放上限值（超过该值后报警）
uchar now_input;//存放当前输入扫描位置
uchar now_scan=0;
bit st=0;

uchar bdata baojin=0x00;//存放报警
sbit baojin0=baojin^0;
sbit baojin1=baojin^1;
sbit baojin2=baojin^2;
sbit baojin3=baojin^3;
sbit baojin4=baojin^4;
sbit baojin5=baojin^5;
sbit baojin6=baojin^6;
sbit baojin7=baojin^7;
uchar baojin_max;
bit sound=0;//存放是否声音报警
bit shanshuo=0;//存放闪烁

uchar b=0,b2=0;//记录

uchar now_display=0;//存放当前显示位置
uchar display_speed=20;

void usleep(uint z)
{
	uint x;
	uchar y;
	for(x=0;x<z;x++)
		for(y=0;y<110;y++);
}

//启动ADC
void start_ADC()
{
	P0=0xF0;//防止干扰显示
	P2=now_scan;//设置当前读取那一路数据
	ST=1;//脉冲上升沿复位
	usleep(1);//脉冲宽度至少200ns
	ST=0;//脉冲下降沿启动
}

void found_max()
{
	uchar max=0,i,c,o=baojin_max;
	for(i=0;i<7;i++)
	{
		if(inputs[i]>input_max[i])
		c=inputs[i]-input_max[i];
		if(c>max)
		{
			max=c;
			baojin_max=i;
		}
	}
	if(baojin_max!=o)
	{
		b2=0;//最大值有变化 显示屏数据更新
	}
}
void is_overproof(uchar i)
{
	bit b=0;
	if(inputs[i]>input_max[i])
	{
		b=1;
	}
	switch(i)
	{
		case 0:
			baojin0=b;
			break;
		case 1:
			baojin1=b;
			break;
		case 2:
			baojin2=b;
			break;
		case 3:
			baojin3=b;
			break;
		case 4:
			baojin4=b;
			break;
		case 5:
			baojin5=b;
			break;
		case 6:
			baojin6=b;
			break;
		case 7:
			baojin7=b;
			break;
	}
	if(baojin) found_max();//找到最大的报警量
}

//改变指示灯
void update_LED()
{
	int temp;
	//如果有报警则对应灯闪烁
	if(baojin)
	{
		if(shanshuo) P1=~baojin;
		else P1=0xFF;
	}
	else//如果没有报警 指示灯指示当前显示气体
	{
		temp=0xFE;
		temp=_crol_(temp,now_display);
		P1=temp;
	}
}

//数码管显示原型函数
//uchar i 
void _display(uchar i,uchar d)
{
	P0=display_wei[i];
	P2=d;
	usleep(10);
}
void display_num(uchar input)
{
	uchar temp;
	bit o=0;//标记是否开始第一位数字
	temp=input/100;
	if(temp)//隐藏前导0
	{
		_display(1,codes[temp]);
		o=1;
	}
	temp=(input%100)/10;
	if(temp||o)
	{
		_display(2,codes[temp]);
	}
	temp=(input%10);
	_display(3,codes[temp]);
}
void display()
{
	if(baojin)
	{
		if(b2<BAOJIN_SPEED)
		{
			_display(0,display_table[0]);
			_display(1,display_table[1]);
			_display(2,codes[baojin_max]);
			_display(3,display_table[0]);
		}
		else
		{
			display_num(inputs[baojin_max]);
		}
	}
	else
	{
		display_num(inputs[now_display]);
	}
}

//
void main()
{
	inputs[0]=11;
	EA=1;//开放总中断
	
	//初始化ADC0809
	IT0=1;//设置P3.2边沿触发的中断
	EX0=1;//开放P3.2中断
	ST=0;
	OE=0;
	EOC=1;//EOC置1 否则无法读取外部数据产生中断
	
	//初始化定时器
	ET0=1;//开放定时器1中断
	//初始化定时器
	TMOD=0x01;
	TH0=0x00;
	TL0=0x00;//这里不需要太精确的时间
	TR0=1;
	
	while(1)
	{
		display();
		if(st==0)
		{
			
			if(scan_delay<MAX_SCAN_DELAY)
			{
				scan_delay++;
			}
			else
			{
				st=1;
				scan_delay=0;
				start_ADC();
			}
		}
	}
}

//定时更新信息
void timer() interrupt 1
{
	b++;
	if(baojin)
	{//有报警 闪烁 切换报警显示数据
		if(b>2)
		{
			shanshuo=~shanshuo;
			b=0;
			b2++;
			if(b2==BAOJIN_SPEED*2) b2=0;
		}
	}
	else
	{
		if(b>display_speed)
		{
			b=0;
			now_display++;
			if(now_display==8) now_display=0;//显示下一路数据
		}
	}
	update_LED();
}
void scan_input() interrupt 0
{
	uchar temp=P2;//保护现场
	P2=0xFF;
	OE=1;
	;
	inputs[now_scan]=P2;//P2;
	OE=0;
	P2=temp;//恢复现场
	is_overproof(now_scan);
	now_scan++;
	scan_delay=0;
	if(now_scan==8) now_scan=0;
	st=0;
}