#include "show.h"
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "main.h"

void qidong_view()//启动界面，按下choice按键
{
	OLED_Clear();
	OLED_ShowCHinese(32,0,0);OLED_ShowCHinese(48,0,1);OLED_ShowCHinese(64,0,2);OLED_ShowCHinese(80,0,3);//启动界面
	OLED_ShowString(12,3,"press--choice",16);
  OLED_ShowString(40,5,"start!!",16);
}
void moshi_view()//OLED显示的参数界面，用来显示测量的输出电压参数
{
	OLED_Clear();
	//模式选择
	OLED_ShowCHinese(32,0,4);OLED_ShowCHinese(48,0,5);OLED_ShowCHinese(64,0,6);OLED_ShowCHinese(80,0,7);
	//放电模式	
  OLED_ShowCHinese(0,3,9);OLED_ShowCHinese(16,3,10);OLED_ShowCHinese(32,3,4);OLED_ShowCHinese(48,3,5);
	//充电模式	
  OLED_ShowCHinese(0,5,8);OLED_ShowCHinese(16,5,10);OLED_ShowCHinese(32,5,4);OLED_ShowCHinese(48,5,5);
}
void hengya_view()//用来显示放电界面
{
	OLED_Clear();
	//放电电压
	OLED_ShowCHinese(0,3,9);OLED_ShowCHinese(16,3,10);OLED_ShowCHinese(32,3,10);OLED_ShowCHinese(48,3,11);
	OLED_ShowString(64,3,":",16);
	OLED_ShowString(88,3,".",16);OLED_ShowString(112,3,"V",16);
}
void hengliu_view()//OLED显示的参数界面，用来显示测量的输出电压参数
{
	OLED_Clear();
	//充电电流
	OLED_ShowCHinese(0,3,8);OLED_ShowCHinese(16,3,10);OLED_ShowCHinese(32,3,10);OLED_ShowCHinese(48,3,12);
	OLED_ShowString(64,3,":",16);
	OLED_ShowString(88,3,".",16);OLED_ShowString(112,3,"A",16);
}
void canshu_view()                                                                                                                                                         //OLED显示的参数界面，用来显示测量的输出电压参数
{
	OLED_Clear();
	//输入输出电压电流	
	OLED_ShowString(0,0,"IV:00.00 II:0.00",16);
	OLED_ShowString(0,2,"OV:00.00 OI:0.00",16);
	//输入功率
	OLED_ShowCHinese(0,4,15);OLED_ShowCHinese(16,4,14);OLED_ShowCHinese(32,4,17);OLED_ShowCHinese(48,4,18);
	OLED_ShowString(64,4,":",16);
	OLED_ShowString(88,4,".",16);OLED_ShowString(120,4,"W",16);
	//输出功率
	OLED_ShowCHinese(0,6,15);OLED_ShowCHinese(16,6,16);OLED_ShowCHinese(32,6,17);OLED_ShowCHinese(48,6,18);
	OLED_ShowString(64,6,":",16);
	OLED_ShowString(88,6,".",16);OLED_ShowString(120,6,"W",16);
}



