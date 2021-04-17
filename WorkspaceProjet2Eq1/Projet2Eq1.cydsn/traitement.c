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


void traitement(uint32_t *signal, int *HR, uint32_t *AC, uint32_t *DC)
{
    uint32_t cleaned[2000] = {};
    float redressed[2000] = {};   

    //Nettoyer le signal avec une moyenne mobile sur 8 données
    int w = 8; //Window size
    
    for (int i=0;i<2000-w;i++)
    {
        for (int j=1;j<w+1;j++)
        {
            cleaned[i]=cleaned[i]+signal[i+j];
        }
        cleaned[i]=cleaned[i]/w;
    }
    for (int i=2000-w;i<2000;i++)
    {
        cleaned[i] = signal[i];
    }
    
    for (int i =0;i<2000;i++)
    {
        //printf("%d\r\n",cleaned[i]);
    }
    
    //Redresser avec un min/max mobile
    int s = 400; 
    *DC = 0;
    *AC = 0;
    
    //Fenêtre du min/max mobile : 
    //400 échantillons = 2 secondes à 200 hz
    //Un espacement de plus de 2 secondes entre 2 battements signifie un HR < 30 BPM.

    for (int i = 0; i<2000/s;i++)
    {
        uint32_t MAX = 0;
        uint32_t MIN = 300000;
        
        for(int j=i*s;j<i*s+s;j++)
        {
            if (cleaned[j] >MAX)
            {
                MAX = cleaned[j];
            }
             if (cleaned[j] < MIN)
            {
                MIN = cleaned[j];
            }
            
        }
        
        for(int j=i*s;j<i*s+s;j++)
        {
           redressed[j] = (float) (cleaned[j]-MIN)/(MAX-MIN);
        }

        *DC = *DC + (MAX+MIN)/2;
        *AC = *AC + (MAX-MIN);
        
    }
    *DC = (uint32_t) *DC/(2000/s);
    *AC = (uint32_t) *AC/(2000/s);
    
    //Arrondir le vecteur redressé pour mettre en valeur les pics
    for(int i=1;i<2000;i++)
    {
       if(redressed[i]<0.5)
       {
            redressed[i] =0;
       }
       else
       {
            redressed[i] =1;
       }
       redressed[i] = redressed[i];
    }
       
    int nb_spike = 0;
    int last_state_change = 0;
    int state = 0; //high or low
    float L = 0; //average length of high pulse
    
    state = redressed[0];
    last_state_change = 0;
    for(int i=1;i<2000;i++)
    {
        if(redressed[i]==!state)
        {
            state = !state;
            if(state == 0)
            {
                L = L + i - last_state_change;
                nb_spike +=1;
                
            }
            last_state_change = i;
            
        }
    }
    if(redressed[1999]==1 && state ==1)
    {
        L = L + 1999 - last_state_change;
        nb_spike +=1;
    }
    
    L = L/nb_spike;
    L = L/2;
    
    state = redressed[0];
    last_state_change = 0;
    for(int i=0;i<2000;i++)
    {
        if(redressed[i]==!state)
        {
            state = !state;
            
            if(state == 0 && i-last_state_change < L)
            {
                nb_spike -=1;
            }
            last_state_change = i;
        }
    }
    
    if(redressed[1999]==1 && state ==1 && 1999 - last_state_change < L)
    {
        nb_spike -=1;
    }
    *HR = nb_spike;

}


void interpretation(int HR_RED, int HR_IR, uint32_t AC_RED, uint32_t DC_RED, uint32_t DC_IR, uint32_t AC_IR, uint32_t *HR,  uint32_t *SPO2 )
{
    
    //HR 
    
    //1200 : seuil où je considère que le signal d'entrée n'est que du bruit
    if(DC_RED < 1200 || DC_IR < 1200)
    {
        // Le HR est toujours calculé avec le signal IR (plus fiable).
        *HR = 0;
    }
    else 
    {
        *HR =  HR_IR*6; 
    }
    
 
        
    //SPO2
    float R = ((float)AC_RED/(float)DC_RED)/((float)AC_IR/(float)DC_IR);
    float a = 1.5958422;
    float b = -34.6596622;
    float c = 112.6898759;
    
    *SPO2 = a*R*R+b*R+c;
    
    //Interprétation des cas extrêmes 
    //Si une valeur n'a pas de sens physiologiquement, le paramètre sera mis à 0.
    
    //Gestion si HR < 27 BPM ou >220 BPM (données incohérentes)
    if(*HR<27 || *HR>220)
    {
       *HR = 0;
    }
    //Gestion si SPO2>100 ou <85% (plus faible que le minimum de la littérature)
    if(*SPO2>100 || *SPO2 < 85)
    {
        *SPO2=0;
    }
   
   //Gestion si un des deux paramètres est nul
    if(*HR==0 || *SPO2 ==0)
    {
        *SPO2 = 0;
        *HR = 0;
    }

}



/* [] END OF FILE */
