/******************************************************************************
* 文    件 :dht11.c
* 描    述 :读取DHT11温湿度传感器
* 日    期 :2020/8/10
* 版    本 :v0.1
* 平    台 :STC8A8K64S4A12@24MHz
* 作    者 :lengyuefenghua
******************************************************************************/

#include "dht11.h"
#include "intrins.h"

/**DHT11温湿度数据*/
unsigned int DHT11_Data; 

/******************************************************************************
* 描  述 :DHT11起始信号延时函数
* 入  参 :无
* 返回值 :无
******************************************************************************/
void DHT11_Delay22ms()		//@24.000MHz
{
	unsigned char i, j, k;
	i = 3;
	j = 174;
	k = 178;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
/******************************************************************************
* 描  述 :DHT1响应延时及数据位判断延时函数
* 入  参 :无
* 返回值 :无
******************************************************************************/
void DHT11_Delay50us()		//@24.000MHz
{
	unsigned char i, j;
	_nop_();
	i = 2;
	j = 140;
	do
	{
		while (--j);
	} while (--i);
}
/******************************************************************************
* 描  述 :DHT1初始化函数
* 入  参 :无
* 返回值 :1 初始化成功(DHT11存在); 0 初始化失败（DHT11不存在或者开始信号时序错误） 
******************************************************************************/
bit DHT11_Init()
{
    DHT11_Pin = 1;
    DHT11_Pin = 0;      //拉低数据线
    DHT11_Delay22ms();  //延时>18ms
    DHT11_Pin = 1;      //拉高数据线
    DHT11_Delay50us();  //等待DHT11响应
    if(!DHT11_Pin)      //DHT11响应
    {
        while(!DHT11_Pin);  //跳过响应低电平
        while(DHT11_Pin);   //跳过响应高电平
        return 1;           //开始传输数据
    }
    return 0;
}
/******************************************************************************
* 描  述 :DHT11字节数据读取函数
* 入  参 :无
* 返回值 :一字节DHT11数据
******************************************************************************/
unsigned char DHT11_ReadByte()
{
    unsigned char i;
    unsigned char DHT11_Dat;
    for ( i = 0; i < 8; i++)        //循环8 次，读取一字节数据
    {
        while(!DHT11_Pin);          //跳过数据开始的50us低电平
        DHT11_Delay50us();          //跳过数据‘0’的高电平，
        DHT11_Dat <<= 1;            //准备缓存数据
        DHT11_Dat |= DHT11_Pin;     // 数据线为高电平 则是‘1’，数据线为低电平，则是‘0’
        while(DHT11_Pin);           //准备下次接受
    }
    return DHT11_Dat;
}
/******************************************************************************
* 描  述 :DHT11数据读取函数
* 入  参 :无
* 返回值 :DHT11温湿度数据，仅保留整数部分，高八位为湿度数据，低八位为温度数据，使用
         方式为：湿度数据 = （返回值）>> 8,温度数据 = （返回值）& 0x00FF.
******************************************************************************/
unsigned int DHT11_ReadData()
{
    unsigned char DHT11_RecData[5] = 0;
    if(DHT11_Init())
    {
        DHT11_RecData[0] = DHT11_ReadByte();    //读取湿度数据高八位
        DHT11_RecData[1] = DHT11_ReadByte();    //读取湿度数据低八位
        DHT11_RecData[2] = DHT11_ReadByte();    //读取温度数据高八位
        DHT11_RecData[3] = DHT11_ReadByte();    //读取温度数据低八位
        DHT11_RecData[4] = DHT11_ReadByte();    //读取校验和
        if(DHT11_RecData[4] == DHT11_RecData[0] + DHT11_RecData[1] + DHT11_RecData[2] + DHT11_RecData[3])  //数据校验
        {
            DHT11_Data = (DHT11_RecData[0] << 8) | DHT11_RecData[2];
            return DHT11_Data;
        }
        else
        {
            DHT11_Data = 0;
            return 0;
        }
    }
    return 0;
}