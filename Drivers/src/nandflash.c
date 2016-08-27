

#include "nandflash.h"

#define K9F2G08_delay_times       5000000L

/*����NAND Flash�������ַ���������Ӳ��������*/
#define Bank2_NAND_ADDR           ((WINT32U_F)0x70000000)
#define Bank_NAND_ADDR            Bank2_NAND_ADDR

/* NAND Area definition  for STM3210E-EVAL Board RevD */
#define CMD_AREA                  (WINT32U_F)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                 (WINT32U_F)(1<<17)  /* A17 = ALE high */
#define DATA_AREA                 ((WINT32U_F)0x00000000)

#define FLASH_DATA		  (Bank_NAND_ADDR)
#define FLASH_COMMAND             (Bank_NAND_ADDR|CMD_AREA)
#define FLASH_ADDRESS             (Bank_NAND_ADDR|ADDR_AREA)


#define W_Flash(data)             *(volatile unsigned char*)FLASH_DATA=(data);      /* ��flash��д����*/
#define R_Flash()                 *(volatile unsigned char*)FLASH_DATA;             /* ��flash�ж�ȡ����*/
#define W_Flash_Comand(comand)    *(volatile unsigned char*)FLASH_COMMAND=(comand); /* ��flash��д����*/
#define W_Flash_Address(address)  *(volatile unsigned char*)FLASH_ADDRESS=(address);/* ��flash��д��ַ*/

#define K9F2G08_IS_READY(uch)           (uch&0x40)!=0
#define K9F2G08_IS_ERROR(uch)           (uch&0x01)!=0
#define K9F2G08_IS_ECC_ERROR(uch)       (uch&0x07)!=4

/*���غ�������*/
static void FlashFSMCcInit(void);
static WINT8U_F FlshSectorWrite0(WINT16U_F block,WINT16U_F sector,WINT16U_F offset,WINT16U_F size,const WINT8U_F *data_buf);
static WINT8U_F K9F2G08_sector_check(WINT16U_F block,WINT16U_F sector,WINT16U_F offset,WINT16U_F size,const WINT8U_F *data_buf);


/*
********************************************************************************
* �� �� ��: MemSetFun
* ����˵��: ���������е����ݲ���Ϊָ����ֵ,������������������;
* ��    ��: ��
* �� �� ֵ: ��
********************************************************************************
*/
//static void MemSetFun(void* buf,WINT8U_F byte,WINT32U_F count)
//{
//  WINT8U_F* pbuf;
//  
//  if((pbuf=(WINT8U_F*)buf)==(WINT8U_F*)0) return;
//  for(WINT32U_F i=0;i<count;i++) pbuf[i]=byte;
//}
/*
********************************************************************************
* �� �� ��: NANDInit
* ����˵��: Flash��ʼ��;
* ��    ��: ��
* �� �� ֵ: ��
********************************************************************************
*/
void NANDFlashInit(void)
{
  FlashFSMCcInit();          /* ����FSMC��GPIO����NAND Flash�ӿ� */
}
/*
********************************************************************************
* �� �� ��: FlashFSMCcInit
* ����˵��: FSMS\GPIO��ʼ��;
* ��    ��: ��
* �� �� ֵ: ��
********************************************************************************
*/
static void FlashFSMCcInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  FSMC_NANDInitTypeDef  FSMC_NANDInitStructure;
  FSMC_NAND_PCCARDTimingInitTypeDef  p;

