/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */


FONT_T textfont;
FATFS fs1[2];
int main(void)
{
	
  
	uint32_t fontcnt;		  
	uint8_t i,j;
	uint8_t fontx[2];//gbk��
	uint8_t key,t;	  


 
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200	
        delay_init(168);	    	 //��ʱ������ʼ��
        TIM7_Int_Init(999,83);
        KEY_Init();
	
        //AT24CXX_Init();		//��ʼ��24CXX
       // AT24CXX_Read( 40,data_r, 14); 
        //data_r[0]=AT24CXX_ReadOneByte(53);
        FSMC_SRAM_Init(); 		//SRAM��ʼ��
	LED_Init();
        bsp_InitI2C();
 textfont.BackColor=CL_BLUE;
  textfont.FrontColor=CL_BLACK;
  textfont.FontCode=FC_ST_16;
    textfont.Space=0;
  /*ʱ��*/
  bsp_InitRTC();

	mem_init(SRAMIN); 		//��ʼ���ڲ��ڴ��
	mem_init(SRAMEX);  		//��ʼ���ⲿ�ڴ��
	mem_init(SRAMCCM); 		//��ʼ��CCM�ڴ��
        TIM3_Int_Init(999,83);  
	W25QXX_Init();
        LCD_InitHard();
	 TOUCH_InitHard(); 
	 LCD_ClrScr(CL_WHITE);
       
	 
	 exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  	f_mount(&fs1[0],"0:",1); 		//����SD�� 
 	f_mount(&fs1[1],"1:",1); 		//����FLASH.
		
      
	
	//LCD_DispStr(20,20,"test",&textfont);
      
//	while(font_init()) 			//����ֿ�
//	  
//	{
//  
//		   	   	  
//		RA8875_DispAscii(30,50,"Explorer STM32F4");
//		while(SD_Init())			//���SD��
//		{
//			RA8875_DispAscii(30,70,"SD Card Failed!");
//			delay_ms(200);
////			LCD_Fill(30,70,200+30,70+16,WHITE);
////			delay_ms(200);		    
//		}								 						    
//		RA8875_DispAscii(30,70,"SD Card OK");
//		RA8875_DispAscii(30,90,"Font Updating...");
//		key=update_font(20,110,16,"0:");//�����ֿ�
//		while(key)//����ʧ��		
//		{			 		  
//			RA8875_DispAscii(30,110,200,16,16,"Font Update Failed!");
//			delay_ms(200);
//			RA8875_FillRect(20,110,200,110,CL_WHITE);
//			delay_ms(200);		       
//		} 		  
//		RA8875_DispAscii(30,110,200,16,16,"Font Update Success!   ");
//		delay_ms(1500);	
//		LCD_ClrScr(CL_WHITE);      
//	}  
//	RA8875_SetFrontColor(CL_RED);     
//	Show_Str(30,50,200,16,"̽����STM32F407������",16,0);				    	 
//	Show_Str(30,70,200,16,"GBK�ֿ���Գ���",16,0);				    	 
//	Show_Str(30,90,200,16,"����ԭ��@ALIENTEK",16,0);				    	 
//	Show_Str(30,110,200,16,"2014��5��15��",16,0);
//	Show_Str(30,130,200,16,"��KEY0,�����ֿ�",16,0);
// 	RA8875_SetFrontColor(CL_BLUE); 
//	Show_Str(30,150,200,16,"������ֽڣ�",16,0);				    	 
//	Show_Str(30,170,200,16,"������ֽڣ�",16,0);				    	 
//	Show_Str(30,190,200,16,"���ּ�������",16,0);
//
//	Show_Str(30,220,200,24,"��Ӧ����Ϊ��",24,0); 
//	Show_Str(30,244,200,16,"��Ӧ����(16*16)Ϊ��",16,0);			 
//	Show_Str(30,260,200,12,"��Ӧ����Ϊ��",12,0);			 
//	while(1)
//	{
//		fontcnt=0;
//		for(i=0x81;i<0xff;i++)
//		{		
//			fontx[0]=i;
//			RA8875_DispNum(118,150,i,3,16);		//��ʾ������ֽ�    
//			for(j=0x40;j<0xfe;j++)
//			{
//				if(j==0x7f)continue;
//				fontcnt++;
//				RA8875_DispNum(118,170,j);	//��ʾ������ֽ�	 
//				RA8875_DispNum(118,190,fontcnt);//���ּ�����ʾ	 
//			 	fontx[1]=j;
//				Show_Font(30+132,220,fontx,24,0);	  
//				Show_Font(30+144,244,fontx,16,0);	  		 		 
//				Show_Font(30+108,260,fontx,12,0);	  		 		 
//				t=200;
//				while(t--)//��ʱ,ͬʱɨ�谴��
//				{
//					delay_ms(1);
//					
//				}
//				
//			}   
//		}	
//	}
	
	
	
	
	
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);
//	WM_SetCreateFlags(WM_CF_MEMDEV);
//
//       
//       
//       printf("Back color is 0x%04X\r\n",RA8875_GetBackColor());
//       printf("Front color is 0x%04X\r\n",RA8875_GetFrontColor());
//	GUI_Init();                /* ��ʼ�� */
//	
//	keypad_demo();
  	while(1) 
	{
      LCD_test_Speed();
        }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
