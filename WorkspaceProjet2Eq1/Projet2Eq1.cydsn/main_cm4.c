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
#include "string.h"
#include "task.h"
#include <stdio.h>


/* Image buffer cache */
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

/* Semaphore handle pour I2C avec MAX */
SemaphoreHandle_t xSemaphoreI2C_MAX;   

/* Taille du buffer I2C */
#define I2C_BUFFER_SIZE (10u)

//Fonction de lecture I2C pour le capteur MAX
unsigned int MAX_ReadBytes(uint8_t RegisterAddr, uint8_t *RegisterValue)
{
	/* Variable used for status of I2C operation */
	static unsigned int status;
		
	/* Local variables for storing I2C Master transfer configuration structure */
	cy_stc_scb_i2c_master_xfer_config_t config;
		
	config.slaveAddress = (uint32)MAX_ADDRESS;
	config.buffer       = &RegisterAddr;
	config.bufferSize   = 1;
	config.xferPending  = true;
		
	/* Start I2C write and take the semaphore */
	status = (unsigned int)MAX_I2C_MasterWrite(&config);
	xSemaphoreTake(xSemaphoreI2C_MAX, portMAX_DELAY);
		
	if(status == CY_SCB_I2C_SUCCESS)
	{
		config.slaveAddress = (uint32)MAX_ADDRESS;
		config.buffer =  RegisterValue;
		config.bufferSize = 1;
		config.xferPending = false;
			
		/* Start I2C read and take the semaphore */
		status = (unsigned int)MAX_I2C_MasterRead(&config);
		xSemaphoreTake(xSemaphoreI2C_MAX, portMAX_DELAY);
	}
	return status;
}

//Fonction d'écriture I2C pour le capteur MAX
unsigned int MAX_WriteBytes(uint8_t RegisterAddr, uint8_t *RegisterValue)
{
    /* Variable used for status of I2C operation */
    unsigned int status;
    
    /* Temporary buffer used for I2C transfer */ 
    static uint8_t tempBuff[I2C_BUFFER_SIZE];
    tempBuff[0] = RegisterAddr;
    memcpy(tempBuff+1, RegisterValue, 1);

    /* Local variables for storing I2C Master transfer configuration structure */
    cy_stc_scb_i2c_master_xfer_config_t config = 
    {  
        .slaveAddress = (uint32)MAX_ADDRESS,
        .buffer =  tempBuff,
        .bufferSize = 2,
        .xferPending = false
    };
    
    /* Start I2C write and take the semaphore */
    status = (unsigned int)MAX_I2C_MasterWrite(&config);
    xSemaphoreTake(xSemaphoreI2C_MAX, portMAX_DELAY);
        
    return status;
   
}

