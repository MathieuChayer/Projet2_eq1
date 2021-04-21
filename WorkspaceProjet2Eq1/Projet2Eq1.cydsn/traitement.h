/* ========================================
 *
 * traitement.h
 * 
 * Auteur : Ariane
 * ========================================*/
#include "project.h"
#include <stdio.h>

void traitement(uint32_t *signal, int *HR, uint32_t *AC, uint32_t *DC);
void interpretation(int HR_RED, int HR_IR, uint32_t AC_RED, uint32_t DC_RED, uint32_t DC_IR, uint32_t AC_IR, uint32_t *HR,  uint32_t *SPO2 );

/* [] END OF FILE */