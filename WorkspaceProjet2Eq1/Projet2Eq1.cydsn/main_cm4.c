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
#include "traitement.h"
#include "string.h"
#include "task.h"
#include <stdio.h>


volatile uint8_t data = 0; //Variable temporaire de lecture/écriture


//Flags contextuels - Interface
volatile int FLAG_RED = 1;
volatile int FLAG_option = 1;
volatile int FLAG_menu = 0;
volatile int FLAG_modif = 0;
volatile uint32_t RED[2000] = {54573,54583,54544,54569,54550,54551,54565,54558,54574,54568,54553,54574,54563,54558,54548,54558,54563,54549,54556,54553,54548,54563,54539,54541,54538,54512,54507,54479,54448,54397,54365,54351,54312,54290,54240,54239,54240,54208,54218,54214,54228,54237,54228,54248,54244,54263,54282,54284,54308,54316,54325,54348,54334,54371,54358,54378,54406,54391,54407,54408,54416,54432,54435,54450,54438,54445,54475,54449,54470,54461,54463,54491,54480,54495,54489,54494,54497,54494,54481,54499,54486,54516,54487,54511,54497,54489,54515,54477,54500,54480,54489,54517,54496,54508,54500,54493,54515,54499,54502,54500,54501,54527,54506,54506,54516,54516,54535,54515,54522,54519,54533,54542,54520,54530,54531,54529,54564,54526,54543,54533,54532,54556,54520,54533,54530,54521,54554,54520,54537,54533,54532,54568,54527,54533,54524,54550,54550,54528,54538,54523,54525,54541,54534,54530,54541,54534,54537,54521,54521,54525,54531,54541,54539,54533,54524,54530,54553,54543,54531,54530,54538,54567,54532,54541,54528,54535,54559,54545,54545,54538,54528,54549,54523,54548,54521,54551,54553,54526,54543,54517,54511,54533,54500,54468,54421,54394,54380,54345,54314,54258,54237,54241,54226,54218,54197,54206,54239,54206,54220,54219,54235,54267,54262,54294,54286,54301,54318,54318,54356,54346,54354,54390,54376,54395,54399,54417,54450,54426,54441,54414,54440,54472,54451,54443,54461,54438,54472,54451,54464,54468,54468,54473,54459,54463,54458,54481,54491,54471,54489,54462,54462,54482,54459,54476,54451,54473,54477,54467,54492,54465,54452,54482,54473,54485,54464,54463,54481,54464,54480,54469,54465,54494,54481,54486,54485,54482,54485,54482,54493,54487,54510,54507,54492,54517,54480,54494,54493,54482,54496,54487,54487,54494,54501,54503,54488,54499,54499,54493,54503,54496,54507,54499,54507,54518,54484,54500,54499,54495,54526,54494,54512,54505,54486,54516,54505,54483,54502,54496,54521,54498,54519,54497,54498,54513,54508,54515,54495,54508,54516,54510,54504,54511,54508,54529,54514,54500,54512,54522,54522,54505,54522,54516,54516,54539,54507,54518,54491,54489,54493,54456,54429,54377,54354,54337,54316,54293,54278,54244,54254,54227,54234,54227,54237,54274,54243,54262,54257,54265,54304,54292,54329,54319,54342,54369,54366,54357,54376,54388,54412,54383,54404,54410,54411,54447,54415,54429,54428,54427,54452,54441,54442,54433,54435,54462,54447,54439,54424,54429,54469,54443,54449,54434,54433,54458,54444,54440,54427,54446,54463,54439,54446,54449,54439,54450,54443,54452,54448,54436,54468,54437,54442,54433,54446,54469,54447,54471,54453,54455,54468,54448,54461,54454,54471,54482,54446,54454,54450,54473,54480,54460,54473,54458,54460,54479,54472,54474,54459,54458,54499,54469,54475,54474,54471,54480,54475,54472,54473,54461,54491,54462,54487,54470,54468,54499,54474,54474,54468,54468,54502,54493,54496,54475,54483,54499,54478,54489,54467,54477,54519,54474,54504,54475,54481,54510,54496,54495,54485,54488,54505,54474,54489,54503,54499,54505,54478,54479,54458,54479,54486,54474,54455,54418,54418,54384,54339,54332,54302,54289,54281,54231,54242,54223,54229,54241,54216,54228,54226,54247,54277,54271,54274,54283,54312,54333,54330,54352,54353,54360,54374,54367,54372,54370,54371,54399,54376,54393,54388,54394,54409,54399,54400,54396,54411,54417,54397,54409,54391,54416,54437,54409,54413,54406,54402,54439,54426,54419,54404,54411,54443,54412,54426,54406,54418,54438,54412,54418,54392,54423,54443,54406,54437,54415,54419,54437,54419,54433,54423,54423,54436,54422,54436,54430,54432,54453,54423,54450,54441,54440,54452,54440,54469,54447,54468,54447,54448,54465,54459,54449,54465,54459,54460,54446,54453,54476,54455,54473,54456,54460,54487,54461,54468,54454,54464,54478,54471,54472,54445,54457,54482,54454,54476,54463,54465,54474,54466,54481,54455,54463,54466,54472,54467,54464,54459,54483,54463,54465,54445,54456,54477,54467,54467,54458,54458,54462,54449,54478,54449,54436,54448,54444,54468,54446,54461,54454,54433,54447,54409,54394,54378,54342,54306,54278,54282,54247,54228,54209,54171,54187,54161,54156,54180,54173,54189,54188,54190,54224,54224,54241,54239,54247,54278,54264,54286,54285,54310,54328,54317,54328,54325,54338,54355,54333,54340,54358,54345,54374,54361,54374,54374,54357,54383,54363,54385,54374,54369,54385,54388,54378,54384,54385,54398,54385,54385,54382,54393,54412,54401,54407,54400,54387,54410,54384,54395,54405,54391,54421,54406,54398,54398,54400,54445,54420,54419,54399,54417,54422,54408,54419,54418,54438,54442,54409,54415,54415,54421,54439,54425,54430,54412,54428,54449,54429,54431,54430,54437,54457,54433,54435,54428,54434,54448,54425,54433,54423,54420,54444,54415,54436,54421,54428,54435,54425,54410,54428,54422,54437,54418,54429,54421,54425,54449,54415,54421,54426,54428,54450,54430,54438,54425,54432,54446,54432,54437,54415,54443,54459,54420,54442,54433,54435,54437,54425,54442,54435,54430,54463,54427,54431,54432,54422,54456,54410,54420,54375,54374,54359,54297,54288,54258,54253,54221,54186,54179,54162,54158,54162,54154,54157,54159,54165,54168,54179,54192,54199,54213,54247,54240,54252,54247,54277,54308,54296,54293,54283,54315,54331,54306,54334,54323,54333,54336,54329,54355,54339,54370,54382,54357,54361,54380,54360,54382,54381,54390,54370,54366,54417,54378,54389,54384,54376,54401,54372,54388,54382,54370,54396,54370,54376,54378,54376,54399,54387,54377,54371,54370,54417,54378,54386,54391,54384,54412,54370,54392,54392,54401,54420,54390,54389,54393,54390,54414,54381,54400,54377,54392,54425,54390,54407,54383,54396,54410,54389,54407,54409,54400,54439,54386,54419,54402,54394,54433,54396,54408,54403,54395,54440,54392,54414,54394,54425,54422,54415,54417,54413,54403,54432,54400,54404,54394,54422,54415,54407,54420,54392,54417,54444,54389,54415,54412,54412,54439,54409,54430,54403,54409,54444,54402,54412,54411,54399,54417,54397,54426,54418,54412,54432,54406,54412,54389,54383,54384,54333,54301,54274,54244,54252,54194,54170,54144,54139,54136,54106,54103,54094,54100,54134,54121,54140,54140,54141,54183,54187,54211,54196,54219,54246,54225,54247,54258,54283,54298,54273,54291,54298,54300,54326,54320,54339,54314,54346,54304,54291,54200,54291,54361,54344,54345,54215,54271,54379,54334,54340,54318,54323,54318,54305,54370,54323,54326,54308,54330,54348,54327,54345,54330,54336,54365,54339,54331,54339,54326,54352,54336,54338,54332,54358,54377,54341,54354,54346,54348,54370,54345,54375,54350,54354,54387,54360,54380,54357,54377,54397,54367,54384,54379,54371,54410,54376,54381,54377,54389,54391,54370,54404,54382,54382,54414,54379,54392,54376,54398,54400,54380,54383,54396,54371,54383,54330,54309,54273,54254,54255,54220,54195,54151,54138,54136,54109,54119,54091,54095,54130,54117,54132,54138,54144,54184,54161,54196,54186,54205,54211,54205,54241,54242,54238,54266,54250,54267,54243,54270,54292,54250,54276,54272,54263,54303,54281,54285,54283,54284,54323,54287,54313,54311,54310,54322,54302,54303,54309,54315,54335,54302,54316,54312,54312,54325,54308,54308,54304,54314,54319,54308,54306,54308,54307,54324,54312,54310,54295,54326,54335,54325,54326,54319,54320,54318,54320,54337,54311,54322,54314,54320,54316,54317,54330,54344,54314,54342,54325,54344,54342,54345,54341,54326,54341,54339,54316,54322,54318,54342,54333,54326,54337,54322,54348,54351,54353,54358,54331,54341,54345,54333,54345,54349,54339,54341,54343,54356,54337,54337,54362,54331,54349,54331,54327,54339,54330,54354,54345,54352,54330,54336,54354,54336,54355,54337,54329,54357,54335,54347,54348,54324,54347,54340,54336,54318,54319,54325,54300,54297,54281,54240,54227,54195,54183,54151,54150,54144,54122,54119,54108,54102,54125,54117,54111,54119,54143,54158,54164,54169,54177,54178,54185,54183,54197,54206,54199,54227,54219,54228,54228,54229,54254,54224,54243,54227,54251,54253,54240,54255,54246,54246,54279,54261,54253,54243,54267,54273,54268,54268,54252,54268,54277,54256,54270,54260,54263,54293,54257,54277,54272,54273,54275,54263,54261,54270,54272,54283,54265,54275,54264,54278,54286,54287,54293,54278,54285,54300,54286,54281,54279,54288,54302,54296,54292,54293,54279,54324,54301,54314,54290,54296,54312,54297,54322,54311,54295,54312,54304,54325,54307,54313,54323,54301,54319,54301,54304,54333,54293,54327,54311,54296,54342,54301,54317,54331,54328,54333,54307,54304,54309,54305,54332,54315,54322,54306,54314,54323,54304,54318,54313,54311,54323,54287,54308,54309,54310,54331,54316,54307,54301,54315,54332,54295,54313,54285,54303,54309,54305,54294,54263,54269,54263,54226,54211,54195,54179,54183,54156,54160,54117,54136,54149,54130,54141,54141,54152,54173,54156,54172,54161,54175,54226,54182,54199,54201,54202,54234,54211,54212,54217,54232,54255,54222,54235,54232,54230,54267,54233,54253,54246,54239,54272,54248,54267,54245,54254,54285,54259,54267,54266,54268,54295,54268,54273,54272,54269,54291,54272,54278,54270,54273,54294,54264,54283,54277,54268,54290,54256,54278,54265,54271,54291,54269,54279,54261,54262,54294,54270,54275,54267,54276,54306,54281,54289,54274,54286,54299,54280,54281,54274,54273,54288,54281,54286,54275,54282,54312,54264,54288,54275,54286,54303,54283,54291,54275,54278,54290,54275,54298,54283,54300,54288,54281,54298,54278,54270,54306,54283,54287,54281,54276,54298,54283,54304,54288,54279,54294,54275,54294,54275,54258,54277,54273,54294,54271,54278,54270,54264,54267,54263,54263,54272,54254,54259,54251,54257,54265,54249,54255,54230,54238,54248,54238,54232,54223,54226,54218,54193,54200,54171,54194,54181,54173,54183,54163,54204,54191,54191,54212,54207,54210,54214,54221,54238,54217,54216,54237,54235,54224,54231,54229,54241,54235,54244,54232,54233,54235,54236,54237,54248,54233,54236,54250,54250,54243,54240,54244,54243,54274,54246,54253,54252,54248,54258,54246,54247,54257,54270,54266,54250,54259,54249,54256,54270,54247,54258,54256,54262,54267,54252,54266,54261,54266,54279,54247,54270,54272,54269,54297,54260,54271,54283,54264,54287,54263,54290,54271,54277,54292,54271,54272,54263,54264,54276,54274,54277,54257,54270,54284,54260,54267,54244,54259,54276,54254,54271,54255,54260,54283,54269,54280,54258,54270,54274,54266,54269,54258,54274,54270,54260,54280,54265,54274,54278,54265,54264,54274,54250,54268,54259,54266,54267,54261,54292,54276,54280,54265,54261,54304,54273,54277,54272,54262,54292,54270,54279,54263,54268,54259,54231,54241,54194,54190,54197,54156,54162,54143,54145,54157,54136,54146,54147,54171,54184,54163,54188,54182,54202,54219,54191,54216,54210,54213,54229,54223,54219,54224,54224,54257,54247,54261,54223,54236,54255,54254,54232,54234,54239,54285,54250,54269,54251,54238,54269,54260,54265,54261,54259,54278,54260,54277,54249,54255,54271,54256,54265,54254,54265,54278,54257,54278,54266,54271,54277,54260,54276,54255,54279,54302,54299,54294,54299,54306,54325,54315,54325,54320,54319,54359,54338,54342,54347,54346,54366,54329,54346,54339,54344,54368,54337,54360,54337,54340,54374,54332,54349,54326,54338,54364,54338,54350,54329,54340,54360,54332,54354,54337,54327,54346,54325,54346,54329,54346,54368,54346,54349,54331,54331,54345,54343,54336,54342,54328,54360,54332,54334,54338,54346,54340,54334,54343,54316,54342,54359,54332,54344,54319,54335,54357,54342,54358,54341,54330,54352,54333,54329,54314,54329,54320,54302,54319,54298,54279,54291,54261,54269,54232,54248,54247,54226,54250,54236,54247,54265,54245,54287,54263,54264,54285,54281,54302,54306,54294,54311,54299,54307,54305,54307,54298,54314,54325,54307,54317,54329,54312,54321,54308,54321,54319,54322,54329,54303,54341,54322,54334,54338,54333,54326,54324,54334,54335,54336,54327,54335,54327,54339,54316,54333,54325,54327,54342,54317,54336,54336,54343,54347,54324,54349,54348,54345,54357,54340,54344,54337,54364,54373,54349,54342,54338,54332,54359,54346,54351,54340,54350,54366,54345,54361,54355,54358,54366,54348,54349,54340,54331,54361,54345,54360,54333,54348,54373,54349,54350,54350,54330,54361,54335,54341,54331,54352,54361,54341,54328,54343,54346,54365,54330,54348,54331,54340,54363,54334,54350,54334,54347,54362,54336,54343,54337,54346,54354,54336,54327,54323,54351,54358,54331,54347,54337,54345,54366,54333,54328,54324,54336,54370,54324,54346,54328,54330,54349,54321,54327,54298,54293,54311,54262,54255,54229,54243,54243,54232,54238,54226,54226,54268,54237,54232};

