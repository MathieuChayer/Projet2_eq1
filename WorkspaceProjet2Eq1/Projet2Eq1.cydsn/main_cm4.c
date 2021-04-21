/******************************************************************************
* main_cm4.c
*
* Auteur : Mathieu
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
#include "traitement.h"
#include "string.h"
#include "task.h"
#include <stdio.h>

volatile uint8_t data = 0; // Variable temporaire de lecture/écriture

// Flags contextuels - Interface
volatile int FLAG_RED       = 0;
volatile int FLAG_option    = 1;
volatile int FLAG_menu      = 0;
volatile int FLAG_modif     = 0;

volatile uint32_t  AC_RED = 0; 
volatile uint32_t  DC_RED = 0; 
volatile int       HR_RED = 0;
volatile uint32_t  AC_IR  = 0; 
volatile uint32_t  DC_IR  = 0; 
volatile int       HR_IR  = 0;

// Tâche principale
void Task_principal(void)
{
    Start_Oxymeter();
    drawWaiting();
    Cy_SysLib_Delay(2000); // Délais de 2s qui laisse le temps à l'utilisateur de mettre son doigt

    for(;;)
	{  
        if(!FLAG_menu)
        {
           // On entre dans cette boucle s'il y une modification à faire au courant
           if(FLAG_modif)
           {
                drawWaiting();
                Cy_SysLib_Delay(2000); // Délais de 2s qui laisse le temps à l'utilisateur de mettre son doigt
                set_LED_current(Current_LED1,Current_LED2); 
                FLAG_modif = 0;
           }
           
           // 1-Échantillonnage (Mathieu)
           MAX_ReadFIFO(&IR_data, &RED_data); // Lecture du FIFO --> 2000 samples@200sps = 10 secs
            
           // On ferme les deux LEDs d'alarme 
           Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);
           Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);
        
            if(!FLAG_menu)
            {
                 // 2-Traitement (Ariane)
                traitement(&RED_data, &HR_RED, &AC_RED, &DC_RED);                    
                traitement(&IR_data, &HR_IR, &AC_IR, &DC_IR);
                    
                interpretation(HR_RED, HR_IR, AC_RED,DC_RED,DC_IR,AC_IR, &HR, &SPO2);
                
                 // 3 - Affichage de la courbe (Andréa)
                drawAffichageCourbe();
            
                // Vérification des alertes physiologiques 
                if(HR_max_alarm < HR || HR_min_alarm > HR || SPO2_min_alarm > SPO2)
                {
                    Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,0);
                }
                else
                {
                    Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);  
                }
            }
        }
        else
        {
            // On éteint les deux LEDs d'alarme
            Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);
            Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);    
            
            if(FLAG_menu == 1)
            {   
                draw_MenuPrincipal();
                
                int button0_pressed = 0;
                int button1_pressed = 0;
                while(button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
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
                    if(FLAG_menu > 0)
                    {
                        FLAG_option += 1;
                        if(FLAG_option > 6)
                        {
                            FLAG_option = 1;
                        }
                    }
                }
                
                if(button1_pressed)
                {
                    if(FLAG_menu > 0)
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
	}
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    UART_Start(); 
    Cy_GPIO_Write(MAX_power_0_PORT,MAX_power_0_NUM, 1); // Alimentation du MAX30102

    // Interruption de la détection de mouvement
    NVIC_DisableIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
    Cy_GPIO_ClearInterrupt(Pin_AnyMotion_INT_PORT, Pin_AnyMotion_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_AnyMotionINT_cfg.intrSrc);

    // Interruption du bouton SW2
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
    
    xTaskCreate(Task_principal, "Task Principal", 8000, NULL,configMAX_PRIORITIES - 1, NULL);
	vTaskStartScheduler();

    for(;;)
    {
       
    }
}

/* [] END OF FILE */