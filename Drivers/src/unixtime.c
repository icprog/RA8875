/*
********************************************************************************
* felename :               unixtime.c
* author   :               sunlishuang
* date     :               2014.08.28
* describe :               unix time transmit .
********************************************************************************
*/


/*
�޸ļ�¼:
--------------------------------------------------------------------------------
2014.09.09
1.��1970��1�·ݽ���������,��Ϊ����ʱ���Ƕ���ʱ��,�����ʱ���,����8��Сʱ;
2.�����Ͷ����ʽ�����˶�������,Ϊ���ɿ��ļ���׼��;
--------------------------------------------------------------------------------
*/
#define  UNIXTIME_C

/*include files*/
#include "unixtime.h"

#define START_YEAR                      1970                                    /*���ʱ�׼��1970-01-01 00:00:00��ʼ*/
#define END_YEAR                        2106                                    /*ʱ�������ʱ��,32λ��������������ʱ������*/
#define TIME_AREA                          8
#define BIAS_TIMEAREA_TIME              (3600*TIME_AREA)                        /*Ĭ�϶�8ʱ��*/
/*
********************************************************************************
* ��������: Unixtime_To_Localtime
* ����    : Localtime-�û����ݵ��β�,�˺�����Localtime-Unixtimeת������ͨʱ�䲢��ֵ
            ��
            Localtime.UnixYear=***;
            Localtime.UnixMonth=***;
            Localtime.UnixDay=***;
            Localtime.UnixHour=***;
            Localtime.UnixMinute=***;
            Localtime.UnixSecond=***;            
* ��������: ��Localtime->Unixtime32λ�޷�������ʱ���ʽת����������Localtimeʱ���ʽ;
* ����ֵ  : 0-ת���ɹ�;
            1-�������Ϸ�;
********************************************************************************
*/
INT8U_LIB_ARM Unixtime_To_Localtime_0(TIME_UNIX * Localtime)
{
  const INT8U_LIB_ARM Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
  INT32U_LIB_ARM utime,n32_Pass4year=0;
  INT32U_LIB_ARM n32_hpery=0;
  
  if(!Localtime) return 1;                                                      /*checking validity of the parameter*/
  
  utime=Localtime->Unixtime;
  utime += 28800;                                                               /*���ϱ���ʱ��*/  
  Localtime->UnixSecond=(INT8U_LIB_ARM)(utime % 60);                            /*ȡ��ʱ��*/
  utime /= 60;  
  Localtime->UnixMinute=(INT8U_LIB_ARM)(utime % 60);                            /*ȡ����ʱ��*/  
  utime /= 60;                                                                  /*ת����Сʱ*/
  n32_Pass4year=((INT32U_LIB_ARM)utime / ((365*4+1) * 24L ));                   /*�����ȥ���ٸ����꣬ÿ������ 1461*24 Сʱ*/ 
  Localtime->UnixYear=(n32_Pass4year << 2)+1970;                                /*�������*/  
  utime %= (365*4+1) * 24L;                                                     /*������ʣ�µ�Сʱ��*/
  /*У������Ӱ�����ݣ�����һ����ʣ�µ�Сʱ��*/
  for (;;)
  {    
    n32_hpery = 365 * 24;                                                       /*һ���Сʱ��*/    
    if ((Localtime->UnixYear & 3) == 0)                                         /*�ж�����*/
    {
      n32_hpery += 24;                                                          /*�����꣬һ�����24Сʱ����һ��*/
    }
    if (utime < n32_hpery)
    {
      break;
    }
     Localtime->UnixYear++;
     utime -= n32_hpery;
    }
    /*Сʱ��*/
    Localtime->UnixHour=(INT8U_LIB_ARM)(utime % 24);
    /*һ����ʣ�µ�����*/
    utime /= 24;
    /*�ٶ�Ϊ����*/
    utime++;
    /*У��������������·ݣ�����*/
    if ((Localtime->UnixYear & 3) == 0)      
    {
      if (utime > 60)
      {
        utime--;
      }
      else
      {
      if (utime == 60)
      {
        Localtime->UnixMonth = 2;
        Localtime->UnixDay = 29;
        return 0;
      }
    }
  }
  /*��������*/
  for (Localtime->UnixMonth = 0; Days[Localtime->UnixMonth] < utime;Localtime->UnixMonth++)
  {
    utime -= Days[Localtime->UnixMonth];
  }
  Localtime->UnixMonth = Localtime->UnixMonth+1;
  Localtime->UnixDay = (INT8U_LIB_ARM)(utime);
  return 0;
}
/*
********************************************************************************
* ��������: Unixtime_To_Localtime
* ����    : Localtime-�û����ݵ��β�,�˺�����Localtime-Unixtimeת������ͨʱ�䲢��ֵ
            ��
            Localtime.UnixYear=***;
            Localtime.UnixMonth=***;
            Localtime.UnixDay=***;
            Localtime.UnixHour=***;
            Localtime.UnixMinute=***;
            Localtime.UnixSecond=***;  
* ����ֵ  : 0-ת���ɹ�;
            1-�������Ϸ�;
            2-
            3-
            4-ת�������޳���������;
* ��������: ��Localtime->Unixtime32λ�޷�������ʱ���ʽת����������Localtimeʱ���ʽ;            
********************************************************************************
*/
INT8U_LIB_ARM Unixtime_To_Localtime (TIME_UNIX * Localtime)
{
  INT8U_LIB_ARM daytemp;
  INT32U_LIB_ARM unixtime,restime,i;
  INT32U_LIB_ARM monthtable[13]={0,2678400,5097600,7776000,10368000,13046400,15638400,18316800,
                         20995200,23587200,26265600,28857600,31536000};         /*�ۼ�����,Ĭ���ǰ���ƽ�����*/
  TIME_UNIX t;
  
  if(!Localtime) return 1;
  
  /*��ȡ��ת����unixʱ���*/  
  unixtime=Localtime->Unixtime;
  
  /*�Ƚ�ʱ�丳��ֵ:  1971-01-01 00:00:00*/  
  t.UnixYear=START_YEAR+1;                                                      
  t.UnixMonth=1;
  t.UnixDay=1;
  t.UnixHour=0;
  t.UnixMinute=0;
  t.UnixSecond=0;
  Localtime_To_Unixtime(&t);

  /*�ж����ʱ��*/
  Localtime->UnixYear=START_YEAR;
  for(i=START_YEAR+1;i<END_YEAR;i++)                                            /*ͨ����ѭ���ж�����*/
  {
    t.UnixYear=i;
    Localtime_To_Unixtime(&t);

    if(unixtime>t.Unixtime)
    {
      Localtime->UnixYear=i;                                                    /*��ʱ������ݸ�ֵ��unixtime->UnixYear,�ٽ����´��ж�*/
      continue;
    }
    else if(unixtime==t.Unixtime)                                               /*���������ֵ,������*/
    {
      *Localtime=t;
      return 0;
    }         
    else 
    {
      if(t.UnixYear==(START_YEAR+1))
      {
        t.UnixHour=TIME_AREA;
      }
      t.UnixYear-=1;
      Localtime_To_Unixtime(&t);
      break;
    }
  }
  if(i>=END_YEAR) return 4;
  restime=unixtime-t.Unixtime;                                                  /*���������һ�������*/  
  
  if(((Localtime->UnixYear% 4) == 0 && ((Localtime->UnixYear% 100) != 0 || (Localtime->UnixYear % 400) == 0)))/*�ж�����*/
  {
    /*���������,�·����������μ�24*3600=86400s*/
    for(i=2;i<13;i++)
    {
      monthtable[i] = monthtable[i] + 86400;
    }    
  }
  /*�ж��ǲ���1970��,��Ϊʱ���ԭ��,Ҫ���в���*/
  if(Localtime->UnixYear==START_YEAR)
  {
    for(i=1;i<12;i++)
    {
      monthtable[i]-=BIAS_TIMEAREA_TIME;
    }
  }  
  
  /*�ж��·�,��ʼ1�¿�ʼ�ж�*/
  i=0;                                                                          /*��1�·ݿ�ʼ*/
  do
  {    
    i++;
    if(i>12) return 3;
    Localtime->UnixMonth=i;
  }
  while(restime>=monthtable[i]);                                                /*�·ݳ�����ǰ�·ݵ�����,�������·��ж�*/
  
  switch(Localtime->UnixMonth)                                                  /*�����·�,�ж���*/
  {
    case 1:
    /*1970-1-1 8:00:00��ʼ����unixʱ��;*/
    if(Localtime->UnixYear==START_YEAR)
    {
      restime+=BIAS_TIMEAREA_TIME;
    }
    daytemp=restime/86400;
    break;
    case 2:
    restime-=monthtable[1];
    daytemp=restime/86400;
    break;
    case 3:
    restime-=monthtable[2];
    daytemp=restime/86400;
    break;
    case 4:
    restime-=monthtable[3];
    daytemp=restime/86400;
    break;
    case 5:
    restime-=monthtable[4];
    daytemp=restime/86400;
    break;
    case 6:
    restime-=monthtable[5];
    daytemp=restime/86400;
    break;
    case 7:
    restime-=monthtable[6];
    daytemp=restime/86400;
    break;
    case 8:
    restime-=monthtable[7];
    daytemp=restime/86400;
    break;
    case 9:
    restime-=monthtable[8];
    daytemp=restime/86400;
    break;
    case 10:
    restime-=monthtable[9];
    daytemp=restime/86400;
    break;
    case 11:
    restime-=monthtable[10];
    daytemp=restime/86400;
    break;
    case 12:
    restime-=monthtable[11];
    daytemp=restime/86400;
    break;
    default:    
    break;
  }  
  Localtime->UnixDay=daytemp+1;                                                 /*����ÿ���µ����ڴ�1�ſ�ʼ*/

  restime-=daytemp*86400;
  Localtime->UnixHour=restime/3600;                                             /*Сʱ\����\�붼�Ǵ�0ʱ��ʼ����*/
  restime%=3600;
  Localtime->UnixMinute=restime/60;
  Localtime->UnixSecond=restime%60;
  
  return 0;  
}

