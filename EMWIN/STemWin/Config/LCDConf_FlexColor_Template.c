#include "GUI.h"
#include "lcd.h"
#include "touch.h"
#include "GUIDRV_Template.h"
#include "GUIDRV_FlexColor.h"

//�봥�����йض��壬����ʵ�������д
#define TOUCH_AD_TOP		160  	//���´������Ķ�����д�� Y ��ģ������ֵ��
#define TOUCH_AD_BOTTOM		3870 	//���´������ĵײ���д�� Y ��ģ������ֵ��
#define TOUCH_AD_LEFT 		3760		//���´���������࣬д�� X ��ģ������ֵ��
#define TOUCH_AD_RIGHT		200	//���´��������Ҳ࣬д�� X ��ģ������ֵ��


//��Ļ��С
#define XSIZE_PHYS  854 //X��
#define YSIZE_PHYS  480 //Y��
#define VXSIZE_PHYS	854 
#define VYSIZE_PHYS 480


//���ü��
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

  

//���ó���,���ڴ�����ʾ��������,������ɫת���������ʾ�ߴ�
void LCD_X_Config(void) {
	GUI_DEVICE_CreateAndLink(&GUIDRV_Template_API, GUICC_M565, 0, 0); //������ʾ��������
	LCD_SetSizeEx    (0, lcddev.width, lcddev.height);
	LCD_SetVSizeEx   (0, lcddev.width, lcddev.height);
	
		if(lcddev.dir == 0) //����
		{					
			GUI_TOUCH_Calibrate(GUI_COORD_X,0,lcddev.width-1,TOUCH_AD_LEFT,TOUCH_AD_RIGHT);
			GUI_TOUCH_Calibrate(GUI_COORD_Y,0,lcddev.height-1,TOUCH_AD_TOP,TOUCH_AD_BOTTOM);
		}else //����
		{
			GUI_TOUCH_SetOrientation(GUI_SWAP_XY|GUI_MIRROR_Y); 
			GUI_TOUCH_Calibrate(GUI_COORD_X,0,479,TOUCH_AD_LEFT,TOUCH_AD_RIGHT); 	
			GUI_TOUCH_Calibrate(GUI_COORD_Y,0,853,TOUCH_AD_TOP,TOUCH_AD_BOTTOM);
		}
	
}

//��ʾ�������Ļص�����
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
	//����ʼ����ʱ�򱻵���,��Ҫ��������ʾ������,�����ʾ���������ⲿ��ʼ������Ҫ�û���ʼ��
		
	//	TFTLCD_Init(); //��ʼ��LCD �Ѿ��ڿ�ʼ��ʼ����,���Դ˴�����Ҫ��ʼ����
    return 0;
  }
		default:
    r = -1;
	}
  return r;
}