//Fonction qui assure le bon déroulement de la fin d'une écriture ou lecture I2C pour MAX
void MAX_I2C_Callback(uint32_t events)
{
	BaseType_t xHigherPriorityTaskWoken;
	/**
	 * Unblock the task by releasing the semaphore only if no hardware error is 
	 * detected and I2C master read or write is completed
	 */
	if(events & CY_SCB_I2C_MASTER_ERR_EVENT)
	{
		printf("Failure!  : I2C hardware error detected\r\n");
         
		CY_ASSERT(0u); /* Halt CPU */
	}
	else if((events & CY_SCB_I2C_MASTER_WR_CMPLT_EVENT)|| 
			(events & CY_SCB_I2C_MASTER_RD_CMPLT_EVENT))
	{
		xSemaphoreGiveFromISR(xSemaphoreI2C_MAX, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
//Fonction qui remet à 0 le capteur
void MAX_reset()
{
    MAX_WriteBytes(REG_MODE_CONFIG, 0x40);
}

//Initialisation des registres du capteur
void MAX_init(void)
{
    uint8_t value = 0;
    
    value = 0x00;
    MAX_WriteBytes(REG_INTR_ENABLE_1,&value); // INTR setting
    
    value = 0x00;
    MAX_WriteBytes(REG_INTR_ENABLE_2,&value); // INTR setting
    
    value = 0x00;
    MAX_WriteBytes(REG_FIFO_WR_PTR,&value); //FIFO_WR_PTR[4:0]
    
    value = 0x00;
    MAX_WriteBytes(REG_OVF_COUNTER,&value); //OVF_COUNTER[4:0]
    
    value = 0x00;
    MAX_WriteBytes(REG_FIFO_RD_PTR,&value); //FIFO_RD_PTR[4:0]
    
    value = 0x0F;
    MAX_WriteBytes(REG_FIFO_CONFIG,&value); //sample avg = 1, fifo rollover=false, fifo almost full = 17
    
    value = 0x03;
    MAX_WriteBytes(REG_MODE_CONFIG,&value); //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
    
    value = 0x27;
    MAX_WriteBytes(REG_SPO2_CONFIG,&value); // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
    
    value = 0x24;
    MAX_WriteBytes(REG_LED1_PA,&value); //Choose value for ~ 7mA for LED1
    
    value = 0x24;
    MAX_WriteBytes(REG_LED2_PA,&value); //Choose value for ~ 7mA for LED2
    
}

void MAX_ReadFIFO(void)
{
  
}
//Fonction pour ajuster le courant dans les 2 LEDs
void set_LED_current(current_LED1,current_LED2)
{
    MAX_WriteBytes(REG_LED1_PA,&current_LED1);
    MAX_WriteBytes(REG_LED2_PA,&current_LED2);
    
    // De 0 à 51 mA (0x00 à 0xFF) par bonds de 0.2 mA
}
//Tâche principale
void Task_Read(void* pvParameters)
{
	
	(void)pvParameters; /* Remove warning for unused parameter */
    
	MAX_I2C_Start();  //Démarrer I2C du MAX
    MAX_I2C_RegisterEventCallback(MAX_I2C_Callback); 
	xSemaphoreI2C_MAX = xSemaphoreCreateBinary(); 
	
    uint8_t data = 0;
    
    //Vérifier si le capteur est sous-tension
    if(Cy_GPIO_ReadOut(MAX_power_0_PORT,MAX_power_0_NUM) == 0)
    {
        printf("Le capteur n'est pas alimenté \r\n");
    }
    
    MAX_reset(); //Remise à 0 du capteur
    vTaskDelay(500);  //Délais 
    
    //Lecture de la valeur initiale du courant dans la LED2.
    MAX_ReadBytes(REG_LED2_PA,&data);
    printf("Avant : Le contenu du registre REG_LED2_PA du MAX30102 : %p \r\n",data);
     
    MAX_ReadBytes(REG_INTR_STATUS_1,&data); //Clear interrupt (power ready)
    MAX_ReadBytes(REG_INTR_STATUS_2,&data); //Clear INT 2
    
    MAX_init(); //Initialisation des registres
    
    //Lecture de la valeur du courant LED2 après initialisation.
    MAX_ReadBytes(REG_LED2_PA,&data);
    printf("Apres : Le contenu du registre REG_LED2_PA du MAX30102 : %p \r\n",data);
    
    //Déterminer le nombre d'échantillons disponibles
    uint8_t WR_PTR = 0;
    uint8_t RD_PTR = 0;
    for(;;)
	{  
    
        MAX_ReadBytes(REG_FIFO_WR_PTR,&WR_PTR);
        MAX_ReadBytes(REG_FIFO_RD_PTR,&RD_PTR);
        printf("Number of samples : %d \r\n",WR_PTR-RD_PTR);
        vTaskDelay(1000);

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
    
    UART_Start(); 
    Cy_GPIO_Write(MAX_power_0_PORT,MAX_power_0_NUM, 1); //Alimentation du MAX30102
    printf("___Communication I2C__\r\n");
    
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
    xTaskCreate(Task_Read, "Read Task", configMINIMAL_STACK_SIZE, NULL,configMAX_PRIORITIES - 1, 0);
	vTaskStartScheduler();

    for(;;)
    {
       
    }
}

/* [] END OF FILE */
