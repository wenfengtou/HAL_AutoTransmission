#include "hx711.h"

long HX711_Buffer = 0;
long Weight_Maopi = 0,Weight_Shiwu = 0;

#define GapValue 107  /*430*/
#define HIGH 1
#define LOW  0
#define HX711_GROUP GPIOB
#define HX711_DT   GPIO_PIN_3
#define HX711_SCK  GPIO_PIN_4
//****************************************************
//初始化HX711
//****************************************************
void delay_us(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=10;  //自己定义
      while(i--) ;    
   }
}

void delay_ms(uint16_t time)
{    
   uint16_t i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}


//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();		
} 



//****************************************************
//读取HX711,对第一个交互力传感器
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i; 	
	
  HAL_GPIO_WritePin(HX711_GROUP, HX711_SCK, GPIO_PIN_RESET);//模块没准备好时，单片机在CLK线上输出低电平
  count=0;
	int ii=0;
    while(1)
	{
	    if(HAL_GPIO_ReadPin(HX711_GROUP,HX711_DT)==0)
		{
			delay_us(1);
			if(HAL_GPIO_ReadPin(HX711_GROUP,HX711_DT)==0)
			{
				break;
			}			
		}
			
		ii++;
		if(ii>=1000000)
		{
			break;
		}
	}
	delay_us(1);
  	for(i=0;i<24;i++)
	{ 			
		HAL_GPIO_WritePin(HX711_GROUP,HX711_SCK,GPIO_PIN_SET);
		delay_us(1);
	  	count=count<<1; 	
		HAL_GPIO_WritePin(HX711_GROUP,HX711_SCK,GPIO_PIN_RESET);		
	  	if(HAL_GPIO_ReadPin(HX711_GROUP,HX711_DT)==1)//读入数据
		{
			count++; 
		}		
			
	} 
	HAL_GPIO_WritePin(HX711_GROUP,HX711_SCK,GPIO_PIN_SET);
	delay_us(2);
  count=count^0x800000;//第25个脉冲下降沿来时，转换数据
	HAL_GPIO_WritePin(HX711_GROUP,HX711_SCK,GPIO_PIN_RESET);
	delay_us(1);
	HAL_GPIO_WritePin(HX711_GROUP,HX711_SCK,GPIO_PIN_SET);
	delay_us(1);
	HAL_GPIO_WritePin(HX711_GROUP,HX711_SCK,GPIO_PIN_RESET);
	return(count);
}


//****************************************************
//称重
//****************************************************
long Get_Weight()
{
	HX711_Buffer = HX711_Read();
	Weight_Shiwu = HX711_Buffer;
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	Weight_Shiwu = (long)((float)Weight_Shiwu/GapValue); 	
	return Weight_Shiwu;
}