/*
******************************************************************************
  --NAND Flash GPIOs ����  ------
  
    PD0/FSMC_D2
    PD1/FSMC_D3
    PD4/FSMC_NOE
    PD5/FSMC_NWE
    PD7/FSMC_NCE2
    PD11/FSMC_A16
    PD12/FSMC_A17
    
    PD14/FSMC_D0
    PD15/FSMC_D1
    PE7/FSMC_D4
    PE8/FSMC_D5
    PE9/FSMC_D6
    PE10/FSMC_D7
    PG6/FSMC_INT2  (�������ò�ѯ��ʽ��æ���˿�����Ϊ��ͨGPIO���빦��ʹ��)
******************************************************************************
*/

  /* ʹ�� GPIO ʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);

  /* ʹ�� FSMC ʱ�� */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  /*  ����GPIOD */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7,  GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_7 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /*  ����GPIOE */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /*  ����GPIOG, PG6��Ϊæ��Ϣ������Ϊ���� */

  /* INT2 ��������Ϊ�ڲ��������룬����æ�ź� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* ���� FSMC ʱ�� */
  /*
    Defines the number of HCLK cycles to setup address before the command assertion for NAND-Flash
    read or write access  to common/Attribute or I/O memory space (depending on the memory space
    timing to be configured).This parameter can be a value between 0 and 0xFF.
  */
    //p.FSMC_SetupTime = 0x01;
    p.FSMC_SetupTime = 0x1;

  /*
    Defines the minimum number of HCLK cycles to assert the command for NAND-Flash read or write
    access to common/Attribute or I/O memory space (depending on the memory space timing to be
    configured). This parameter can be a number between 0x00 and 0xFF
  */
  //p.FSMC_WaitSetupTime = 0x03;
  p.FSMC_WaitSetupTime = 0x3;

  /*
    Defines the number of HCLK clock cycles to hold address (and data for write access) after the
    command deassertion for NAND-Flash read or write access to common/Attribute or I/O memory space
     (depending on the memory space timing to be configured).
    This parameter can be a number between 0x00 and 0xFF
  */
  //p.FSMC_HoldSetupTime = 0x02;
  p.FSMC_HoldSetupTime = 0x2;

  /*
    Defines the number of HCLK clock cycles during which the databus is kept in HiZ after the start
    of a NAND-Flash  write access to common/Attribute or I/O memory space (depending on the memory
    space timing to be configured). This parameter can be a number between 0x00 and 0xFF
  */
  //p.FSMC_HiZSetupTime = 0x01;
  p.FSMC_HiZSetupTime = 0x1;


  FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;                     /* ����FSMC NAND BANK �� */
  FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;     /* ����ȴ�ʱ��ʹ�� */
  FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;  /* ���ݿ�� 8bit */
  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;                      /* ECC������;�������ʹ�� */
  FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;   /* ECC ҳ���С */
  FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x01;                       /* CLE�ͺ�RE��֮����ӳ٣�HCLK������ */
  FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x01;                        /* ALE�ͺ�RE��֮����ӳ٣�HCLK������ */
  FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;               /* FSMC Common Space Timing */
  FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;            /* FSMC Attribute Space Timing */

  FSMC_NANDInit(&FSMC_NANDInitStructure);

  /* FSMC NAND Bank ʹ�� */
  FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}
