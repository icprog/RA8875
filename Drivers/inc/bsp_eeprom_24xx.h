/*
*********************************************************************************************************
*
*  ģ������ : ����EEPROM 24xx02����ģ��
*  �ļ����� : bsp_eeprom_24xx.h
*  ��    �� : V1.0
*  ˵    �� : ͷ�ļ�
*
*  �޸ļ�¼ :
*    �汾��  ����       ����    ˵��
*    v1.0    2012-10-12 armfly  ST�̼���汾 V2.1.0
*
*  Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_EEPROM_24XX_H
#define _BSP_EEPROM_24XX_H
#include "bsp_i2c_gpio.h"
//#define AT24C02
#define AT24C128

#ifdef AT24C02
  #define EE_MODEL_NAME    "AT24C02"
  #define EE_DEV_ADDR      0xA0    /* �豸��ַ */
  #define EE_PAGE_SIZE    8      /* ҳ���С(�ֽ�) */
  #define EE_SIZE        256      /* ������(�ֽ�) */
  #define EE_ADDR_BYTES    1      /* ��ַ�ֽڸ��� */
#endif

#ifdef AT24C128
  #define EE_MODEL_NAME    "AT24C128"
  #define EE_DEV_ADDR      0xA0    /* �豸��ַ */
  #define EE_PAGE_SIZE    64      /* ҳ���С(�ֽ�) */
  #define EE_SIZE        (16*1024)  /* ������(�ֽ�) */
  #define EE_ADDR_BYTES    2      /* ��ַ�ֽڸ��� */
#endif

uint8_t FM_CheckOk(void);
uint8_t FMReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t FM24C64_READ_BYTE (uint16_t addr);
void FM24C64_READ_MUL(uint16_t StartAddr, uint8_t *RdData, uint16_t Numbers);
uint8_t FM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void FM24C64_WRITE_BYTE (uint16_t addr,uint8_t data);
void FM24C64_WRITE_MUL(uint16_t StartAddr, uint8_t *RdData, uint16_t Numbers);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
