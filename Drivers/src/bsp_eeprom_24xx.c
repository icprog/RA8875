/*
*********************************************************************************************************
*
* ģ������ : ����EEPROM 24xx����ģ��
* �ļ����� : bsp_eeprom_24xx.c
* ��    �� : V1.0
* ˵    �� : ʵ��24xxϵ��EEPROM�Ķ�д������д��������ҳдģʽ���д��Ч�ʡ�
*
* �޸ļ�¼ :
* �汾��  ����        ����     ˵��
* V1.0    2013-02-01 armfly  ��ʽ����
*
* ���ǿƼ�-��ҵ�������߼��ϵͳ
*
*********************************************************************************************************
*/

/*
  Ӧ��˵�������ʴ���EEPROMǰ�����ȵ���һ�� bsp_InitI2C()�������ú�I2C��ص�GPIO.
*/

#include "bsp_eeprom_24xx.h"

/*
*********************************************************************************************************
*  �� �� ��: ee_CheckOk
*  ����˵��: �жϴ���EERPOM�Ƿ�����
*  ��    ��:  ��
*  �� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
*********************************************************************************************************
*/
uint8_t FM_CheckOk(void)
{
  if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
  {
    return 1;
  }
  else
  {
    /* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
    i2c_Stop();
    return 0;
  }
}


/*
*********************************************************************************************************
*  �� �� ��: FMReadBytes
*  ����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*  ��    ��:  _usAddress : ��ʼ��ַ
*            _usSize : ���ݳ��ȣ���λΪ�ֽ�
*            _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*  �� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t FMReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
  uint16_t i;

  /* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

  /* ��1��������I2C���������ź� */
  i2c_Start();

  /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
  i2c_SendByte(EE_DEV_ADDR | I2C_WR);  /* �˴���дָ�� */

  /* ��3��������ACK */
  if (i2c_WaitAck() != 0)
  {
    goto cmd_fail;  /* EEPROM������Ӧ�� */
  }

  /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
  if (EE_ADDR_BYTES == 1)
  {
    i2c_SendByte((uint8_t)_usAddress);
    if (i2c_WaitAck() != 0)
    {
      goto cmd_fail;  /* EEPROM������Ӧ�� */
    }
  }
  else
  {
    i2c_SendByte(_usAddress >> 8);
    if (i2c_WaitAck() != 0)
    {
      goto cmd_fail;  /* EEPROM������Ӧ�� */
    }

    i2c_SendByte(_usAddress);
    if (i2c_WaitAck() != 0)
    {
      goto cmd_fail;  /* EEPROM������Ӧ�� */
    }
  }

  /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
  i2c_Start();

  /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
  i2c_SendByte(EE_DEV_ADDR | I2C_RD);  /* �˴��Ƕ�ָ�� */

  /* ��8��������ACK */
  if (i2c_WaitAck() != 0)
  {
    goto cmd_fail;  /* EEPROM������Ӧ�� */
  }

  /* ��9����ѭ����ȡ���� */
  for (i = 0; i < _usSize; i++)
  {
    _pReadBuf[i] = i2c_ReadByte();  /* ��1���ֽ� */

    /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
    if (i != _usSize - 1)
    {
      i2c_Ack();  /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
    }
    else
    {
      i2c_NAck();  /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
    }
  }
  /* ����I2C����ֹͣ�ź� */
  i2c_Stop();
  return 1;  /* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
  /* ����I2C����ֹͣ�ź� */
  i2c_Stop();
  return 0;
}

/*
*** ��ڣ�			addr-��ַ
*** ���ڣ�			����������
*** ����������		��̶���ַ��������
*/
uint8_t FM24C64_READ_BYTE (uint16_t addr)
{ 
  uint8_t SlaveADDR,DATA_R;
  SlaveADDR = (uint8_t)(addr>>8);
      SlaveADDR = (uint8_t)(addr>>8);
	  SlaveADDR = (uint8_t)(addr>>8);
          i2c_Start ();
	  i2c_SendByte (EE_DEV_ADDR | I2C_WR);
	  if (i2c_WaitAck() != 0)
          {
            while(1);
          }
      i2c_SendByte(SlaveADDR);
      if (i2c_WaitAck() != 0)
          {
            while(1);
          }
      i2c_SendByte(addr);
      if (i2c_WaitAck() != 0)
          {
            while(1);
          } 
      i2c_Start();
      i2c_SendByte(EE_DEV_ADDR | I2C_RD);  /* �˴��Ƕ�ָ�� */
      if (i2c_WaitAck() != 0)
        {
          while(1);
        }
      DATA_R=i2c_ReadByte();
      i2c_NAck();
      i2c_Stop();
      return DATA_R;
}

/*
*** ��ڣ�			StartAddr-��ʼ��ַ��RdData-���ݴ�Ż�������
						Numbers-���������ݸ���
*** ���ڣ�			��
*** ����������		�ӹ̶���ַ��������
*/
void FM24C64_READ_MUL(uint16_t StartAddr, uint8_t *RdData, uint16_t Numbers)
{ 
         
      uint8_t SlaveADDR;
	  SlaveADDR=(uint8_t)(StartAddr>>8);
      i2c_Start ();
	 i2c_SendByte (EE_DEV_ADDR | I2C_WR);//Ƭѡ
         if (i2c_WaitAck() != 0)
          {
            while(1);
          }
      i2c_SendByte (SlaveADDR); 
      if (i2c_WaitAck() != 0)
          {
            while(1);
          }
      i2c_SendByte (StartAddr);  //ROM�׵�ַ(0-1FFF)
      if (i2c_WaitAck() != 0)
          {
            while(1);
          }
      i2c_Start ();
      i2c_SendByte (EE_DEV_ADDR | I2C_RD);//��
      if (i2c_WaitAck() != 0)
          {
            while(1);
          }
      while(Numbers>1)
      {   
          *RdData =i2c_ReadByte();
          i2c_Ack();   //Ӧ��
          Numbers--;   
          RdData++;
           
      }    
      *RdData =i2c_ReadByte();
      i2c_NAck();  //��Ӧ��
      i2c_Stop ();
}

/*
*********************************************************************************************************
*  �� �� ��: FM_WriteBytes
*  ����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*  ��    ��:  _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*            _usAddress : ��ʼ��ַ
*            _usSize : ���ݳ��ȣ���λΪ�ֽ�
*           
*  �� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t FM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
  uint16_t i,m;
  uint16_t usAddr;

  /*
    д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
    ����24xx02��page size = 8
    �򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ÿд1���ֽڣ������͵�ַ
    Ϊ���������д��Ч��: ����������page wirte������
  */

  usAddr = _usAddress;
  for (i = 0; i < _usSize; i++)
  {
    /* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
    if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
    {
      /*���ڣ�������ֹͣ�źţ������ڲ�д������*/
      i2c_Stop();

      /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
        CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
      */
      for (m = 0; m < 1000; m++)
      {
        /* ��1��������I2C���������ź� */
        i2c_Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
        i2c_SendByte(EE_DEV_ADDR | I2C_WR);  /* �˴���дָ�� */

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (i2c_WaitAck() == 0)
        {
          break;
        }
      }
      if (m  == 1000)
      {
        goto cmd_fail;  /* EEPROM����д��ʱ */
      }

      /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
      if (EE_ADDR_BYTES == 1)
      {
        i2c_SendByte((uint8_t)usAddr);
        if (i2c_WaitAck() != 0)
        {
          goto cmd_fail;  /* EEPROM������Ӧ�� */
        }
      }
      else
      {
        i2c_SendByte(usAddr >> 8);
        if (i2c_WaitAck() != 0)
        {
          goto cmd_fail;  /* EEPROM������Ӧ�� */
        }

        i2c_SendByte(usAddr);
        if (i2c_WaitAck() != 0)
        {
          goto cmd_fail;  /* EEPROM������Ӧ�� */
        }
      }
    }

    /* ��6������ʼд������ */
    i2c_SendByte(_pWriteBuf[i]);

    /* ��7��������ACK */
    if (i2c_WaitAck() != 0)
    {
      goto cmd_fail;  /* EEPROM������Ӧ�� */
    }

    usAddr++;  /* ��ַ��1 */
  }

  /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
  i2c_Stop();
  return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
  /* ����I2C����ֹͣ�ź� */
  i2c_Stop();
  return 0;
}

