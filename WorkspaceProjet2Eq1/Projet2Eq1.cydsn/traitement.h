/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "communication.h"

#include "LCDConf.h"
#include <arm_math.h>
#include <core_cm4.h>

#include <stdlib.h>
#include <math.h>

float32_t bufferRed[2000];   // buffer rouge
float32_t bufferIR[2000];    // buffer infrarouge

float32_t bufferRedFiltre[2000];   // buffer rouge filtré 
float32_t bufferIRFiltre[2000];    // buffer infrarouge filtré 

extern uint8_t  bpm;
extern uint8_t  saturation;

extern uint32_t AC_red;
extern uint32_t DC_red;
extern uint32_t AC_IR;
extern uint32_t DC_IR;

void traitementCompletSignaux(uint32_t *IR_data,  uint32_t *RED_data);
void acquisitionSignaux(uint32_t *data_red,  uint32_t *data_IR,float32_t *bufferRed,  float32_t *bufferIR);  
void filtreSignal(float32_t *buffer,float32_t *bufferFiltre);   
uint8_t traitementSignal(float32_t *buffer,float32_t *bufferFiltre, uint32_t *AC,uint32_t *DC);
void calculFrequenceCardiaque(uint8_t compteur, uint8_t *bpm);    // probablement à effacer, plus simple de le faire direct dans traitement complet                         
void calculSaturation(uint32_t AC_red, uint32_t DC_red,uint32_t AC_IR,uint32_t DC_IR, uint8_t *saturation);


/* [] END OF FILE */
