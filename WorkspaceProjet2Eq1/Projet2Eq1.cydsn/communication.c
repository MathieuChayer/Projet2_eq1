/* ========================================
 * Communication.c
 * 
 * Auteur : Mathieu
 * ========================================
*/
#include "communication.h"


extern SemaphoreHandle_t xSemaphoreI2C_MAX; 
extern SemaphoreHandle_t xSemaphoreI2C_BMI;

struct bmi160_dev sensor; //structure de configuration du BMI160

/*******************************************************************************
* Nom: AnyMotion_Interrupt()
********************************************************************************
* Résumé:
*  Routine d'interruption de détection du mouvement
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
void AnyMotion_Interrupt(void)
{
    Cy_SysLib_Delay(200);//Debounce
    
    //Désactiver l'interruption quand le capteur n'est pas en contexte de lecture!
    
    
    //Routine 
    printf("Mouvement detected! \r\n");
    Cy_GPIO_Write(WORKING_LED_0_PORT,WORKING_LED_0_NUM,1);
    Cy_GPIO_Write(ERROR_LED_0_PORT,ERROR_LED_0_NUM,0);
    
    /* Clear any pending interrupts */
    Cy_GPIO_ClearInterrupt(Pin_AnyMotion_INT_PORT, Pin_AnyMotion_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
}

/*******************************************************************************
* Nom: MotionSensor_ConfigAnyMotionIntr()
********************************************************************************
* Résumé:
*  Configuration de l'interruption AnyMotion du BMI160
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
void MotionSensor_ConfigAnyMotionIntr(void)
{
    /* Structure for storing interrupt configuration */
    struct bmi160_int_settg int_config;

    
    /* Map the step interrupt to INT1 pin */
    int_config.int_channel = BMI160_INT_CHANNEL_2;

    /* Select the Interrupt type Step Detector interrupt */
    int_config.int_type = BMI160_ACC_ANY_MOTION_INT; //Anymotion int
    
    /* Interrupt pin configuration */
    /* Enabling interrupt pins to act as output pin */
    int_config.int_pin_settg.output_en = BMI160_ENABLE;
    /*Choosing push-pull mode for interrupt pin */
    int_config.int_pin_settg.output_mode = BMI160_DISABLE;
    /* Choosing active High output */
    int_config.int_pin_settg.output_type = BMI160_ENABLE;
    /* Choosing edge triggered output */
    int_config.int_pin_settg.edge_ctrl = BMI160_ENABLE;
    /* Disabling interrupt pin to act as input */
    int_config.int_pin_settg.input_en = BMI160_DISABLE;
    /* 2.5 millisecond latched output */
    int_config.int_pin_settg.latch_dur =BMI160_LATCH_DUR_2_5_MILLI_SEC;
    
    /* Interrupt type configuration */
   // na - Select the Any Motion Interrupt parameter
    
   // na - 1- Enable the any-motion, 0- disable any-motion
   int_config.int_type_cfg.acc_any_motion_int.anymotion_en = BMI160_ENABLE; 
   // na - Enabling x-axis for any motion interrupt - monitor x axis 
   int_config.int_type_cfg.acc_any_motion_int.anymotion_x = BMI160_ENABLE; 
   // na - Enabling y-axis for any motion interrupt - monitor y axis 
   int_config.int_type_cfg.acc_any_motion_int.anymotion_y = BMI160_ENABLE; 
   // na - Enabling z-axis for any motion interrupt - monitor z axis
   int_config.int_type_cfg.acc_any_motion_int.anymotion_z = BMI160_ENABLE; 
    
   //***SENSIBILITÉ de l'intteruption***//
    
   // na - any-motion duration. This is the consecutive datapoints -> see datasheet pg32 section 2.6.1 <int_anym_dur> and pg78
   int_config.int_type_cfg.acc_any_motion_int.anymotion_dur = 2;            
   // na - An interrupt will be generated if the absolute value of two consecutive accelation signal exceeds the threshold value -> see datasheet pg32 section 2.6.1 <int_anym_th> and pg78 INT_MOTION[1] 
   int_config.int_type_cfg.acc_any_motion_int.anymotion_thr = 1;          
   // na - (2-g range) -> (anymotion_thr) * 3.91 mg, (4-g range) -> (anymotion_thr) * 7.81 mg, (8-g range) ->(anymotion_thr) * 15.63 mg, (16-g range) -> (anymotion_thr) * 31.25 mg

    
    /* Set the Step Detector interrupt */
    bmi160_set_int_config(&int_config, &sensor);
    
    /* Initialize and enable Orientation Interrupt*/
    Cy_SysInt_Init(&SysInt_AnyMotionINT_cfg, AnyMotion_Interrupt);
    NVIC_EnableIRQ(SysInt_AnyMotionINT_cfg.intrSrc);

}

