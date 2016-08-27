/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : RTC�ײ�����
*	
*
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H
#define __BSP_RTC_H
#include "stm32f4xx.h"
#include "unixtime.h"
extern TIME_UNIX TimeNow;
void bsp_InitRTC(void);
void RTC_Config(void);
void GetTimeNow(void);
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

