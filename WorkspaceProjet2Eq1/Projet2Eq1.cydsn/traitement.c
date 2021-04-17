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
#include "traitement.h"


 uint8_t bpm = 0;
 uint8_t saturation = 0;

 uint32_t AC_red = 0;
 uint32_t DC_red = 0;
 uint32_t AC_IR = 0;
 uint32_t DC_IR = 0;

void traitementCompletSignaux(uint32_t *IR_data,  uint32_t *RED_data)
{

    acquisitionSignaux(IR_data,RED_data, &bufferRed[0], &bufferIR[0]);
    
    filtreSignal(&bufferRed[0],&bufferRedFiltre[0]); // envoyer signal filtre a Interface
    filtreSignal(&bufferIR[0],&bufferIRFiltre[0]);  // envoyer signal filtre a Interface
    
    uint8_t compteurMaxRed = traitementSignal(&bufferRed[0], &bufferRedFiltre[0], &AC_red, &DC_red);
    uint8_t compteurMaxIR = traitementSignal(&bufferIR[0], &bufferIRFiltre[0], &AC_IR, &DC_IR);
    
    bpm = compteurMaxIR/10*60; // à valider et à envoyer à Interface
    calculSaturation(AC_red, DC_red, AC_IR, DC_IR, &saturation); // à envoyer à interface
    
}

void acquisitionSignaux(uint32_t *data_red,  uint32_t *data_IR,float32_t *bufferRed,  float32_t *bufferIR) // taille date = 2000, taille buffer = 4000
{
    for(uint16_t i = 0; i < 2000; i++){
    
        bufferRed[i] = data_red[i];
        bufferIR[i] = data_IR[i];
    }
}

void filtreSignal(float32_t *buffer,float32_t *bufferFiltre) 
{
    // Initialisation du filtre et appel de la fonction de filtration
    float32_t coeff[20] = {-54220622909.66125,-9139386833.91592,108005074319.7006,56458912545.19908,-57266996975.93793,-178982408271.516,-5221159209.76206,172814867502.733,294081512130.9059,-326529792297.7484,-326529792297.7484,294081512130.9059,172814867502.733,-5221159209.76206,-178982408271.516,-57266996975.93793,56458912545.19908,108005074319.7006,-9139386833.91592,-54220622909.66125};
    uint16_t numTaps  = 20;
    float32 firStateF32[2000];
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S, numTaps, &coeff[0], &firStateF32[0], 2000);
    
    arm_fir_f32(&S, buffer, bufferFiltre , 2000);    
    
    // Filtration du DC rapide
    float32_t moyenne=0;
    
     for(uint16_t i = 0; i < 2000; i++){
           moyenne += bufferFiltre[i];
        }
    moyenne = moyenne/2000;
    
     for(uint16_t i = 0; i < 2000; i++){
       bufferFiltre[i] -= moyenne;
    }
    
    // Affichage via UART des valeurs dans le buffer ou le buffer filtre  
//    UART_1_PutString("Valeurs filtrees dans fct: \n\r");
        for(uint16_t i = 0; i < 2000; i++){
            char buffer_test[100]="";
            sprintf(buffer_test,"%e, ",bufferFiltre[i]);
            UART_1_PutString(buffer_test);
        }
//        UART_1_PutString("Fin valeurs filtrees dans fct \n\r");
  }

    uint8_t traitementSignal(float32_t *buffer, float32_t *bufferFiltre, uint32_t *AC,uint32_t *DC){
    // À TESTER = seems ok but not sure
    // Extraction des composants AC et DC du signal filtré (trouver max, min, nombre de max, etc.)
    
    // Paramètres à calibrer au besoin (selon signal)
    uint16_t index_start = 0;
    uint16_t index_last  = 1999;
    uint16_t cutoffH =  60;
    uint16_t cutoffV = 35;
    int16_t  cutoffVNeg = -5;
    uint16_t lastMax = index_start-cutoffH;
    uint16_t lastMin = index_start-cutoffH;
    
    // Paramètres esssentiels
    uint8_t compteurMax = 0;
    uint8_t compteurMin = 0;
    uint32_t sommeTot = 0;
    uint32_t sommeMax = 0;
    int32_t  sommeMin = 0;
    
    for(uint16_t i = index_start; i < index_last; i++){
        
        sommeTot += buffer[i];
        
        bool maxTrouver = (i >= lastMax + cutoffH & bufferFiltre[i]>cutoffV & bufferFiltre[i-1]<bufferFiltre[i] & bufferFiltre[i+1]<bufferFiltre[i]);
        if(maxTrouver){
            compteurMax += 1;
            sommeMax += bufferFiltre[i];
            lastMax = i;
        }

        bool minTrouver = (i >= lastMin + cutoffH & bufferFiltre[i]<cutoffVNeg & bufferFiltre[i-1]>bufferFiltre[i] & bufferFiltre[i+1]>bufferFiltre[i]);
        if(minTrouver){
            compteurMin += 1;
            sommeMin += bufferFiltre[i];
            lastMin = i;
        }
    }
    *DC = sommeTot /(index_last-index_start);
    *AC = (sommeMax/compteurMax)-(sommeMin/compteurMin);
    
   return compteurMax;
}

void calculFrequenceCardiaque(uint8_t compteur, uint8_t *bpm) 
{ 
    uint8_t temps_traite = 10; // variable à effacer si le temps d'acquisition reste fixe | ou on efface completement la fonction
    *bpm = (compteur/temps_traite*60);
}

void calculSaturation(uint32_t AC_red, uint32_t DC_red,uint32_t AC_IR,uint32_t DC_IR, uint8_t *saturation)
{
    float ratio = (AC_red*DC_red)/(AC_IR*DC_IR);
    
    float a = 1.5958422;
    float b = -34.6596622;
    float c = 112.6898759;
    
    *saturation = a*pow(ratio,2) + b*ratio + c; 
}

/* [] END OF FILE */
