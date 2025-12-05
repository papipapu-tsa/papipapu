#ifndef __FMQ_H
#define __FMQ_H 
/*
    版 权 所 有：嵌入式Dora
		
		需定制STM32加QQ：2781384621
		主 控 芯  片：STM32F103C8T6
		传  感  器 ：SY906红外测温传感器
*/
#define GY906_PIN GPIO_Pin_0
#define GY906_PORT GPIOB
void mfq_Init(void);
void fmq(float temp_one);

#endif

