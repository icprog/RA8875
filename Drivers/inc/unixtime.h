/*
********************************************************************************
* felename :                    unixtime.h
* author   :                    sunlishuang
* date     :                    2014.08.28
* describe :                    unixtime header file.
********************************************************************************
*/
#ifndef __UNIXTIME_H
#define __UNIXTIME_H

#define BYTE_LIB_ARM              char
#define INT8U_LIB_ARM    unsigned char
#define INT16U_LIB_ARM   unsigned short
#define INT32U_LIB_ARM   unsigned int

/*unixtime structure*/
typedef struct
{
  /*��ͨʱ��*/
  INT16U_LIB_ARM UnixYear;
  INT8U_LIB_ARM  UnixMonth;
  INT8U_LIB_ARM  UnixDay;
  INT8U_LIB_ARM  UnixWeekDay;
  INT8U_LIB_ARM  UnixHour;
  INT8U_LIB_ARM  UnixMinute;
  INT8U_LIB_ARM  UnixSecond;

  /*unixʱ���*/
  INT32U_LIB_ARM Unixtime ;
}TIME_UNIX;

/*functions of unixtime converting declaration*/
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
INT8U_LIB_ARM Localtime_To_Unixtime(TIME_UNIX* Localtime);

/*
********************************************************************************
* ��������: Unixtime_To_Localtime(����bug,���Ƿ���ֵ�������о�)
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
INT8U_LIB_ARM Unixtime_To_Localtime_0(TIME_UNIX * Localtime);

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
* ��������: ��Localtime->Unixtime32λ�޷�������ʱ���ʽת����������Localtimeʱ���ʽ;            
********************************************************************************
*/
INT8U_LIB_ARM Unixtime_To_Localtime(TIME_UNIX * Localtime);
#endif /*unixtime.h*/

