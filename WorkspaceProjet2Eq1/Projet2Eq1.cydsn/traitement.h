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

float32_t bufferRed[4000];   // buffer circulaire rouge
float32_t bufferIR[4000];    // buffer circulaire infrarouge

float32_t bufferRedFiltre[4000];   // buffer circulaire rouge filtré (à envoyer à Andréa)
float32_t bufferIRFiltre[4000];    // buffer circulaire infrarouge filtré (à envoyer à Andréa)

extern uint16_t indice;

extern uint8_t  bpm;
extern uint8_t  saturation;

extern uint32_t AC_red;
extern uint32_t DC_red;
extern uint32_t AC_IR;
extern uint32_t DC_IR;

void traitementCompletSignaux(uint32_t *bufferRed,  uint32_t *bufferIR);
void acquisitionSignaux(uint32_t *data_red,  uint32_t *data_IR,float32_t *bufferRed,  float32_t *bufferIR, uint16_t *indice);  
void filtreSignal(float32_t *buffer,float32_t *bufferFiltre);   
uint8_t traitementSignal(float32_t *buffer,float32_t *bufferFiltre, uint32_t *AC,uint32_t *DC);
void calculFrequenceCardiaque(uint8_t compteur, uint8_t *bpm);                             
void calculSaturation(uint32_t AC_red, uint32_t DC_red,uint32_t AC_IR,uint32_t DC_IR, uint8_t *saturation);


/* [] END OF FILE */