/*******************************************************************************
* Nom: MotionSensor_Init()
********************************************************************************
* Résumé:
*  Configuration du capteur BMI160
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
void MotionSensor_Init(void)
{
    /* Sensor configurations */
    sensor.id           = BMI160_I2C_ADDR;
    sensor.interface    = BMI160_I2C_INTF;
    sensor.read         = (bmi160_com_fptr_t)BMI_ReadBytes;
    sensor.write        = (bmi160_com_fptr_t)BMI_WriteBytes;
    sensor.delay_ms     = vTaskDelay;
    
    /* Initialize BNI160 sensor */
    bmi160_init(&sensor);
        
    /* Select the Output data rate, range of accelerometer sensor */
    sensor.accel_cfg.odr    = BMI160_ACCEL_ODR_1600HZ;
    sensor.accel_cfg.range  = BMI160_ACCEL_RANGE_16G;
    sensor.accel_cfg.bw     = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Select the power mode of accelerometer sensor */
    sensor.accel_cfg.power  = BMI160_ACCEL_NORMAL_MODE;

    /* Select the Output data rate, range of Gyroscope sensor */
    sensor.gyro_cfg.odr     = BMI160_GYRO_ODR_3200HZ;
    sensor.gyro_cfg.range   = BMI160_GYRO_RANGE_2000_DPS;
    sensor.gyro_cfg.bw      = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    sensor.gyro_cfg.power   = BMI160_GYRO_NORMAL_MODE; 

    /* Set the sensor configuration */
    bmi160_set_sens_conf(&sensor);
    
}

/*******************************************************************************
* Nom: BMI_WriteBytes()
********************************************************************************
* Résumé:
*  Fonction d'écriture I2C pour le capteur BMI160
*
* Paramètres:
*  uint8_t Address         - Adresse I2C du capteur
*  uint8_t RegisterAddr    - Adresse du registre où écrire
*  uint8_t *RegisterValue  - Pointeur vers la variable où l'information à écrire est stockée
*  uint8_t Registerlen     - Nombre de bytes à lire
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
unsigned int BMI_WriteBytes(uint8_t Address, uint8_t RegisterAddr, uint8_t *RegisterValue, uint8_t RegisterLen)
{
    /* Variable used for status of I2C operation */
    unsigned int status;
    
    /* Temporary buffer used for I2C transfer */ 
    static uint8_t tempBuff[I2C_BUFFER_SIZE];
    tempBuff[0] = RegisterAddr;
    memcpy(tempBuff+1, RegisterValue, RegisterLen);

    /* Local variables for storing I2C Master transfer configuration structure */
    cy_stc_scb_i2c_master_xfer_config_t config = 
    {  
        .slaveAddress = (uint32)Address,
        .buffer =  tempBuff,
        .bufferSize = RegisterLen+1,
        .xferPending = false
    };
    
    /* Start I2C write and take the semaphore */
    status = (unsigned int)BMI_I2C_MasterWrite(&config);
    xSemaphoreTake(xSemaphoreI2C_BMI, portMAX_DELAY);
        
    return status;
}

/*******************************************************************************
* Nom: BMI_ReadBytes()
********************************************************************************
* Résumé:
*  Fonction de lecture I2C pour le capteur BMI160
*
* Paramètres:
*  uint8_t Address         - Adresse I2C du capteur
*  uint8_t RegisterAddr    - Adresse du registre où écrire
*  uint8_t *RegisterValue  - Pointeur vers la variable où l'information à écrire est stockée
*  uint8_t Registerlen     - Nombre de bytes à lire
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
unsigned int BMI_ReadBytes(uint8_t Address, uint8_t RegisterAddr, uint8_t *RegisterValue, uint8_t RegisterLen)
{
    /* Variable used for status of I2C operation */
    static unsigned int status;
    
    /* Local variables for storing I2C Master transfer configuration structure */
    cy_stc_scb_i2c_master_xfer_config_t config;
    
    config.slaveAddress = (uint32)Address;
    config.buffer       = &RegisterAddr;
    config.bufferSize   = 1;
    config.xferPending  = true;
    
    /* Start I2C write and take the semaphore */
    status = (unsigned int)BMI_I2C_MasterWrite(&config);
    xSemaphoreTake(xSemaphoreI2C_BMI, portMAX_DELAY);
    
    if(status == CY_SCB_I2C_SUCCESS)
    {
        config.slaveAddress = (uint32)Address;
        config.buffer =  RegisterValue;
        config.bufferSize = RegisterLen;
        config.xferPending = false;
        
        /* Start I2C read and take the semaphore */
        status = (unsigned int)BMI_I2C_MasterRead(&config);
        xSemaphoreTake(xSemaphoreI2C_BMI, portMAX_DELAY);
    }
    return status;
}

