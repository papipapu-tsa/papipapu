#ifndef __GY906_H
#define __GY906_H
#include "sys.h"
/*
    版 权 所 有：嵌入式Dora
		
		需定制STM32加QQ：2781384621
		主 控 芯  片：STM32F103C8T6
		传  感  器 ：SY906红外测温传感器
*/
#define ACK	 0
#define	NACK 1  //不应答或否定的应答
#define SA				0x00 //从机地址，单个GY906时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command
#define EEPROM_ACCESS	0x20 //EEPROM access command
#define RAM_TOBJ1		0x07 //To1 address in the eeprom

#define GY906_PORT	    GPIOB
#define GY906_SCL		GPIO_Pin_11
#define GY906_SDA		GPIO_Pin_10

#define RCC_APB2Periph_SMBUS_PORT		RCC_APB2Periph_GPIOB

#define GY906_SCL_H()	    GY906_PORT->BSRR = GY906_SCL
#define GY906_SCL_L()	    GY906_PORT->BRR = GY906_SCL
#define GY906_SDA_H()	    GY906_PORT->BSRR = GY906_SDA
#define GY906_SDA_L()	    GY906_PORT->BRR = GY906_SDA

#define GY906_SDA_PIN()	    GY906_PORT->IDR & GY906_SDA //读取引脚电平


void GY906_Start(void);
void GY906_Stop(void);
void GY906_SendBit(u8);
u8 GY906_SendByte(u8);
u8 GY906_ReceiveBit(void);
u8 GY906_ReceiveByte(u8);
void GY906_Delay(u16);
void GY906_Init(void);
u16 GY906_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float GY906_GetTemp(void);    //获取温度值
#endif