/*
********************************************************************************
* ��������: Localtime_To_Unixtime
* �β�    : Localtime-ʱ��ṹָ��,�ṹ���а�����ͨʱ���ʽ��unixʱ��(32λ�޷���)
            ʱ���ʽ;
* ����ֵ  : ����ת��״̬��Ϣ
            0-ת���ɹ�;
            1-�û����ݲ������Ϸ�;
* ��������: �˺�����Localtime����ͨʱ���ʽת����unixʱ���ʱ���ʽ,����ֵ��Localtime->Unixtime;            
********************************************************************************
*/
INT8U_LIB_ARM Localtime_To_Unixtime(TIME_UNIX* Localtime)
{
  INT8U_LIB_ARM month=Localtime->UnixMonth,day=Localtime->UnixDay, hour=Localtime->UnixHour;
  INT8U_LIB_ARM minute=Localtime->UnixMinute, second=Localtime->UnixSecond;
  INT16U_LIB_ARM i,x,unixtimeyear,year=Localtime->UnixYear;
  INT16U_LIB_ARM monthtable[13]={0,31,59,90,120,151,181,212,243,273,304,334,365};       /*�ۼ�����*/
  INT32U_LIB_ARM  unixtimeday,extraday;  
  
  if(!Localtime) return 1;                                                      /*checking validity of the parameter*/  
  
  for(x=1970,extraday=0;x<year;x++)                                             /*����1970�굽�����ж������µĶ�һ��*/
  {
    if(((x % 4) == 0 && ((x % 100) != 0 || (x % 400) == 0)))
    {
      extraday++;
    }
  }
  unixtimeyear = year - 1970;
  if((((year) % 4) == 0 && (((year) % 100) != 0 || ((year) % 400) == 0)))       /*�жϵ����Ƿ�������,����������޸����ۼ���������*/
  {
    for(i=2;i<12;i++)
    {
      monthtable[i] = monthtable[i] + 1;
    }
  }  
  unixtimeday = unixtimeyear*365 + monthtable[month-1] + day-1 + extraday;      /*�����ܹ�������*/
  Localtime->Unixtime = unixtimeday*86400 + hour*3600+minute*60 + second-BIAS_TIMEAREA_TIME;/*���չ��ʱ�׼,����ʱ��(��8ʱ��)1970-1-1 8:00:00��ʼ����unixʱ��Ϊ0;*/  
  return 0;
}