/*******************************************************************************
* Nom: BMI_I2C_Callback
********************************************************************************
* Résumé:
*  Fonction qui assure le bon déroulement des transactions I2C avec le capteur 
*  BMI160.
*
* Paramètres:
*  uint32_t events  
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
void BMI_I2C_Callback(uint32_t events)
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
        xSemaphoreGiveFromISR(xSemaphoreI2C_BMI, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/*******************************************************************************
* Nom: MAX_ReadBytes()
********************************************************************************
* Résumé:
*  Fonction de lecture I2C pour le capteur MAX30102
*
* Paramètres:
*  uint8_t RegisterAddr    - Adresse du registre à lire
*  uint8_t *RegisterValue  - Pointeur vers la variable où stocker l'information lue
*  uint8_t RegisterLen     - Nombre de bytes à lire
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
unsigned int MAX_ReadBytes(uint8_t RegisterAddr, uint8_t *RegisterValue, uint8_t RegisterLen)
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
		config.bufferSize = RegisterLen;
		config.xferPending = false;
			
		/* Start I2C read and take the semaphore */
		status = (unsigned int)MAX_I2C_MasterRead(&config);
		xSemaphoreTake(xSemaphoreI2C_MAX, portMAX_DELAY);
	}
	return status;
}

/*******************************************************************************
* Nom: MAX_WriteBytes()
********************************************************************************
* Résumé:
*  Fonction d'écriture I2C pour le capteur MAX30102
*
* Paramètres:
*  uint8_t RegisterAddr    - Adresse du registre où écrire
*  uint8_t *RegisterValue  - Pointeur vers la variable où l'information à écrire est stockée
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
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

/*******************************************************************************
* Nom: MAX_I2C_Callback
********************************************************************************
* Résumé:
*  Fonction qui assure le bon déroulement des transactions I2C avec le capteur 
*  MAX30102.
*
* Paramètres:
*  uint32_t events  
*
* Inspiré du code proposé dans le workspace de démonstration de PSoC Creator
* pour le capteur de mouvement.
*******************************************************************************/
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

/*******************************************************************************
* Nom: MAX_reset()
********************************************************************************
* Résumé:
*  Fonction de remise à 0 du capteur MAX30102 par écriture dans le registre 
*  approprié.  
*******************************************************************************/
void MAX_reset(void)
{
    MAX_WriteBytes(REG_MODE_CONFIG, 0x40);
}

/*******************************************************************************
* Nom: MAX_init()
********************************************************************************
* Résumé:
*  Configuration du capteur MAX30102 en écrivant dans les registres. 
*******************************************************************************/
void MAX_init(void)
{
    
    uint8_t value = 0; //Variable temporaire de passage
       
    MAX_reset(); //Remise à 0 du capteur
    Cy_SysLib_Delay(1000);  //Délais 
    
    MAX_ReadBytes(REG_INTR_STATUS_1,&value,1); //Clear interrupts
    MAX_ReadBytes(REG_INTR_STATUS_2,&value,1);
    
    
    value = 0x40;
    MAX_WriteBytes(REG_INTR_ENABLE_1,&value); // INTR setting  -->Power ready + new data in FIFO
    
    value = 0x00;
    MAX_WriteBytes(REG_INTR_ENABLE_2,&value); // INTR setting
    
    value = 0x00;
    MAX_WriteBytes(REG_FIFO_WR_PTR,&value); //FIFO_WR_PTR[4:0]
    
    value = 0x00;
    MAX_WriteBytes(REG_OVF_COUNTER,&value); //OVF_COUNTER[4:0]
    
    value = 0x00;
    MAX_WriteBytes(REG_FIFO_RD_PTR,&value); //FIFO_RD_PTR[4:0]
    
    //value = 0x00;
    //MAX_WriteBytes(REG_FIFO_CONFIG,&value); //sample avg = 1, fifo rollover=false, fifo almost full = 17
    
    value = 0x03;
    MAX_WriteBytes(REG_MODE_CONFIG,&value); //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
    
    value = 0x27; // 0 01 001 11
    MAX_WriteBytes(REG_SPO2_CONFIG,&value); // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), 18 bit resolution
    
    value = 0x10;
    MAX_WriteBytes(REG_LED1_PA,&value); //Current LED1 (rouge)
    
    value = 0x10;
    MAX_WriteBytes(REG_LED2_PA,&value); //Current LED2 (IR)

}

