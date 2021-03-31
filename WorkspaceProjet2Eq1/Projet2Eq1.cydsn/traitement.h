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


void acquisitionSignaux();                  // met lecture de Mathieu dans buffer circulaire (interruption = flag "Données à lire" et quand flag on traite)
void traitementSignal(uint32_t *buffer);    // + filtres utilisés, flag mi-chemin du buffer = traitement
uint8_t calculFrequenceCardiaque();         // va probablement avoir des params
uint8_t calculSaturation(uint32_t AC_red, uint32_t DC_red,uint32_t AC_IR,uint32_t DC_IR);


/* [] END OF FILE */
