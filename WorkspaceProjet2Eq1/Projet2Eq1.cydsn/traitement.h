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

void traitementCompletSignaux(uint32_t *bufferRed,  uint32_t *bufferIR);
void acquisitionSignaux(uint32_t *data_red,  uint32_t *data_IR,uint32_t *bufferRed,  uint32_t *bufferIR, uint16_t *indice);  
uint8_t traitementSignal(uint32_t *buffer,uint32_t *bufferFiltre, uint32_t *AC,uint32_t *DC);                            
void calculFrequenceCardiaque(uint8_t compteur, uint8_t *bpm);                             
void calculSaturation(uint32_t AC_red, uint32_t DC_red,uint32_t AC_IR,uint32_t DC_IR, uint8_t *saturation);


/* [] END OF FILE */
