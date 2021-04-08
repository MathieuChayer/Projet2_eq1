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
#include "string.h"
#include "task.h"
#include <stdio.h>


/* Image buffer cache */
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

volatile uint32_t IR_data[2000]; //Buffer de la LED IR
volatile uint32_t RED_data[2000]; //Buffer de la LED RED
volatile uint8_t data = 0; //Variable temporaire de lecture/éc

//FLAGS

int FLAG_mesure = 1; //Flag qui indique que l'utilisateur est en mode utilisation!

//Tâche principale
void Task_principal(void)
{
    
    Start_Oxymeter();
    
    for(;;)
	{  
        if(FLAG_mesure)
        {
            MAX_ReadFIFO(&IR_data, &RED_data); //Lecture du FIFO --> 1000 samples@200sps = 5 secs
            
            //Affichage des données dans TERA
            for (int i=0;i<2000;i++)
            {
                printf("RED : %lu   IR : %lu\r\n",RED_data[i],IR_data[i]);
                //printf("%lu \r\n",RED_data[i]);
                //printf("%lu \r\n",IR_data[i]);
            }

            //Remettre la LED verte et fermer la LED rouge éventuellement (prochain cycle)
            vTaskDelay(1000);
            Cy_GPIO_Write(WORKING_LED_0_PORT,WORKING_LED_0_NUM,0);
            Cy_GPIO_Write(ERROR_LED_0_PORT,ERROR_LED_0_NUM,1);
        }
	}
}




/*******************************************************************************
* Function Name: void UpdateDisplay(void)
********************************************************************************
*
* Summary: This function updates the display with the data in the display 
*			buffer.  The function first transfers the content of the EmWin
*			display buffer to the primary EInk display buffer.  Then it calls
*			the Cy_EINK_ShowFrame function to update the display, and then
*			it copies the EmWin display buffer to the Eink display cache buffer
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  It takes about a second to refresh the display.  This is a blocking function
*  and only returns after the display refresh
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
* Function Name: void WaitforSwitchPressAndRelease(void)
********************************************************************************
*
* Summary: This implements a simple "Wait for button press and release"
*			function.  It first waits for the button to be pressed and then
*			waits for the button to be released.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function and exits only on a button press and release
*
*******************************************************************************/
void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while(Status_SW2_Read() != 0);
    
    /* Wait for SW2 to be released */
    while(Status_SW2_Read() == 0);
}

// PARTIE 1, code 1.1
void drawGraph(float *vector150elements){
    GUI_SetPenSize(1);
    GUI_DrawRect(7, 5, 257, 155);
    int x1 = 7;
    int y1 = 0;
    int x2 = 9;
    int y2 = 0;
    for(uint i=0;i<125;i++){
        y1 = (vector150elements[i]*75)+79;
        y2 = (vector150elements[i+1]*75)+79;       
        GUI_DrawLine(x1,y1,x2,y2);
        x1 = x2;
        x2 = x2 + 2;
    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// PARTIE 1, code 1.2
void updateParameters(int param1, int param2){

    char str[100];
    sprintf(str,"DutyCycle: %d %%       Moyenne: %d mV",param1, param2);
    
    GUI_DispStringHCenterAt(str, 129, 160);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

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
    
    __enable_irq(); /* Enable global interrupts. */
    
    /* Clear any pending interrupts */
    NVIC_DisableIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
    Cy_GPIO_ClearInterrupt(Pin_AnyMotion_INT_PORT, Pin_AnyMotion_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
        
    
    UART_Start(); 
    Cy_GPIO_Write(MAX_power_0_PORT,MAX_power_0_NUM, 1); //Alimentation du MAX30102
    printf("___Communication I2C__\r\n");
    
    for(int i =1;i<6;i++)
    {
        printf("%d\r\n",i);
        Cy_SysLib_Delay(1000);   
    }
    
    
    /* Initialize emWin Graphics */
    GUI_Init();

    /* Start the eInk display interface and turn on the display power */
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    ClearScreen();
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    
    //Tâche principale I2C
    xTaskCreate(Task_principal, "Task Principal", configMINIMAL_STACK_SIZE, NULL,configMAX_PRIORITIES - 1, 0);
	vTaskStartScheduler();

    for(;;)
    {
       
    }
}

/* [] END OF FILE */
