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
#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "touch_task.h"
//#include "display_task.h"
//#include "uart_debug.h" 


void conversionVecteur(uint32_t* vecteur,  int longueur, uint16_t* bonVecteur);
void drawGraph(uint16_t *vecteur200elements);
void updateParameters(int param1, int param2);
void interface(void *pvParameters);
void drawAffichageCourbe(void);

/* [] END OF FILE */