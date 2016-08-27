
#include "rtc.h"
#include "unixtime.h"


#define RTC_Debug                      0     /* ����ѡ�����ģʽ0-��ͨģʽ,1-����ģʽ; */

/* ѡ��RTC��ʱ��Դ */
#define RTC_CLOCK_SOURCE_LSE           0     /* LSE */
#define RTC_CLOCK_SOURCE_LSI           1     /* LSI */ 
#define RTC_SOURCE   RTC_CLOCK_SOURCE_LSE    /*ʱ��Դѡ��*/


/* ���� */
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_InitTypeDef  RTC_InitStructure;
RTC_DateTypeDef  RTC_DateStructure;


TIME_UNIX TimeNow;                              /*ϵͳʱ��-����ʱ��,��������ʱ������Ϣ*/
static void RTC_Config(void);

/* ��������RTC��Ƶ */
__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
/*
*********************************************************************************************************
*  �� �� ��: bsp_InitRTC
*  ����˵��: ��ʼ��RTC
*  ��    �Σ���
*  �� �� ֵ: ��            
*********************************************************************************************************
*/
void bsp_InitRTC(void)
{  
  /* ���ڼ���Ƿ��Ѿ����ù�RTC��������ù��Ļ����������ý���ʱ
  ����RTC���ݼĴ���Ϊ0x32F2��������RTC���ݼĴ�������0x32F2
  ��ô��ʾû�����ù�����Ҫ����RTC.
  */
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  {  
    /* RTC ����  */
    RTC_Config();
    
    /* ��ӡ������Ϣ */
#if (RTC_Debug==1)
    printf("��һ��ʹ��RTC \n\r");
#endif   
    
    /* ����ϵ縴λ��־�Ƿ����� */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      /* �����ϵ縴λ */
#if (RTC_Debug==1)
      printf("�����ϵ縴λ \n\r");
#endif
    }
  }
  else
  {
    /* ��ӡ������Ϣ */
#if (RTC_Debug==1)
    printf("��n��ʹ��RTC \n\r");
#endif
    
    /* ����ϵ縴λ��־�Ƿ����� */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      /* �����ϵ縴λ */
#if (RTC_Debug==1)
      printf("�����ϵ縴λ \n\r");
#endif
    }
    /* ������Ÿ�λ��־�Ƿ����� */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      /* �������Ÿ�λ */
#if (RTC_Debug==1)
      printf("�������Ÿ�λ \n\r");
#endif      
    }
    
    /* ʹ��PWRʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    
    /* �������RTC */
    PWR_BackupAccessCmd(ENABLE);
    
    /* �ȴ� RTC APB �Ĵ���ͬ�� */
    RTC_WaitForSynchro();
    
    /* ���RTC���ӱ�־ */
    RTC_ClearFlag(RTC_FLAG_ALRAF);
    
    /* ���RTC�����жϹ����־ */
    EXTI_ClearITPendingBit(EXTI_Line17);
  }
  
}

/*
*********************************************************************************************************
*  �� �� ��: RTC_Config
*  ����˵��: ��������ʱ�������
*  ��    �Σ���
*  �� �� ֵ: ��
*********************************************************************************************************
*/
static void RTC_Config(void)
{  
  /* ʹ��PWRʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* �������RTC */
  PWR_BackupAccessCmd(ENABLE);
  
  /* ѡ��LSI��Ϊʱ��Դ */
#if (RTC_SOURCE==RTC_CLOCK_SOURCE_LSI)
  
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  
  /* ѡ��RTCʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  /* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
  uwSynchPrediv = 0xFF;
  uwAsynchPrediv = 0x7F;
  
  /* ѡ��LSE��ΪRTCʱ�� */
#elif (RTC_SOURCE==RTC_CLOCK_SOURCE_LSE)
  /* ʹ��LSE����  */
  RCC_LSEConfig(RCC_LSE_ON);
  
  /* �ȴ����� */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  
  /* ѡ��RTCʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  uwSynchPrediv = 0xFF;
  uwAsynchPrediv = 0x7F;
  
#else
#error "Please select the RTC Clock source inside the main.c file"
#endif 
  
  /* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);
  
  /* �ȴ�RTC APB�Ĵ���ͬ�� */
  RTC_WaitForSynchro();
  
  /* ʹ��ʱ��� */
  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);  
  
  RTC_TimeStampPinSelection(RTC_TamperPin_PC13);  
  
  /* ����RTC���ݼĴ����ͷ�Ƶ��  */
  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
  
  /* ���RTC��ʼ�� */
  if (RTC_Init(&RTC_InitStructure) == ERROR)
  {
    /* ��ӡ������Ϣ */
#if (RTC_Debug==1)
    printf("RTC��ʼ��ʧ�� \n\r");
#endif
  }
  
  /* ���������պ����� */
  RTC_DateStructure.RTC_Year = 0x13;
  RTC_DateStructure.RTC_Month = RTC_Month_January;
  RTC_DateStructure.RTC_Date = 0x11;
  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
  
  /* ����ʱ���룬�Լ���ʾ��ʽ */
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  RTC_TimeStructure.RTC_Hours   = 0x05;
  RTC_TimeStructure.RTC_Minutes = 0x20;
  RTC_TimeStructure.RTC_Seconds = 0x00; 
  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
  
  /* ���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC */
  RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}
/*
*********************************************************************************************************
*  �� �� ��: GetTimeNow
*  ����˵��: ��ȡϵͳʱ��,��ת����unixʱ���;
*  ��    �Σ���
*  �� �� ֵ: ��
*********************************************************************************************************
*/
void GetTimeNow(void)
{  
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

  /*������ʱ����*/
  TimeNow.UnixYear=RTC_DateStructure.RTC_Year+2000;
  TimeNow.UnixMonth=RTC_DateStructure.RTC_Month;
  TimeNow.UnixDay=RTC_DateStructure.RTC_Date;
  TimeNow.UnixWeekDay=RTC_DateStructure.RTC_WeekDay;
  TimeNow.UnixHour=RTC_TimeStructure.RTC_Hours;
  TimeNow.UnixMinute=RTC_TimeStructure.RTC_Minutes;
  TimeNow.UnixSecond=RTC_TimeStructure.RTC_Seconds;

  /*ת��UNIXTIME*/
  Localtime_To_Unixtime(&TimeNow);
}

