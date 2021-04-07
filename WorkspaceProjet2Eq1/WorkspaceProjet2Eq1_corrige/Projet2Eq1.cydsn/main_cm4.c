/******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.2
*
* Description: This file main application code for the CE223727 EmWin Graphics
*				library EInk Display 

********************************************************************************
*************MODIFIED by Christophe Cloutier-Tremblay.**************************
********************************************************************************
* 
*
* Hardware Dependency: CY8CKIT-028-EPD E-Ink Display Shield
*					   CY8CKIT-062-BLE PSoC6 BLE Pioneer Kit
*
******************************************************************************* 
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved. 
******************************************************************************* 
* This software, including source code, documentation and related materials 
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its 
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent 
* protection (United States and foreign), United States copyright laws and 
* international treaty provisions. Therefore, you may use this Software only 
* as provided in the license agreement accompanying the software package from 
* which you obtained this Software (“EULA”). 
* 
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive, 
* non-transferable license to copy, modify, and compile the Software source 
* code solely for use in connection with Cypress’s integrated circuit products. 
* Any reproduction, modification, translation, compilation, or representation 
* of this Software except as specified above is prohibited without the express 
* written permission of Cypress. 
* 
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
********************************************************************************/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include <math.h>
#include "interface.h"
#include "project.h" 
#include "FreeRTOS.h"
#include "task.h" 
#include "queue.h"
//#include "params.h"

// Variables globales
volatile int    compteur = 0;
volatile float  bufferCirculaire[4000];
volatile int    indexEcriture = 0;
volatile int    processBufferIdx = 0;
volatile bool   processBufferFlag = false; 
volatile int bonneLongueur = 0;



/* POUR PROJET :

uint16_t *ptrSignal = &signalCourt[0];

// graphe du signal : s'affiche sur 100x250 (y,x) mais je le veux pas direct en haut... Espace
// de 20 pixels environ à partir du top avant de commencer dessin



Attention,drawGraph prend des uint16, donc convertir le 32 en 16
avec bitshift par exemple et somme des 2 ensuite :

uint terme1 = []
uint16_t terme1_1 = terme1 & 00000000000000001111111111111111
uint16_t terme1_2 = terme1 & 11111111111111110000000000000000

Créer une fonction qui fait le graphique en prenant en paramètre un uint
avec drawLine entre 2 pts


void drawGraph(uint* pointeur)
{
    
    for (int i=0; i<=124; i++)
    {
        int x0 = 2*(i);
        int x1 = 2*(i+1);
        int y0 = (int) (pointeur[i]*50)+64;
        int y1 = (int) (pointeur[i+1]*50)+64;
        GUI_DrawLine (x0, y0, x1, y1);
    }
    
}

*/

// fonction pour afficher HR et SpO2

//
//// PARTIE 1, code 1.2
//void updateParameters(int param1, int param2){
//
//    char str[100];
//    sprintf(str,"DutyCycle: %d %%       Moyenne: %d mV",param1, param2);
//    
//    GUI_DispStringHCenterAt(str, 129, 160);
//    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
//}

//// PARTIE 3, code 3.2 et 3.3
//void adc_handler(){
//    bufferCirculaire[indexEcriture] = ADC_GetResult16(0);
//    indexEcriture=indexEcriture+1;
//    if(indexEcriture==2000){
//        processBufferFlag=true;
//        processBufferIdx = indexEcriture;
//    }
//    else if(indexEcriture==4000){
//        processBufferFlag=true;
//        processBufferIdx = indexEcriture;
//        
//        indexEcriture=0;
//    }
//    
//    NVIC_ClearPendingIRQ(adc_int_cfg.intrSrc);
//}
//
//// PARTIE 3, code 3.4 et 3.5
//void traitementSignal(){
//
//    compteur++;  
//    
//    double somme=0;
//    double resultatConvertit=0;
//    int i = 2000;
//    
//    while(i>0){
//        resultatConvertit=Cy_SAR_CountsTo_mVolts(SAR, 0 , bufferCirculaire[i--]);
//        somme += resultatConvertit; 
//    }
//    double moyenne = somme/2000;
//    double dutyCycle = moyenne*100/3300; 
//    
//    int moyenne_int = round(moyenne);
//    int dutyCycle_int = round(dutyCycle);
//    
//    if(compteur==3){
//        GUI_SetPenSize(1);
//        GUI_SetColor(GUI_BLACK);
//        GUI_SetBkColor(GUI_WHITE);
//        GUI_Clear();
//        
//        GUI_DrawRect(7, 5, 257, 155);
//        
//        int x = dutyCycle_int*2+17;
//        
//        GUI_DrawLine(17,145,17,15);
//        GUI_DrawLine(17,15,x,15);
//        GUI_DrawLine(x,15,x,145);
//        GUI_DrawLine(x,145,217,145);
//        
//        ADC_Stop();
//        updateParameters(dutyCycle_int,moyenne_int);
//        ADC_Start();
//        
//        compteur = 0;
//    }
//    
//    processBufferFlag = false;
//
//}

/*******************************************************************************
* Function Name: int main(void)
********************************************************************************
*
* Summary: À Remplir
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int main(void)
{
   
    xTaskCreate(interface,"Tache_main",1000,NULL,3, NULL);  
    vTaskStartScheduler();
    
//    __enable_irq(); /* Enable global interrupts. */
//    
//    /* Initialize emWin Graphics */
//    GUI_Init();
//
//    /* Start the eInk display interface and turn on the display power */
//    Cy_EINK_Start(20);
//    Cy_EINK_Power(1);
//    
//    GUI_SetPenSize(1);
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetBkColor(GUI_WHITE);
//    GUI_Clear();
//    
//    //Draw a rectangle
//    GUI_DrawRect(1,1,263,175);
//    
//    //Draw a line
//    GUI_SetPenSize(4);
//    GUI_DrawLine(20,65,243,65);
//    
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetBkColor(GUI_WHITE);
//    
//   //Write a title and subtile	
//    GUI_SetFont(GUI_FONT_32B_1);
//    GUI_SetTextAlign(GUI_TA_CENTER);
//    GUI_DispStringAt("GBM2100", 132, 30);
//    GUI_SetFont(GUI_FONT_16_1);
//    GUI_DispStringAt("Laboratoire 3", 132, 70);
//    
//    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
//    
//    // Test du code partie 1 
//    GUI_Clear();                 
//    drawGraph(vectorSinus);      
//    updateParameters(10,1666); 
//    GUI_Clear(); 
//    
//    // Démarrage du module PWM (LED clignote) pour la partie 2
//    PWM_Start();
//    
//    // Démarrage de l'ADC pour la partie 3
//    Cy_SysInt_Init(&adc_int_cfg,adc_handler);   // Init reference
//    NVIC_EnableIRQ(adc_int_cfg.intrSrc);        // Enable interrupt
//    ADC_Start();
//    ADC_StartConvert();
//    
//    for(;;)
//    {
//        // Vérification du flag et traitement si activé (PARTIE 3, code 3.3)
//        if(processBufferFlag==true){
//            traitementSignal();
//        }
//    }
//}
}
/* [] END OF FILE */
