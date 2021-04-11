/******************************************************************************
* File Name: main_cm4.c

********************************************************************************/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include <math.h>
#include "FreeRTOS.h"
#include "max30102.h"
#include "semphr.h"
#include "max30102.h"
#include "bmi160.h"
#include "bmi160_defs.h"
#include "communication.h"
#include "interface.h"
#include "string.h"
#include "task.h"
#include <stdio.h>


volatile uint8_t data = 0; //Variable temporaire de lecture/éc


//Flags contextuels - Interface
volatile int FLAG_RED = 1;
volatile int FLAG_option = 1;
volatile int FLAG_menu = 0;
volatile int FLAG_modif = 0;


//Tâche principale
void Task_principal(void)
{
    
    Start_Oxymeter();
    drawWaiting();
    
    for(;;)
	{  
        if(!FLAG_menu)
        {
           //On entre dans cette boucle s'il y une modification à faire au courant
           if(FLAG_modif)
           {
                drawWaiting();
                set_LED_current(Current_LED1,Current_LED2); 
                FLAG_modif = 0;
           }
            
          
           //1-Échantillonnage (Mathieu)
           MAX_ReadFIFO(&IR_data, &RED_data); //Lecture du FIFO --> 1000 samples@200sps = 5 secs
            
           //On allume la LED verte et on ferme les deux autres
           Cy_GPIO_Write(GreenLED_0_PORT,GreenLED_0_NUM,0);
           Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);
           Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);
       
            /*
            //Affichage des données dans TERA
            for (int i=0;i<2000;i++)
            {
                printf("RED : %lu   IR : %lu\r\n",RED_data[i],IR_data[i]);
                //printf("%lu \r\n",RED_data[i]);
                //printf("%lu \r\n",IR_data[i]);
            }*/
            
            
            //Remettre la LED verte et fermer la LED rouge éventuellement (prochain cycle)
            
            if(!FLAG_menu)
            {
                 //2-Traitement (Ariane)
                 //3 - Affichage de la courbe (Andréa)
            
                drawAffichageCourbe();
            
                //Vérification des alertes physiologiques 
                if(HR_max_alarm<HR || HR_min_alarm>HR || SPO2_min_alarm > SPO2)
                {
                    Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,0);
                    Cy_GPIO_Write(GreenLED_0_PORT,GreenLED_0_NUM,1);
                }
                else
                {
                    Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);  
                    Cy_GPIO_Write(GreenLED_0_PORT,GreenLED_0_NUM,0);
                }
            
            }
            
   
        }
        else
        {
            //On remet la LED verte dans le menu et on éteint les deux autres
            Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);
            Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);
            Cy_GPIO_Write(GreenLED_0_PORT,RedLED_0_NUM,0); 
            
            if(FLAG_menu==1)
            {   draw_MenuPrincipal();
                
                int button0_pressed =0;
                int button1_pressed =0;
                while(button0_pressed == 0 && button1_pressed ==0 && FLAG_menu > 0){
                    
                    CapSense_ScanAllWidgets();

                    if(!CapSense_IsBusy())
                    {
                         CapSense_ProcessAllWidgets();
                         button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                         button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);

                    }  
                }
                
                if(button0_pressed)
                {
                    if(FLAG_menu >0)
                    {
                        FLAG_option += 1;
                        if(FLAG_option>6)
                        {
                            FLAG_option = 1;
                        }
                    }
                }
                
                if(button1_pressed)
                {
                    if(FLAG_menu >0)
                    {
                        FLAG_menu = FLAG_option+1;
                     
                    }
                }
            } 
            else
            {
                draw_SousMenu();
            }
            
        }
        
            
        UpdateDisplay(CY_EINK_FULL_4STAGE, true);
        vTaskDelay(pdMS_TO_TICKS(1000));    
          
	}
}
int main(void)
{
    
    __enable_irq(); /* Enable global interrupts. */
    
    UART_Start(); 
    Cy_GPIO_Write(MAX_power_0_PORT,MAX_power_0_NUM, 1); //Alimentation du MAX30102

    NVIC_DisableIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
    Cy_GPIO_ClearInterrupt(Pin_AnyMotion_INT_PORT, Pin_AnyMotion_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
    
    Cy_SysInt_Init(&SW2_isr_cfg, isr_SW2);
    NVIC_ClearPendingIRQ(SW2_isr_cfg.intrSrc);
    NVIC_EnableIRQ(SW2_isr_cfg.intrSrc);
    
    CapSense_Start();

    GUI_Init();
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
   
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    xTaskCreate(Task_principal, "Task Principal", configMINIMAL_STACK_SIZE, NULL,configMAX_PRIORITIES - 1, 0);
	vTaskStartScheduler();

    for(;;)
    {
       
    }
}

/* [] END OF FILE */