/*******************************************************************************
* Nom: MAX_ReadFIFO()
********************************************************************************
* Résumé:
*  Lecture et traitement du FIFO du capteur MAX30102
*
* Paramètres : 
* uint32_t *IR_data  - Pointeur vers le buffer des échantillons de la LED IR
* uint32_t *RED_data - Pointeur vers le buffer des échantillons de la LED RED
*******************************************************************************/
void MAX_ReadFIFO(uint32_t *IR_data, uint32_t *RED_data)
{
    NVIC_ClearPendingIRQ(SysInt_AnyMotionINT_cfg.intrSrc); 
    Cy_GPIO_ClearInterrupt(Pin_AnyMotion_INT_PORT, Pin_AnyMotion_INT_NUM);
    NVIC_EnableIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
    
    uint8_t temp_data_buffer[6]; //Stockage temporaire des bytes du FIFO
 
    uint32_t temp; //Stockage temporaire d'un échantillon (3 bytes) du FIFO
    
    
    
    
    // Lire le FIFO en entier une fois pour supprimer les échantillons non significatifs
    for (int i=0;i<32;i++)
    {
         MAX_ReadBytes(REG_FIFO_DATA,&temp_data_buffer,6);
    }
    
    // Lecture 
    for (int i=0;i<500;i++)
    {
        IR_data[i] = 0;
        RED_data[i] = 0;
        
        while(Cy_GPIO_Read(MAX_int_0_PORT,MAX_int_0_NUM)){;} //Wait for new data in FIFO
        
        MAX_ReadBytes(REG_FIFO_DATA,&temp_data_buffer,6);
        
        //RED SIGNAL
        temp = temp_data_buffer[0];
        temp <<= 16;
        RED_data[i] += temp;
        temp = temp_data_buffer[1];
        temp <<= 8;
        RED_data[i] += temp;
        temp = temp_data_buffer[2];
        RED_data[i] += temp;
        RED_data[i] &= 0x03FFFF;
        
        //IR SIGNAL
        temp = temp_data_buffer[3];
        temp <<= 16;
        IR_data[i] += temp;
        temp = temp_data_buffer[4];
        temp <<= 8;
        IR_data[i] += temp;
        temp = temp_data_buffer[5];
        IR_data[i] += temp;
        IR_data[i] &= 0x03FFFF;
        
        //printf("%lu\r\n",RED_data[i]);
        //printf("%lu\r\n",IR_data[i]);
        //printf("RED : %lu   IR : %lu\r\n",RED_data[i],IR_data[i]);
    }
    
    NVIC_DisableIRQ(SysInt_AnyMotionINT_cfg.intrSrc);
	Cy_GPIO_ClearInterrupt(Pin_AnyMotion_INT_PORT, Pin_AnyMotion_INT_NUM);
    NVIC_ClearPendingIRQ(SysInt_AnyMotionINT_cfg.intrSrc); 
     

        
}

/*******************************************************************************
* Nom: set_LED_current()
********************************************************************************
* Résumé:
*  Changer le courant dans les 2 LEDs du capteur MAX30102
*
* Paramètres : 
*   uint8 current_LED1      -courant dans la LED 1 (Red), entre 0 (0mA) et 255 (51mA)
*   uint8 current_LED2      -courant dans la LED 2 (IR), entre 0 (0mA) et 255 (51mA)
*******************************************************************************/
void set_LED_current(uint8 current_LED1,uint8 current_LED2)
{
    MAX_WriteBytes(REG_LED1_PA,&current_LED1);
    MAX_WriteBytes(REG_LED2_PA,&current_LED2);
    
}

void Start_Oxymeter(void)
{
    Cy_GPIO_Write(WORKING_LED_0_PORT,WORKING_LED_0_NUM,0);
    
    //Initialisation I2C MAX30102
    MAX_I2C_Start();  
    MAX_I2C_RegisterEventCallback(MAX_I2C_Callback); 
	xSemaphoreI2C_MAX = xSemaphoreCreateBinary();
  
    //Initialisation I2C BMI160
    BMI_I2C_Start();
    BMI_I2C_RegisterEventCallback(BMI_I2C_Callback);
    xSemaphoreI2C_BMI = xSemaphoreCreateBinary();
    
    //Initialisation du BMI160
    MotionSensor_Init();
    MotionSensor_ConfigAnyMotionIntr();
    
    //Initialisation du MAX30102
    MAX_init();
}


/* [] END OF FILE */