/*
********************************************************************************
* �� �� ��: FlashGetId
* ����˵��: ��ȡFlash IDֵ;
* ��    ��: blocks_per_dev-�洢���ܿ���;
*           sectors_per_block-ÿ���������;
*           bytes_per_sector-ÿ���������ֽ���;
* �� �� ֵ: 0-�ɹ�;
*           1-ʧ��;
********************************************************************************
*/
WINT8U_F FlashGetId(WINT16U_F *blocks_per_dev,WINT16U_F *sectors_per_block,WINT16U_F *bytes_per_sector)
{
  WINT8U_F info[8]={0};
  WINT8U_F i;
  
  W_Flash_Comand(0x90);
  W_Flash_Address(0x00);
  for(i=0;i<5;i++)
  {
    info[i]=R_Flash();
  }
  if (info[0]==0xAD&&info[4]==0xAD)
  {
//    if (blocks_per_dev) *blocks_per_dev=1024;
    if (blocks_per_dev) *blocks_per_dev=512;
    if (sectors_per_block) *sectors_per_block=64;
    if (bytes_per_sector) *bytes_per_sector=2048;
    return(0);/*�ɹ�*/
  }
  return(1);/*ʧ��*/
}
/*
********************************************************************************
* �� �� ��: FlashSectorRead
* ����˵��: ��ȡһ������ָ���ֽ���;
* ��    ��: block-���;
*           sector-������;
*           offset-����ƫ��;
*           size-��ȡ�ֽ���;
*           data_buf-���ݴ洢������,��ʼλ��;
* �� �� ֵ: 0-�ɹ�;
*           1-ʧ��;
********************************************************************************
*/
WINT8U_F FlashSecRead(WINT16U_F block,WINT16U_F sector,WINT16U_F offset,WINT16U_F size,WINT8U_F *data_buf)
{
  
  WINT32U_F i;
  WINT8U_F uch;

  W_Flash_Comand(NAND_CMD_AREA_A);
  W_Flash_Address((WINT8U_F)(offset));
  W_Flash_Address((WINT8U_F)(offset>>8));
  
  uch=(WINT8U_F)((block<<6)&0x00FF);
  uch|=(WINT8U_F)(sector&0x003F);
  W_Flash_Address(uch);
  
  uch=(WINT8U_F)((block>>2)&0x00FF);
  W_Flash_Address(uch);
  
  uch=(WINT8U_F)((block>>10)&0x00FF);
  W_Flash_Address(uch);
  W_Flash_Comand(NAND_CMD_AREA_TRUE1);
  
  /*�ȴ�ִ�����*/
  for (i = 0; i < 20; i++);
  i=500000;
  while((GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0)&&(i--!=0));
  if(i==0) return 1;
  
  /* �����ݵ�������pBuffer */
  for(i=0;i<size;i++)
  {
    data_buf[i]=R_Flash();
  }  
  return(0);  
}
/*
********************************************************************************
* �� �� ��: FlshSectorWrite
* ����˵��: block-���;
*           sector-������;
*           offset-����ƫ��;
*           size-��ȡ�ֽ���;
*           data_buf-��д�����ݴ洢������,��ʼλ��;
* ��    ��: ��
* �� �� ֵ: 0-�ɹ�;
*           1-ʧ��;
********************************************************************************
*/
static WINT8U_F FlshSectorWrite0(WINT16U_F block,WINT16U_F sector,WINT16U_F offset,WINT16U_F size,const WINT8U_F *data_buf)
{
  WINT32U_F tmp=0;
  WINT8U_F uch;

  /*��������*/
  W_Flash_Comand(0x80);  

  /*д��ַ*/
  W_Flash_Address((WINT8U_F)(offset));  
  W_Flash_Address((WINT8U_F)(offset>>8));  
  uch=(WINT8U_F)((block<<6)&0x00FF);
  uch|=(WINT8U_F)(sector&0x003F);
  W_Flash_Address(uch);
  uch=(WINT8U_F)((block>>2)&0x00FF);
  W_Flash_Address(uch);
  uch=(WINT8U_F)((block>>10)&0x00FF);
  W_Flash_Address(uch);

  /*д����*/
  for (tmp=0;tmp<size;tmp++)
  {
    W_Flash(data_buf[tmp]);
  }

  /*д����*/
  W_Flash_Comand(0x10);
  
  /*�ȴ�ִ�����*/
  while(tmp<K9F2G08_delay_times)
  {
    W_Flash_Comand(0x70);
    uch=R_Flash();
    if (K9F2G08_IS_READY(uch)) break;
    tmp++;
  }
  
  /*����״̬*/
  if (tmp>=K9F2G08_delay_times) return(1);/*��ʱʧ�ܣ�����һֱæ*/
  if (K9F2G08_IS_ERROR(uch)) return(2); /*����ʧ��*/
  return (0);
  
}
/*
********************************************************************************
* �� �� ��: K9F2G08_sector_check
* ��    ��: block-���;
*           sector-������;
*           offset-����ƫ��;
*           size-��ȡ�ֽ���;
*           data_buf-��д�����ݴ洢������,��ʼλ��;
* ����˵��: ���д��������Ƿ���ȷ;
* �� �� ֵ: 0-�ɹ�;
*           1-ʧ��;
********************************************************************************
*/
static WINT8U_F K9F2G08_sector_check(WINT16U_F block,WINT16U_F sector,WINT16U_F offset,WINT16U_F size,const WINT8U_F *data_buf)
{
  WINT32U_F tmp=0;
  WINT8U_F uch;
  
  W_Flash_Comand(0x00);
  
  W_Flash_Address((WINT8U_F)(offset));
  
  W_Flash_Address((WINT8U_F)(offset>>8));
  
  uch=(WINT8U_F)((block<<6)&0x00FF);
  uch|=(WINT8U_F)(sector&0x003F);
  W_Flash_Address(uch);
  
  uch=(WINT8U_F)((block>>2)&0x00FF);
  W_Flash_Address(uch);
  
  uch=(WINT8U_F)((block>>10)&0x00FF);
  W_Flash_Address(uch);
  
  W_Flash_Comand(0x30);
  
  /*�ȴ�ִ�����*/
  while(tmp<K9F2G08_delay_times)
  {
    W_Flash_Comand(0x70);
    uch=R_Flash();
    if (K9F2G08_IS_READY(uch)) break;
    tmp++;
  }
  
  /*����״̬*/
  if (tmp>=K9F2G08_delay_times)
  {
    return(1);/*��ʱʧ�ܣ�����һֱæ*/
  }
  
  /*��������*/
  W_Flash_Comand(0x00);
  for (tmp=0;tmp<size;tmp++)
  {
    uch=R_Flash();
    if (uch!=data_buf[tmp])
    {
      return(2);
    }
  }
  return(0);                            /*�����ɹ�*/
}
/*
********************************************************************************
* �� �� ��: FlashSecWrite
* ��    ��: block-���;
*           sector-������;
*           offset-����ƫ��;
*           size-��ȡ�ֽ���;
*           data_buf-��д�����ݴ洢������,��ʼλ��;
* ����˵��: д������;
* �� �� ֵ: 0-�ɹ�;
*           1-ʧ��;
********************************************************************************
*/
WINT8U_F FlashSecWrite(WINT16U_F block,WINT16U_F sector,WINT16U_F offset,WINT16U_F size,const WINT8U_F *data_buf)
{
  WINT8U_F err=0;
  
  err=FlshSectorWrite0(block,sector,offset,size,data_buf);/*��д��*/
  if(err) return err;
  err=K9F2G08_sector_check(block,sector,offset,size,data_buf);/*��У��*/
  if(err) return err;
  return 0;
}
/*
********************************************************************************
* �� �� ��: K9F2G08_block_erase
* ����˵��: ����һ����;
* ��    ��: block-���
* �� �� ֵ: 0-�ɹ�;
*           1-ʧ��;
********************************************************************************
*/
WINT8U_F FlshBlockErase(WINT16U_F block)
{
  WINT32U_F tmp=0;
  WINT8U_F uch;
  
  
  W_Flash_Comand(0x60);
  
  uch=(WINT8U_F)((block<<6)&0x00FF);
  W_Flash_Address(uch);
  
  uch=(WINT8U_F)((block>>2)&0x00FF);
  W_Flash_Address(uch);
  
  uch=(WINT8U_F)((block>>10)&0x00FF);
  W_Flash_Address(uch);
  
  W_Flash_Comand(0xD0);
  
  /*�ȴ�ִ�����*/
  while(tmp<K9F2G08_delay_times)
  {
    W_Flash_Comand(0x70);
    uch=R_Flash();
    if (K9F2G08_IS_READY(uch)) break;
    tmp++;
  }
  
  /*����״̬*/
  if (tmp>=K9F2G08_delay_times) return(1);/*��ʱʧ�ܣ�����һֱæ*/
  if (K9F2G08_IS_ERROR(uch)) return(2); /*����ʧ��*/
  return(0);/*�����ɹ�*/
}
