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

uint32_t bufferRedFiltre[4000];   // buffer circulaire rouge filtré (à envoyer à Andréa)
uint32_t bufferIRFiltre[4000];    // buffer circulaire infrarouge filtré (à envoyer à Andréa)

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
    // aka gestion des buffers circulaires
    
    acquisitionSignaux(IR_data,RED_data, &bufferRed[0], &bufferIR[0], &indice);
    
    uint8_t compteurMaxIR = traitementSignal(&bufferIR[0], &bufferIRFiltre[0], &AC_IR, &DC_IR);
    uint8_t compteurMaxRed = traitementSignal(&bufferRed[0],&bufferRedFiltre[0],&AC_red, &DC_red);
    
    calculFrequenceCardiaque(compteurMaxIR,&bpm); // pourrait etre fait direcment ici, car une seule ligne de code simple
    calculSaturation(AC_red, DC_red, AC_IR, DC_IR, &saturation);
    
}

void acquisitionSignaux(uint32_t *data_red,  uint32_t *data_IR,uint32_t *bufferRed,  uint32_t *bufferIR, uint16_t *indice) // taille date = 2000, taille buffer = 4000
{
    for(uint16_t i = *indice; i < 2000 + *indice; i++){
    
        bufferRed[i] = data_red[i];
        bufferIR[i] = data_IR[i];
    
    }
}

uint8_t traitementSignal(uint32_t *buffer,uint32_t *bufferFiltre, uint32_t *AC,uint32_t *DC) // ajouter indice comme param
{
   uint8_t compteurMax = 0;
    
    // TODO: algo matlab à transéfrer (sections: création filtres et traitement signal X (sans l'affichage)
    // filtration + extraction AC/DC (max, min, compteur, moyenne)
    
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
