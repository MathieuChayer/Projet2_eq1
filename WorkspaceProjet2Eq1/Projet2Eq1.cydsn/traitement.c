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

uint32_t bufferRed[4000];   // buffer circulaire rouge
uint32_t bufferIR[4000];    // buffer circulaire infrarouge

float32_t bufferRedFiltre[4000];   // buffer circulaire rouge filtré (à envoyer à Andréa)
float32_t bufferIRFiltre[4000];    // buffer circulaire infrarouge filtré (à envoyer à Andréa)

uint16_t indice = 0;

uint8_t  bpm = 0;
uint8_t  saturation = 0;

uint32_t AC_red = 0;
uint32_t DC_red = 0;
uint32_t AC_IR = 0;
uint32_t DC_IR = 0;

void traitementCompletSignaux(uint32_t *IR_data,  uint32_t *RED_data)
{
    // TODO: gestion de l'appel des différentes fonctions de traitement (traitement première moitiée vs deuxième, etc.)
    // Gestion de l'indice pour l'acquisition et le traitement des signaux (remise à zéro, 2 indices car 2 signal à traiter?)
    // aka gestion des buffers circulaires!!
    
    acquisitionSignaux(IR_data,RED_data, &bufferRed[indice], &bufferIR[indice], &indice);
    
    uint8_t compteurMaxIR = traitementSignal(&bufferIR[indice-2000], &bufferIRFiltre[indice-2000], &AC_IR, &DC_IR);
    uint8_t compteurMaxRed = traitementSignal(&bufferRed[indice-2000],&bufferRedFiltre[indice-2000],&AC_red, &DC_red);

    calculFrequenceCardiaque(compteurMaxIR,&bpm); // pourrait etre fait direcment ici, car une seule ligne de code simple
    calculSaturation(AC_red, DC_red, AC_IR, DC_IR, &saturation);
    
    if(indice == 4000){
        indice = 0;
    }
    
}

void acquisitionSignaux(uint32_t *data_red,  uint32_t *data_IR,uint32_t *bufferRed,  uint32_t *bufferIR, uint16_t *indice) // taille date = 2000, taille buffer = 4000
{
    for(uint16_t i = *indice; i < 2000 + *indice; i++){
    
        bufferRed[i] = data_red[i];
        bufferIR[i] = data_IR[i];
    
    }
}

uint8_t traitementSignal(uint32_t *buffer,float32_t *bufferFiltre, uint32_t *AC,uint32_t *DC) // ajouter indice comme param
{
   // TODO: algo matlab à transéfrer (sections: création filtres et traitement signal X (sans l'affichage) aka FILTRATION
    
    arm_fir_instance_f32 S;
    arm_status status;
    
    
    float32_t coeff[20] = {-1.59987720980260e-06,	-4.75566480052249e-06,	-7.77273389110339e-06,	-1.05579400724207e-05,	-1.30235807709003e-05,	-1.50895500558671e-05,	-1.66853153290864e-05,	-1.77516679906359e-05,	-1.82422054503264e-05,	-1.81245081556136e-05,	-1.73809824893486e-05,	-1.60093482867789e-05,	-1.40227581465864e-05,	-1.14495444681017e-05,	-8.33259903035016e-06,	-4.72839872877413e-06,	-7.05699593990838e-07,	3.65607076846908e-06,	8.26868484932276e-06,	1.30372070818964e-05};
    float32_t firStateF32[39];
    
    arm_fir_init_f32(&S, 20, &coeff[0], &firStateF32[0], 20);
    int nBlocs = 100;
    
    for(int i=0; i < nBlocs; i++)
    {
        arm_fir_f32(&S, (float32_t *)buffer + (i * 20), bufferFiltre + (i * 20), 20);
    }
    
    // Extraction des composants AC et DC du signal filtré (trouver max, min, nombre de max, etc.)
    
    // Paramètres à changer selon signal (FAIRE TESTS AVEC MATLAB!)
    uint16_t index_start = 0;
    uint16_t index_last  = 0;
    uint16_t lastMax = 0;
    uint16_t lastMin = 0;
    uint16_t cutoffH =  1;
    uint16_t cutoffV = 1;
    int16_t  cutoffVNeg = -1;
    
    // Paramètres esssentiels
    uint8_t compteurMax = 0;
    uint8_t compteurMin = 0;
    uint32_t sommeTot = 0;
    uint32_t sommeMax = 0;
    int32_t  sommeMin = 0;
    
    for(uint16_t i = index_start; i < index_last; i++){
        
        sommeTot += bufferFiltre[i];
        
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
    uint8_t temps_acquisition = 10; // variable à effacer si le temps d'acquisition reste fixe
    *bpm = (compteur/temps_acquisition*60);
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