//Variables utiles pour le traitement (à supprimer si ça ne sert pas à Ariane)
volatile uint32_t AC_RED = 0; 
volatile uint32_t DC_RED = 0; 
volatile int HR_RED = 0;
volatile uint32_t AC_IR = 0; 
volatile uint32_t DC_IR = 0; 
volatile int HR_IR = 0;




//Tâche principale
void Task_principal(void)
{
    
    Start_Oxymeter(); //Démarrage de l'oxymètre
    drawWaiting(); //Écran d'attente 

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
            
           //On ferme les LEDS bleue et rouge (fin d'échantillonnage)
           Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);
           Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);
     
            if(!FLAG_menu)
            {
                //2-Traitement (Ariane)
                
              
                
                //3 - Affichage de la courbe (Andréa)
                drawAffichageCourbe();
            
                //Vérification des alertes physiologiques 
                if(HR_max_alarm<HR || HR_min_alarm>HR || SPO2_min_alarm > SPO2)
                {
                    Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,0);
                    //Cy_GPIO_Write(GreenLED_0_PORT,GreenLED_0_NUM,1);
                }
                else
                {
                    Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);  
                    //Cy_GPIO_Write(GreenLED_0_PORT,GreenLED_0_NUM,0);
                }
            
            }
            
   
        }
        else
        {
            //On remet la LED verte dans le menu et on éteint les deux autres
            Cy_GPIO_Write(RedLED_0_PORT,RedLED_0_NUM,1);
            Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);
            //Cy_GPIO_Write(GreenLED_0_PORT,RedLED_0_NUM,0); 
            
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
    
    xTaskCreate(Task_principal, "Task Principal", 8000, NULL,configMAX_PRIORITIES - 1, NULL);
	vTaskStartScheduler();

    for(;;)
    {
       
    }
}

/* [] END OF FILE */
