#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "OELD_Data.h"
#include "FMQ.h"
#include "Serial.h"
#include "GY906.h" 
/*
    版 权 所 有：嵌入式Dora
		
		需定制STM32加QQ：2781384621
		主 控 芯  片：STM32F103C8T6
		传  感  器 ：SY906红外测温传感器
*/
float Temp_Value = 0;

int main(void)
{
		GY906_Init();
		OLED_Init();
		mfq_Init();
		Serial_Init();
	
	  OLED_ShowChinese(20, 0, "嵌入式");
		OLED_ShowString(70,0,"Dora",OLED_8X16);
		OLED_ShowChinese(96, 32, "℃");
	  OLED_ShowChinese(0, 32, "温度：");
		OLED_Update();

		while(1)
		{
			Temp_Value = GY906_GetTemp();
			
			OLED_ShowFloatNum(42,32,Temp_Value,2,2,OLED_8X16);
			
			OLED_Update();
			fmq(Temp_Value);
			printf("温度 = %.2f",Temp_Value);
			Serial_SendString(" ℃\n");
		}
}
