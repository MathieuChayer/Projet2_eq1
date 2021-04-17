/* ========================================
 * communication.h
 * 
 * Auteur : Mathieu
 * ========================================*/


#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "max30102.h"
#include "bmi160.h"
#include "bmi160_defs.h"
#include "string.h"

#include "interface.h"

#include <stdio.h>

#define I2C_BUFFER_SIZE (10u)
SemaphoreHandle_t xSemaphoreI2C_MAX; 
SemaphoreHandle_t xSemaphoreI2C_BMI;

volatile uint32_t IR_data[2000]; //Buffer de la LED IR
volatile uint32_t RED_data[2000]; //Buffer de la LED RED

unsigned int BMI_WriteBytes(uint8_t Address, uint8_t RegisterAddr, uint8_t *RegisterValue, uint8_t RegisterLen);
unsigned int BMI_ReadBytes(uint8_t Address, uint8_t RegisterAddr, uint8_t *RegisterValue, uint8_t RegisterLen);
void BMI_I2C_Callback(uint32_t events);
void AnyMotion_Interrupt(void);
void MotionSensor_Init(void);
void MotionSensor_ConfigAnyMotionIntr(void);
unsigned int MAX_ReadBytes(uint8_t RegisterAddr, uint8_t *RegisterValue, uint8_t RegisterLen);
unsigned int MAX_WriteBytes(uint8_t RegisterAddr, uint8_t *RegisterValue);
void MAX_I2C_Callback(uint32_t events);
void MAX_reset(void);
void MAX_init(void);
void MAX_ReadFIFO(uint32_t *IR_data, uint32_t *RED_data);
void set_LED_current(uint8 current_LED1,uint8 current_LED2);
void Start_Oxymeter(void);

/* [] END OF FILE */