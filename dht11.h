/******************************************************************************
* 文    件 :dht11.h
* 描    述 :DHT11温湿度传感器函数及变量声明
* 日    期 :2020/8/10
* 版    本 :v0.1
* 平    台 :STC8A8K64S4A12@24MHz
* 作    者 :lengyuefenghua
******************************************************************************/

#ifndef _DHT11_H_
#define _DHT11_H_

#include "stc8.h"

/**DHT11数据端口声明*/
sbit DHT11_Pin = P4^1;

extern unsigned int DHT11_Data;

unsigned int DHT11_ReadData();

#endif