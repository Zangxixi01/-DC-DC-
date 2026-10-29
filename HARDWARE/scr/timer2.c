#include "adc.h"
#include "main.h"         
#include "delay.h"        
#include "timer.h"    
#include "timer2.h"
#include "stm32f10x.h"    
#include "stm32f10x_it.h" 

void TIM2_ENABLE(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);           //使能TIM2时钟	
	TIM_DeInit(TIM2);                                             //定时器2寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 10-1; 	          //设置自动重装载值60000-1
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);            //设置TIM2
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                      //使能TIM2溢出中断    
	TIM_Cmd(TIM2,ENABLE);                                         //开TIM2                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;                 //设置TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}
extern __IO uint16_t ADC_ConvertedValue[4];
float ADC_ConvertedValueLocal; 
float ADC_ConvertedValueLoca2;
float ADC_ConvertedValueLoca3; 
float ADC_ConvertedValueLoca4;

void TIM2_IRQHandler(void)
{	
	static u16 adc_count=0;

	static float dcinv_out=0,dcini_out=0;
	static float dcoutv_out=0,dcouti_out=0;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		adc_count++;
		//电压电流检测
		dcinv_out+=(float) ADC_ConvertedValue[0]/4096*3.277; 
		dcini_out+=(float) ADC_ConvertedValue[1]/4096*3.277; 
		dcoutv_out+=(float) ADC_ConvertedValue[2]/4096*3.277; 
		dcouti_out+=(float) ADC_ConvertedValue[3]/4096*3.277; 
		if(adc_count>=200)
		{
			adc_count=0;
			DCIN_V=dcinv_out/200*VDCIN_Ratio; 	
			DCIN_I=dcini_out/200; 
      if(DCIN_I>=2.50f)DCIN_I=DCIN_I-2.50f;
      else DCIN_I=2.50f-DCIN_I;		
      DCIN_I=DCIN_I*IDCIN_Ratio;
			
			DCOUT_V=dcoutv_out/200*VDCOUT_Ratio; 	
			DCOUT_I=dcouti_out/200; 	
      if(DCOUT_I>=2.50f)DCOUT_I=DCOUT_I-2.50f;
      else DCOUT_I=2.50f-DCOUT_I;		
      DCOUT_I=DCOUT_I*IDCOUT_Ratio;		
			
			DCIN_P=DCIN_V*DCIN_I;
			DCOUT_P=DCOUT_V*DCOUT_I;
			dcinv_out=0;dcini_out=0;dcoutv_out=0;dcouti_out=0;
		}
				
		if(work_mode==1&&protect_status==0)
		{
			if(work_status==0)//电压环路
			{
//				if(ADC_ConvertedValue[0]<((1.0524*Target_V-0.5676)/3.3*4096/V_xishu))buck_pwm+=1;
//				else if(ADC_ConvertedValue[0]>((1.0524*Target_V-0.5676)/3.3*4096/V_xishu))buck_pwm-=1;
//				else buck_pwm=buck_pwm;
				if(ADC_ConvertedValue[0]<(Target_V/VDCIN_Ratio/3.277*4096))
				{
					if(dc_pwm<3599)dc_pwm+=1;
					else dc_pwm=3599;
				}
				else if(ADC_ConvertedValue[0]>(Target_V/VDCIN_Ratio/3.277*4096))
				{
					if(dc_pwm>=1)dc_pwm-=1;
					else dc_pwm=0;
				}
				else dc_pwm=dc_pwm;				
				set_pwm(TIM1,1,dc_pwm,3599);						
      }
			else if(work_status==1)//电流环路
			{
//				if(ADC_ConvertedValue[0]<((1.0524*Target_V-0.5676)/3.3*4096/V_xishu))buck_pwm+=1;
//				else if(ADC_ConvertedValue[0]>((1.0524*Target_V-0.5676)/3.3*4096/V_xishu))buck_pwm-=1;
//				else buck_pwm=buck_pwm;
				if(ADC_ConvertedValue[3]<(3124+Target_I/IDCOUT_Ratio/3.277*4096))				
				{
					if(dc_pwm<3599)dc_pwm+=1;
					else dc_pwm=3599;
				}
				else if(ADC_ConvertedValue[3]>(3124+Target_I/IDCOUT_Ratio/3.277*4096))
				{
					if(dc_pwm>=1)dc_pwm-=1;
					else dc_pwm=0;
				}
				else dc_pwm=dc_pwm;
				set_pwm(TIM1,1,dc_pwm,3599);								
			}
		}
		else if(work_mode==0&&protect_status==0)
		{
			dc_pwm=0;
			set_pwm(TIM1,1,dc_pwm,1000);		
		}
		else if(protect_status==1)
		{
			dc_pwm=0;
			set_pwm(TIM1,1,dc_pwm,1000);		
		}
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);          //清除TIM2溢出中断标志 	
}

