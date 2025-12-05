#include "FMQ.h"
#include "stm32f10x.h"  
/*
    版 权 所 有：嵌入式Dora
		
		需定制STM32加QQ：2781384621
		主 控 芯  片：STM32F103C8T6
		传  感  器 ：SY906红外测温传感器
*/
void mfq_Init(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GY906_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GY906_PORT, &GPIO_InitStructure);
	
}

void fmq(float Temp_Value)
{
		if(Temp_Value > 20)
		{
			GPIO_ResetBits(GY906_PORT, GY906_PIN);
    }
		else
		{
			GPIO_SetBits(GY906_PORT, GY906_PIN);
		}

}
