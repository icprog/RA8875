#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


				    
 								  
void SPI2_Init(void);			 //��ʼ��SPI1��
void SPI2_SetSpeed(uint8_t SpeedSet); //����SPI1�ٶ�   
uint8_t SPI2_ReadWriteByte(uint8_t TxData);//SPI1���߶�дһ���ֽ�
void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(uint8_t SpeedSet); //����SPI1�ٶ�   
uint8_t SPI1_ReadWriteByte(uint8_t TxData);//SPI1���߶�дһ���ֽ�		 
#endif

