/* ========================================
 * interface.c
 * 
 * Auteur : Andréa
 * ========================================
*/
#include "interface.h"


volatile int SPO2 = 0;
volatile int HR = 0;
volatile int Current_LED1 = 0x20; 
volatile int Current_LED2 = 0x20; 
volatile int HR_min_alarm = 40; 
volatile int HR_max_alarm = 100;
volatile int SPO2_min_alarm = 90;

uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

/*******************************************************************************
* Function Name: void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
********************************************************************************
*
* Summary: This function updates the eink display with the buffer
*
* Parameters:
*  cy_eink_update_t updateMethod
*  bool powerCycle
*
* Return:
*  None
*
*******************************************************************************/
void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
{
    cy_eink_frame_t* pEmwinBuffer;

    /* Get the pointer to Emwin's display buffer */
    pEmwinBuffer = (cy_eink_frame_t*)LCD_GetDisplayBuffer();

    /* Update the EInk display */
    Cy_EINK_ShowFrame(imageBufferCache, pEmwinBuffer, updateMethod, powerCycle);

    /* Copy the EmWin display buffer to the imageBuffer cache*/
    memcpy(imageBufferCache, pEmwinBuffer, CY_EINK_FRAME_SIZE);
}

/*******************************************************************************
* Function Name: void ClearScreen(void)
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

/*******************************************************************************
* Function Name: void DrawSignal(uint32_t *signal)
********************************************************************************
*
* Summary: Fonction de dessin du signal
*
* Parameters:
*  uint32_t *signal
*
* Return:
*  None
*
*******************************************************************************/
void DrawSignal(uint32_t *signal)
{
    //Espace du graphique : GUI_DrawRect(7,28,257,128) --> 250x100
    
    //Max et min du signal
    uint32_t max = 0;
    uint32_t min = 300000;

    for(int i = 0; i < 2000; i++)
    {
      if(max< signal[i])
      {
         max = signal[i];
      }
      if(min > signal[i])
      {
         min = signal[i];
      }
    }
    
    //On affiche un échantillon sur 8 
    int k = 8;
    for (int j = 0; j < 249; j++)
    {
       GUI_DrawLine(j+7,124 - (124-32)*(signal[k-8]-min)/(max-min),j+8,124 - (124-32)*(signal[k]-min)/(max-min));
       k += 8;
    }   
}

