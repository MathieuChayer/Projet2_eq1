/* ========================================
 * interface.h
 * 
 * Auteur : Andréa
 * ========================================*/

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include "FreeRTOS.h"
#include "task.h"

#include "communication.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <math.h>


extern volatile int FLAG_RED; //On affiche la courbe rouge (1 ou 0)
extern volatile int FLAG_option; //1;On est à l'option 1 dans le menu principal (de base) Valeur possible : 1 à 6 (options dans le menu).
extern volatile int FLAG_menu; //0;On est à l'affichage de la courbe par défaut. Valeur possible : 0 à 7 (8 affichages possibles; courbe, menu principal et 6 sous-menus).
extern volatile int FLAG_modif;

extern volatile int SPO2; //paramètre à afficher; sera déclaré dans Traitement
extern volatile int HR; //paramètre à afficher; sera déclaré dans Traitement


extern volatile int Current_LED1;
extern volatile int Current_LED2;
extern volatile int HR_min_alarm;
extern volatile int HR_max_alarm;
extern volatile int SPO2_min_alarm;

void ClearScreen(void);
void conversionVecteur(uint32_t* vecteur,  int longueur, uint16_t* bonVecteur);
void drawGraph(uint16_t *vecteur200elements);
void interface(void *pvParameters);
void drawAffichageCourbe(void);
void drawWaiting(void);
void isr_SW2(void);
void draw_SousMenu(void);
void draw_MenuPrincipal(void);
void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle);


/* [] END OF FILE */
