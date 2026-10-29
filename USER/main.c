#include "stm32f10x.h" 
#include "main.h"      
#include "delay.h"        
#include "timer.h"     
#include "timer2.h"    
#include "timer3.h"    
#include "timer4.h"       
#include "oled.h"      
#include "show.h"      
#include "key.h"       
#include "adc.h"            
#include "sw.h"
#include "sd.h"
#include "key.h" 
u8 show_mode=1;        //对应不同的界面和模式
u8 work_mode=0;        //对应工作状态是否开启
u8 work_status=0;      //=0为电池充电状态，=1为电池放电状态
u8 protect_status=0;   //继电器异常保护状态
u16 dc_pwm=0;          //DC电路的PWM

float DCIN_V=0.0f;     //输入电压数据
float DCIN_I=0.0f;     //输入电流数据
float DCIN_P=0.0f;     //输入功率

float DCOUT_V=0.0f;    //输出电压数据
float DCOUT_I=0.0f;    //输出电流数据
float DCOUT_P=0.0f;    //输出功率

float Target_I=0.50f;  //追踪的充电电流数据
float Target_V=30.00f; //追踪的放电电压数据

float V_yuzhi=38.00f;  //保护的电压阈值
float I_yuzhi=2.00f;   //保护的电流阈值
/**************放电模式*************** 
DCIN_OFF;
DCOUT_ON;
****************End******************/

/**************充电模式*************** 
DCIN_ON;
DCOUT_OFF;
*****************End*****************/

/**************关闭模式*************** 
DCIN_OFF;
DCOUT_OFF;
*****************End*****************/

