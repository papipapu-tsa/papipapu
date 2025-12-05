#include "GY906.h"
/*
    版 权 所 有：嵌入式Dora
		
		需定制STM32加QQ：2781384621
		主 控 芯  片：STM32F103C8T6
		传  感  器 ：SY906红外测温传感器
*/
void GY906_Init()
{
    GPIO_InitTypeDef    GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GY906_SCL | GY906_SDA;//开漏输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GY906_PORT, &GPIO_InitStructure);

    GY906_SCL_H();
    GY906_SDA_H();
}

void GY906_Start(void)
{
    GY906_SDA_H();		
    GY906_Delay(5);	    
    GY906_SCL_H();	
    GY906_Delay(5);	   
    GY906_SDA_L();		
    GY906_Delay(5);	    
    GY906_SCL_L();	    
    GY906_Delay(5);	    
}

void GY906_Stop(void)
{
    GY906_SCL_L();		
    GY906_Delay(5);	   
    GY906_SDA_L();		
    GY906_Delay(5);	    
    GY906_SCL_H();		
    GY906_Delay(5);	    
    GY906_SDA_H();		
}

u8 GY906_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;

    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)
        {
            bit_out=1;   
        }
        else  
        {
            bit_out=0;  					
        }
        GY906_SendBit(bit_out);	  
        Tx_buffer<<=1;						
    }
     Ack_bit=GY906_ReceiveBit();    
    return	Ack_bit;
}

void GY906_SendBit(u8 bit_out)
{
    if(bit_out==0)
    {
        GY906_SDA_L();
    }
    else
    {
        GY906_SDA_H();
    }
    GY906_Delay(2);					
    GY906_SCL_H();					
    GY906_Delay(6);					
    GY906_SCL_L();					
    GY906_Delay(3);					
    return;
}

u8 GY906_ReceiveBit(void)
{
    u8 Ack_bit;

    GY906_SDA_H();          
		GY906_Delay(2);			
    GY906_SCL_H();			
    GY906_Delay(5);			
    if (GY906_SDA_PIN())
    {
        Ack_bit=1;
    }
    else
    {
        Ack_bit=0;
    }
    GY906_SCL_L();			
    GY906_Delay(3);			

    return	Ack_bit;
}

u8 GY906_ReceiveByte(u8 ack_nack)
{
    u8 	RX_buffer;
    u8	Bit_Counter;

    for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
    {
        if(GY906_ReceiveBit())
        {
            RX_buffer <<= 1;
            RX_buffer |=0x01;
        }
        else
        {
            RX_buffer <<= 1;
            RX_buffer &=0xfe;
        }
    }
    GY906_SendBit(ack_nack);
    return RX_buffer;
}

void GY906_Delay(u16 time)
{
    u16 i, j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<time; j++);
    }
}

u16 GY906_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;			
    u8 Pec;				
    u8 DataL=0;			
    u8 DataH=0;			
    u8 arr[6];															// 存储用于计算PEC的数组
    u8 PecReg;															// 存储计算得到的PEC寄存器值
    u8 ErrorCounter;	

    ErrorCounter=0x00;											// 初始化错误计数器为0
	  slaveAddress <<= 1;	 										// 将从机地址左移一位，以符合I2C协议
	
    do
    {
repeat:
        GY906_Stop();			    
        --ErrorCounter;				    
        if(!ErrorCounter) 			    				// 如果错误计数器为0
        {
            break;					    
        }

        GY906_Start();				
        if(GY906_SendByte(slaveAddress))		// 发送从机地址
        {
            goto	repeat;			    
        }
        if(GY906_SendByte(command))	    		// 发送命令
        {
            goto	repeat;		    	
        }

        GY906_Start();					
        if(GY906_SendByte(slaveAddress+1))	// 发送从机地址（读模式）
        {
            goto	repeat;             	
        }

        DataL = GY906_ReceiveByte(ACK);	 		// 接收低字节数据并发送ACK
        DataH = GY906_ReceiveByte(ACK);  		// 接收高字节数据并发送ACK
        Pec = GY906_ReceiveByte(NACK);			// 接收PEC并发送NACK
        GY906_Stop();				

        arr[5] = slaveAddress;		 					// 存储从机地址
        arr[4] = command;										// 存储命令
        arr[3] = slaveAddress+1;						// 存储从机地址（读模式）
        arr[2] = DataL;											// 存储低字节数据
        arr[1] = DataH;											// 存储高字节数据
        arr[0] = 0;													// 存储0
        PecReg=PEC_Calculation(arr);				// 计算PEC寄存器值
    }
    while(PecReg != Pec);										// 如果计算得到的PEC值与接收到的PEC值不相等，重新读取

	  data = (DataH<<8) | DataL;	 						// 将高字节和低字节数据组合成16位数据
    return data;
}

u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];
    u8	BitPosition=47;
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        BitPosition=47;
        shift=0;

        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }

        shift=BitPosition-8;

        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }
            shift--;
        }

        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }
    }
    while(BitPosition>8); 
    return pec[0];
}

float GY906_GetTemp(void)
{   
	float temp;
	temp = GY906_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;// T_{Celsius} = T_{Kelvin} - 273.15 摄氏度
	return temp;
}