/*
*** ��ڣ�			addr-��ַ��data-Ҫд�������
*** ���ڣ�			��
*** ����������		��̶���ַд������
*/
void FM24C64_WRITE_BYTE (uint16_t addr,uint8_t data)
{ 
     uint8_t SlaveADDR;
//        uint8_t i;
	SlaveADDR = (uint8_t)(addr>>8);	
    	i2c_Start ();
	  
	 i2c_SendByte(EE_DEV_ADDR | I2C_WR);
	 
      i2c_SendByte (SlaveADDR); // E2PROM Ƭѡ
//if (i2c_WaitAck() != 0)
//    {
//      while(1);
//    }
      i2c_SendByte ((uint8_t)addr);
//if (i2c_WaitAck() != 0)
//    {
//      while(1);
//    }
      i2c_SendByte (data);
//if (i2c_WaitAck() != 0)
//    {
//      while(1);
//    }
      i2c_Stop ();  
      //���Զο�ʼ
//      if(data == FM24C64_READ_BYTE(addr))
//        i = 0;
//      else
//        i = 1;
//      i = i;
      //���Զν���
}

/*
*** ��ڣ�			addr-��ַ��RdData-Ҫд������ݵ�ַ
						Numbers-д�����ݵĸ���
*** ���ڣ�			��
*** ����������		��̶���ַд��������
*/
void FM24C64_WRITE_MUL(uint16_t StartAddr, uint8_t *RdData, uint16_t Numbers)
{ 
     
      uint8_t SlaveADDR;
      
      SlaveADDR = (uint8_t)(StartAddr>>8);
      i2c_Start ();
      i2c_SendByte(EE_DEV_ADDR | I2C_WR);// E2PROM Ƭѡ
      if (i2c_WaitAck() != 0)
    {
      while(1);
    }
      i2c_SendByte (SlaveADDR); //д��8λ��ַ
      if (i2c_WaitAck() != 0)
    {
     while(1);
    }
      i2c_SendByte (StartAddr); //д��8λ��ַ
      if (i2c_WaitAck() != 0)
    {
      while(1);
    }
      while(Numbers--)
      {
          i2c_SendByte (*RdData);
          RdData++;
      }
      
      i2c_Stop ();
     
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