/*************************引脚配置***************************
电压电流检测:输入电压PA0，输入电流PA1，输出电压PA2，输出电流PA3
BUCK电路PWM控制引脚:PA8
BUCK电路使能控制引脚:PA9
OLED屏幕:SCL:PB1,SDA:PB0
输入继电器控制位:PA6 输出继电器控制位:PA5
SET按键:PB12 CHOICE按键:PB13 REDUCE按键:PB14 ADD按键PB15
************************************************************/
int main(void) 
{
  u8 key;	   
	Delay_Init();                   //延时功能初始化           
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	
	
  SD_Init();                      //芯片使能引脚初始化
	SD_OFF;                         //关闭芯片引脚输出
	
	SW_Init( );                     //输入输出继电器初始化
	DCIN_OFF;                       //关闭输入继电器，配置为电阻模式
	DCOUT_OFF;                      //关闭输出继电器，配置为反向二极管模式

	KEY_Init();                     //按键初始化
	Init_adc();                     //ADC初始化	 	
	OLED_Init();			              //初始化OLED  
  TIM2_ENABLE();                  //执行各类函数，5ms执行一次 	
	 
	loop:	
	dc_pwm=0;
	show_mode=1;                    //切换到第一个界面进行显示
	work_mode=0;                    //关闭工作状态，不执行闭环追踪
	SD_OFF;                         //关闭IR2104S半桥驱动芯片芯片引脚输出	
	delay_ms(200);
	DCIN_OFF;                       //关闭输入继电器，配置为电阻模式
	delay_ms(200);
	DCOUT_OFF;                      //关闭输出继电器，配置为反向二极管模式
	delay_ms(200);
	BOOST_PWM_Init();               //Boost电路初始化，20KHz工作频率，初始化放电状态保证安全	

	qidong_view();                  //启动界面	
	while(1)
	{	
		key=KEY_Scan(0);              //扫描按键
		if(key==SET_PRES)
		{
      show_mode=2;			          			
			break;
		}
		delay_ms(50);
	}	
	
	moshi_view();                   //模式选择界面
	while(1)
	{	
		//工作在模式选择界面的时候
		if(show_mode==2)
		{
			if(work_status==0)
			{
				OLED_ShowString(112,3,"<-",16);
				OLED_ShowString(112,5,"  ",16);
			}
			else
			{
				OLED_ShowString(112,3,"  ",16);
				OLED_ShowString(112,5,"<-",16);				
			}
			
			key=KEY_Scan(0);//扫描按键	
			if(key==SET_PRES && protect_status==0)
			{
				show_mode=3;
				if(work_status==0)
				{
					hengya_view();   //放电升压模式
					BOOST_PWM_Init();//Boost电路初始化，20KHz工作频率，初始化放电状态保证安全
				}
				else 
				{
					hengliu_view();  //恒流放电模式
					BUCK_PWM_Init(); //Bcuk电路初始化，20KHz工作频率，初始化放电状态保证安全
				}
			}
			else if(key==CHOICE_PRES)
			{
				work_status=!work_status;
			}
		}
		//电压或者电流调节
		else if(show_mode==3)
		{
			if(work_status==0)
			{
				OLED_ShowNum(72,3,Target_V,2,16);
				OLED_ShowNum(96,3,(u16)(Target_V*100)%100,2,16);
			}	
      else if(work_status==1)
			{
				OLED_ShowNum(72,3,Target_I,2,16);
				OLED_ShowNum(96,3,(u16)(Target_I*100)%100,2,16);				
			}
			
			key=KEY_Scan(0);
			if(key==SET_PRES)
			{
        if(work_status==0)//放电模式
				{
	        DCIN_OFF;                       //关闭输入继电器，配置为电阻模式	
          delay_ms(200);					
	        DCOUT_ON;                       //打开输出继电器，配置为直连模式	
          delay_ms(200);										
	        SD_ON;                          //打开芯片引脚输出					
				}
				else if(work_status==1)//充电模式
				{
					DCIN_ON;
          delay_ms(200);					
			    DCOUT_OFF;
          delay_ms(200);
	        SD_ON;                         //打开芯片引脚输出						
				}
				show_mode=4;
				work_mode=1;
			  protect_status=0;
				canshu_view();
			}
			else if(key==ADD_PRES)
			{
		    if(work_status==0)Target_V+=0.10001f;
		    else Target_I+=0.10001f;	
			}
			else if(key==REDUCE_PRES)
			{
		    if(work_status==0)Target_V-=0.099999f;
		    else Target_I-=0.099999f;					
			}
		}
		else if(show_mode==4)
		{
			key=KEY_Scan(0);
			OLED_ShowNum(24,0,DCIN_V,2,16);
			OLED_ShowNum(48,0,(u16)(DCIN_V*100)%100,2,16);	
			OLED_ShowNum(96,0,DCIN_I,1,16);
			OLED_ShowNum(112,0,(u16)(DCIN_I*100)%100,2,16);	
			OLED_ShowNum(24,2,DCOUT_V,2,16);
			OLED_ShowNum(48,2,(u16)(DCOUT_V*100)%100,2,16);	
			OLED_ShowNum(96,2,DCOUT_I,1,16);
			OLED_ShowNum(112,2,(u16)(DCOUT_I*100)%100,2,16);	
			OLED_ShowNum(72,4,DCIN_P,2,16);
			OLED_ShowNum(96,4,(u16)(DCIN_P*100)%100,2,16);	
			OLED_ShowNum(72,6,DCOUT_P,2,16);
			OLED_ShowNum(96,6,(u16)(DCOUT_P*100)%100,2,16);				
      if(key==SET_PRES)                                                                                                                                                                                                        
			{
				work_mode=0;//失能PID工作状态
				work_status=0;//PID工作状态复位为放电
				goto loop;
			}		
			
      if(work_status==0&&DCIN_V>=V_yuzhi)
		  {
			  protect_status=1;
			  work_mode=0;//失能PID
			  work_status=0;//PID工作状态复位为放电
			  goto loop;
		  }		

      if(work_status==1&&DCOUT_I>=I_yuzhi)
		  {
			  protect_status=1;
			  work_mode=0;//失能PID
			  work_status=0;//PID工作状态复位为放电
			  goto loop;
		  }					
		}
	}
}