/*******************************************************************************
* Function Name: void drawWaiting(void)
********************************************************************************
*
* Summary: Affichage d'un écran d'attente
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void drawWaiting(void)
{
  
    GUI_Clear();
    GUI_SetFont(GUI_FONT_13B_1);
    GUI_SetPenSize(2);
    
    GUI_DrawRect(42,4,222,24);  // Nature
    GUI_DispStringAt("Courbe :", 70, 8);
    GUI_DispStringAt("R", 140, 8);
    GUI_DispStringAt("IR", 200, 8);
    
    GUI_DrawRect(7,28,257,128); //graphique
    GUI_DispStringAt("Mettez votre doigt sur le capteur ", 20,50);
    GUI_DispStringAt("Veuillez patienter....", 20, 90);
    
    GUI_DrawRect(7,132,132,172); //HR
    GUI_DispStringAt("HR (BPM) : ", 36, 138);
    
    GUI_DrawRect(137,132,257,172); //SPO2
    GUI_DispStringAt("SpO2 (%) : ", 167, 138);
       
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);

}

/*******************************************************************************
* Function Name: void drawAffichageCourbe(void)
********************************************************************************
*
* Summary: Affichage de la courbe choisie et des paramètres vitaux 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void drawAffichageCourbe(void)
{
    
    GUI_Clear();
    GUI_SetFont(GUI_FONT_13B_1);
    GUI_SetPenSize(2);
    
    GUI_DrawRect(42,4,222,24);  // Nature
    GUI_DispStringAt("Courbe :", 70, 8);
    GUI_DispStringAt("R", 140, 8);
    GUI_DispStringAt("IR", 200, 8);
   
    // Sélecteur de courbe
    if(FLAG_RED ==1)
    {
        GUI_FillRect(127,10,135,18);
        GUI_DrawRect(187,10,195,18);
        
        DrawSignal(&RED_data);// Affichage du signal non-filtré R
        
    }
    else
    {
        GUI_DrawRect(127,10,135,18);
        GUI_FillRect(187,10,195,18);
        DrawSignal(&IR_data); // Affichage du signal non-filtré IR
    }
    
    GUI_DrawRect(7,28,257,128); // graphique
    
    GUI_DrawRect(7,132,132,172); // HR
    GUI_DispStringAt("HR (BPM) : ", 36, 138);
    
    GUI_DrawRect(137,132,257,172); // SPO2
    GUI_DispStringAt("SpO2 (%) : ", 167, 138);
        
    // Valeur des paramètres
    GUI_SetFont(GUI_FONT_13_1);
    char str_param[40];
    sprintf(str_param,"%d",HR);
    GUI_DispStringAt(str_param, 36, 154);
    sprintf(str_param,"%d",SPO2);
    GUI_DispStringAt(str_param, 167, 154);
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

/*******************************************************************************
* Function Name:void draw_MenuPrincipal(void)
********************************************************************************
*
* Summary: Affichage du menu principal
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void draw_MenuPrincipal(void)
{
    GUI_Clear();
    
    GUI_SetPenSize(2);
    GUI_SetFont(GUI_FONT_13B_1);
    
    GUI_DrawRect(82,4,182,24);  // MENU
    GUI_DispStringAt("MENU", 118, 8);
    
    GUI_DrawRect(7,28,257,48);
    GUI_DrawRect(10,32,30,44);
    GUI_DispStringAt("Choix de la courbe (R/IR)", 60, 32);
   
    GUI_DrawRect(7,52,257,72);
    GUI_DrawRect(10,56,30,68);
    GUI_DispStringAt("Borne superieure HR", 60, 56);
    
    GUI_DrawRect(7,76,257,96);
    GUI_DrawRect(10,80,30,92);
    GUI_DispStringAt("Borne inferieure HR", 60, 80);
    
    GUI_DrawRect(7,100,257,120);
    GUI_DrawRect(10,104,30,116);
    GUI_DispStringAt("Borne inferieure SPO2", 60, 104);
    
    GUI_DrawRect(7,124,257,144);
    GUI_DrawRect(10,128,30,140);
    GUI_DispStringAt("Courant de la DEL 1 (Rouge)", 60, 128);
    
    GUI_DrawRect(7,148,257,168);
    GUI_DrawRect(10,152,30,164);
    GUI_DispStringAt("Courant de la DEL 2 (infrarouge)", 60, 152);
    
    //Choix de l'option 
       
    switch (FLAG_option)
    {
        case 1:
        GUI_FillRect(10,32,30,44);
        break;

        case 2:
        GUI_FillRect(10,56,30,68);
        break;
        
        case 3:
        GUI_FillRect(10,80,30,92);
        break;
        
        case 4:
        GUI_FillRect(10,104,30,116);
        break;
        
        case 5:
        GUI_FillRect(10,128,30,140);
        break;
        
        case 6:
        GUI_FillRect(10,152,30,164);
        break;

    }

    UpdateDisplay(CY_EINK_FULL_4STAGE, true);

}

/*******************************************************************************
* Function Name:void draw_SousMenu(void)
********************************************************************************
*
* Summary: Affichage des sous-menus
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void draw_SousMenu(void)
{
    GUI_Clear();
    int button0_pressed =0;
    int button1_pressed =0;
    int slider_pos = 0xFFFF;
    
    switch (FLAG_menu)
    {
        case 2:
        
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Choix de la courbe (R/IR)", 50, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Changer | SW2 : Appliquer", 40, 34);
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DispStringAt("R", 105, 100);
            GUI_DispStringAt("IR", 175, 100);
      
            if(FLAG_RED ==1)
            {
                GUI_FillRect(92,102,100,110);
                GUI_DrawRect(162,102,170,110);
            }
            else
            {
                GUI_DrawRect(92,102,100,110);
                GUI_FillRect(162,102,170,110);
            }

            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
          
            while(button0_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID); 
                }  
            } 
            
            if (button0_pressed)
            {
                FLAG_RED = !FLAG_RED;
            }
       
        break;
                
        case 3:
            
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Borne superieure HR", 55, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
          
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,95,232,110);
            GUI_DispStringAt("60 BPM", 28, 80);
            GUI_DispStringAt("120 BPM", 210, 80);
            GUI_FillRect(32,95,32+(HR_max_alarm-60)*200/60,110);
            
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DrawRect(80,130,190,160);
            
            char HR_max_alarm_str[20];
            sprintf(HR_max_alarm_str,"%d",HR_max_alarm);
            GUI_DispStringAt(HR_max_alarm_str, 120 , 135);
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                HR_max_alarm -=1; 
            }
            if (button1_pressed)
            {
                HR_max_alarm +=1; 
            }
            
            if (slider_pos < 0xFFFF)
            {
                HR_max_alarm = 60 + slider_pos*60/100;
            }
            
            if(HR_max_alarm > 120)
            {
            HR_max_alarm = 120;
            }
            
            if(HR_max_alarm < 60)
            {
            HR_max_alarm = 60;
            }
      
        break;
                
        case 4:
            
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Borne inferieure HR", 55, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
          
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,95,232,110);
            GUI_DispStringAt("30 BPM", 28, 80);
            GUI_DispStringAt("60 BPM", 210, 80);
            GUI_FillRect(32,95,32+(HR_min_alarm-30)*200/30,110);
            
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DrawRect(80,130,190,160);
            
            char HR_min_alarm_str[20];
            sprintf(HR_min_alarm_str,"%d",HR_min_alarm);
            GUI_DispStringAt(HR_min_alarm_str, 120 , 135);
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                HR_min_alarm -=1; 
            }
            if (button1_pressed)
            {
                HR_min_alarm +=1; 
            }
            
            if (slider_pos < 0xFFFF)
            {
                HR_min_alarm = 10 + slider_pos*30/100;
            }
            
            if(HR_min_alarm > 60)
            {
                HR_min_alarm = 60;
            }
            
            if(HR_min_alarm < 30)
            {
                HR_min_alarm = 30;
            }
      
        break;
                
        case 5:
            
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Borne inferieure SPO2", 55, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
          
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,95,232,110);
            GUI_DispStringAt("0 %", 28, 80);
            GUI_DispStringAt("100 %", 210, 80);
            GUI_FillRect(32,95,32+(SPO2_min_alarm)*200/100,110);
            
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DrawRect(80,130,190,160);
            
            char SPO2_min_alarm_str[20];
            sprintf(SPO2_min_alarm_str,"%d",SPO2_min_alarm);
            GUI_DispStringAt(SPO2_min_alarm_str, 120 , 135);
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                SPO2_min_alarm -=1; 
            }
            if (button1_pressed)
            {
                SPO2_min_alarm +=1; 
            }
            
            if (slider_pos < 0xFFFF)
            {
                SPO2_min_alarm = slider_pos;
            }
            
            if(SPO2_min_alarm > 100)
            {
                SPO2_min_alarm = 100;
            }
            
            if(SPO2_min_alarm < 0)
            {
                SPO2_min_alarm = 0;
            }
      
       
        break;
                
        case 6:
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Courant de la DEL 1 (Rouge)", 40, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
       
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,110,232,130);
            GUI_DispStringAt("0", 28, 90);
            GUI_DispStringAt("255", 222, 90);
            
            char str_Current_LED1[10];
            sprintf(str_Current_LED1,"%d / 255",Current_LED1);
            GUI_DispStringAt(str_Current_LED1, 106, 145);;
            GUI_DrawRect(100,134,164,170);
            
            GUI_FillRect(32,110,32+200*Current_LED1/0xFF,130);
   
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
      
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                Current_LED1 -=0x01; 
            }
            if (button1_pressed)
            {
                Current_LED1 +=0x01; 
            }
            if (slider_pos < 0xFFFF)
            {
                Current_LED1 = slider_pos*0xFF/100;
            }
            
            if (Current_LED1 > 0xFF)
            {
                Current_LED1 = 0xFF;
            }
            
            if (Current_LED1 < 0x00)
            {
                Current_LED1 = 0x00;
            }

        break;
        
        case 7:
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Courant de la DEL 2 (infrarouge)", 32, 8);
             GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
       
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,110,232,130);
            GUI_DispStringAt("0", 28, 90);
            GUI_DispStringAt("255", 222, 90);
            
            char str_Current_LED2[10];
            sprintf(str_Current_LED2,"%d / 255",Current_LED2);
            GUI_DispStringAt(str_Current_LED2, 106, 145);
            GUI_DrawRect(100,134,164,170);
            
            GUI_FillRect(32,110,32+200*Current_LED2/0xFF,130);
   
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                Current_LED2 -=0x01; 
            }
            if (button1_pressed)
            {
                Current_LED2 +=0x01;
            }
            if (slider_pos < 0xFFFF)
            {
                Current_LED2 = slider_pos*0xFF/100;
            }
            if (Current_LED2 > 0xFF) // Empêche le courant d'être plus grand que 0xFF
            {
                Current_LED2 = 0xFF;
            }
            if (Current_LED2 < 0x00) // Empêche le courant d'être négatif
            {
                Current_LED2 = 0x00;
            }
              
        break;

    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);


}

/*******************************************************************************
* Function Name:void isr_SW2(void)
********************************************************************************
*
* Summary: Routine d'interruption du bouton SW2
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void isr_SW2(void)
{    
  
    Cy_GPIO_ClearInterrupt(SW2_0_PORT,SW2_0_NUM);
    NVIC_ClearPendingIRQ(SW2_isr_cfg.intrSrc);
    
    Cy_SysLib_Delay(100); // Délai pour debouncing
    
    FLAG_modif = FLAG_menu; // On garde en mémoire le FLAG_menu
    
    FLAG_menu = !FLAG_menu; // On inverse le FLAG menu
    
    Cy_GPIO_ClearInterrupt(SW2_0_PORT,SW2_0_NUM);
    NVIC_ClearPendingIRQ(SW2_isr_cfg.intrSrc);

}

/* [] END OF FILE */
