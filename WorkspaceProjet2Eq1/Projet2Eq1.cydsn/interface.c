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
#include "interface.h"

//Valider que le drawgraph fonctionne pour tous les signaux possibles
//Tester le menu et trouver les bus
//Vérifier que tout est logique 

volatile int FLAG_RED =1; //On affiche la courbe rouge (1 ou 0)
volatile int FLAG_option = 1; //1;On est à l'option 1 dans le menu principal (de base) Valeur possible : 1 à 6 (options dans le menu).
volatile int FLAG_menu = 0; //0;On est à l'affichage de la courbe par défaut. Valeur possible : 0 à 7 (8 affichages possibles; courbe, menu principal et 6 sous-menus).


volatile uint32_t signalLong[]={205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416,204837,205671,204732,206338,204657,206251,204614,206460,204591,206662,204581,206367,204586,206402,204548,205967,204548,205683,204576,205312,204550,204995,204844,204647,205035,204328,205121,204143,205709,203943,205474,203995,205910,204074,206112,204207,205943,204379,206337,204543,206193,204879,206211,205289,206165,205581,206150,206104,206141,206566,206010,206910,206113,207618,206076,207602,206252,208240,206424,208563,206665,208406,206886,208919,207081,208835,207367,208762,207638,208729,207896,208526,208348,208387,208732,208203,209019,208070,209391,207916,209311,207900,209782,207913,209978,208038,209863,208165,210137,208232,209963,208477,209827,208599,209678,208829,209472,209224,209399,209521,209079,209731,208932,210161,208772,210177,208810,210728,208819,210815,208995,210858,209138,211288,209319,211079,209575,211060,209773,210901,209995,210738,210439,210645,210706,210529,211089,210404,211618,210365,211667,210429,212263,210423,212435,210609,212498,210811,212917,210990,212716,211242,212775,211454,212706,211741,212553,212006,212075,211822,211394,211512,210369,211017,209260,210003,208265,209523,207043,208474,206230,207583,205368,207020,204742,206069,204125,205441,203797,204749,203574,204319,203623,203937,203726,203711,204060,203522,204535,203455,204648,203514,205413,203600,205487,203925,205838,204139,206355,204422,206331,204765,206340,204905,206310,205237,206250,205671,206162,205966,205968,206378,205974,206914,205810,207094,205989,207848,206030,207872,206243,208126,206361,208540,206469,208161,206571,208104,206495,207865,206553,207469,206683,207157,206675,206745,206985,206432,207161,205979,207035,205823,207540,205630,207319,205637,207490,205725,207795,205872,207640,206063,207704,206191,207660,206451,207570,206852,207480,207082,207399,207523,207226,207973,207110,208152,207223,208946,207143,208810,207353,209287,207568,209686,207753,209513,208047,209837,208178,209768,208528,209740,208807,209638,209102,209523,209624,209366,210012,209201,210201,209221,210869,209090,210717,209231,211127,209364,211505,209471,211253,209641,211502,209749,211284,210005,211154,210212,210911,210296,210704,210713,210535,211061,210301,211173,210189,211774,210039,211600,210189,212038,210190,212362,210297,212096,210407,212316,210475,212075,210711,211948,210956,211803,211212,211653,211541,211458,211950,211297,212162,211147,212676,211065,212538,211107,213028,211128,213309,211291,213136,211550,213521,211655,213358,211962,213213,212192,213126,212324,212892,212745,212641,212897,212237,212817,211647,212736,210700,211734,209874,211225,208833,210424,207895,209190,207005,208490,206174,207482,205680,206645,205094,205800,204742,205171,204714,204613,204625,204101,204717,203793,205149,203572,205005,203696,205554,203726,205828,203942,205803,204307,206380,204622,206388,205060,206559,205421,206533,205667,206535,206180,206481,206490,206292,206883,206160,207464,206115,207521,206292,208305,206402,208474,206687,208511,206872,208939,206992,208721,207152,208503,207163,208264,207233,207947,207416,207560,207525,207132,207592,206828,207877,206444,207676,206334,208139,206136,207901,206112,207857,206140,208169,206238,207931,206358,207820,206447,207713,206638,207466,206977,207303,207277,207192,207706,207081,208103,206973,208188,207054,208950,207122,209005,207357,209176,207559,209677,207752,209623,208115,209711,208280,209649,208588,209549,208922,209296,209159,209208,209510,209029,209986,208906,210092,208903,210711,208890,210682,209036,210928,209164,211335,209311,211094,211720,211030,209525,210907,209785,210764,210028,210524,210284,210345,210516,210097,210907,209864,211006,209891,211597,209748,211450,209870,211707,209890,212048,209987,211765,210127,211854,210201,211623,210469,211470,210733,211316,210888,211118,211196,210930,211603,210667,211690,210606,212342,210484,212189,210579,212483,210694,212799,210842,212573,211019,212804,211122,212648,211327,212545,211738,212381,211884,212207,212313,211997,212555,211697,212417,211130,212474,210392,211663,209582,210986,208536,210186,207596,208845,206687,208006,205810,206859,205077,205887,204565,204967,204035,204126,203888,203512,203845,202960,203738,202628,204118,202439,203935,202488,204343,202575,204707,202795,204640,203013,205052,203336,204937,203668,204977,204040,204969,204330,204876,204830,204776,205237,204682,205516,204642,206110,204556,206120,204730,206553,204767,206984,205086,206843,205306,207283,205514,207187,205758,207073,206036,206817,205965,206536,206288,206252,206480,205757,206476,205521,206800,205189,206481,205014,206753,204868,206842,204892,206576,204847,206794,204864,206512,204991,206368,205193,206259,205479,206067,205917,206011,206185,205832,206505,205736,207049,205641,207054,205744,207677,205808,207841,205975,207873,206220,208307,206433,208201,206816,208156,206957,208166,207280,207951,207650,207851,207951,207751,208318,207524,208751,207360,208732,207447,209377,207486,209448,207647,209547,207845,209949,208064,209884,208361,209769,208443,209677,208653,209502,209006,209346,209267,209108,209543,208882,209991,208717,209936,208778,210564,208710,210582,208885,210665,208980,211015,209119,210898,209304,210890,209478,210768,209613,210563,209998,210388,210286,210144,210565,210008,210988,209826,210946,209720,211636,209803,211609,209905,211789,210059,212260,210340,212156,210567,212254,210845,212192,211165,212252,211804,212405,212285,212545,212971,212673,213594,212600,213848,212870,214737,212927,214762,213232,215114,213355,215520,213428,214996,213115,214297,212290,213075,211348,211815,210476,210447,209470,209150,208748,207828,208024,206645,207233,205750,207023,204975,206291,204508,206021,204099,205998,204007,205585,204018,205645,204029,205451,204382,205460,204681,205400,205052,205435,205558,205352,206132,205385,206300,205392,207043,205377,206977,205572,207429,205698,207808,205855,207670,206112,207954,206217,207802,206579,207710,206838,207691,207156,207571,207659,207520,208121,207396,208164,207266,208831,207087,208549,207031,208881,206937,209011,206953,208672,206911};
volatile uint16_t signalCourt[sizeof(signalLong)/sizeof(uint32_t)]; // si problème, int taille = [...]

// le signal sera de 2000 éléments, donc le signal court sera de 400.

// Trouver maximum d'un array
// max = 0;
// min = 300 000;
// vecteur[2000];
// for(int i = 0; i<2000;i++)
//{
//  if(max< vecteur[i])
//    {
//        max = vecteur[i];
//    }
//  if(min > vecteur[i])
//    {
//        min = vecteur[i];
//    }
//}


volatile int SPO2 = 99;
volatile int HR = 70;
volatile int Current_LED1 = 0x15;
volatile int Current_LED2 = 0x15;
volatile int HR_min_alarm = 40;
volatile int HR_max_alarm = 100;
volatile int SPO2_min_alarm = 90;

uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};

void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
{
    cy_eink_frame_t* pEmwinBuffer;

    /* Get the pointer to Emwin's display buffer */
    pEmwinBuffer = (cy_eink_frame_t*)LCD_GetDisplayBuffer();

    /* Update the EInk display */
    Cy_EINK_ShowFrame(imageBufferCache, pEmwinBuffer, updateMethod, powerCycle);

    /* Copy the EmWin display buffer to the imageBuffer cache*/
    memcpy(imageBufferCache, pEmwinBuffer, CY_EINK_FRAME_SIZE);
}

/*******************************************************************************
* Function Name: void ClearScreen(void)
********************************************************************************
*
* Summary: This function clears the screen
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

//fonction pour convertir et raccourcir (nouvel échantantillonnage) le signal en vecteur de 
//uint16 pour pouvoir l'utiliser dans DrawGraph par la suite
int bonneLongueur = 0;
void conversionVecteur(uint32_t* vecteur,  int longueur, uint16_t* bonVecteur) {
    int min = 2000;
    int max = 2500;
    int hauteur = 100; //pixels
    int j =0;
    for(int i=0; i<longueur; i+=5)
    {
        bonVecteur[j]= (vecteur[i]/100 - min)/((max - min)/hauteur);
        j++;
    }
   bonneLongueur = longueur/5;
}

//Affichage de la bonne courbe et des paramètres vitaux
void drawAffichageCourbe(void)
{
    
    GUI_Clear();
    GUI_SetFont(GUI_FONT_13B_1);
    GUI_SetPenSize(2);
    
    GUI_DrawRect(42,4,222,24);  // Nature
    GUI_DispStringAt("Courbe :", 70, 8);
    GUI_DispStringAt("R", 140, 8);
    GUI_DispStringAt("IR", 200, 8);
   
    //Sélecteur de courbe
    if(FLAG_RED ==1)
    {
        GUI_FillRect(127,10,135,18);
        GUI_DrawRect(187,10,195,18);
        GUI_DrawGraph(signalCourt,200,7,65);
        //Gui_drawgraph(&rouge)
    }
    else
    {
        GUI_DrawRect(127,10,135,18);
        GUI_FillRect(187,10,195,18);
        //GUI_DrawGraph(signalCourt,200,7,65); IR
    }
    
    GUI_DrawRect(7,28,257,128); //graphique
    
    GUI_DrawRect(7,132,132,172); //HR
    GUI_DispStringAt("HR (BPM) : ", 36, 138);
    
    GUI_DrawRect(137,132,257,172); //SPO2
    GUI_DispStringAt("SpO2 (%) : ", 167, 138);
        
    //Valeur des paramètres
    GUI_SetFont(GUI_FONT_13_1);
    char str_param[40];
    sprintf(str_param,"%d",HR);
    GUI_DispStringAt(str_param, 36, 154);
    sprintf(str_param,"%d",SPO2);
    GUI_DispStringAt(str_param, 167, 154);
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

//Menu principal 
void draw_MenuPrincipal(void)
{
    GUI_Clear();
    
    GUI_SetPenSize(2);
    GUI_SetFont(GUI_FONT_13B_1);
    
    GUI_DrawRect(82,4,182,24);  // MENU
    GUI_DispStringAt("MENU", 118, 8);
    
    GUI_DrawRect(7,28,257,48);
    GUI_DrawRect(10,32,30,44);
    GUI_DispStringAt("Choix de la courbe (R/IR)", 60, 32);
   
    GUI_DrawRect(7,52,257,72);
    GUI_DrawRect(10,56,30,68);
    GUI_DispStringAt("Borne superieure HR", 60, 56);
    
    GUI_DrawRect(7,76,257,96);
    GUI_DrawRect(10,80,30,92);
    GUI_DispStringAt("Borne inferieure HR", 60, 80);
    
    GUI_DrawRect(7,100,257,120);
    GUI_DrawRect(10,104,30,116);
    GUI_DispStringAt("Borne inferieure SPO2", 60, 104);
    
    GUI_DrawRect(7,124,257,144);
    GUI_DrawRect(10,128,30,140);
    GUI_DispStringAt("Courant de la DEL 1 (Rouge)", 60, 128);
    
    GUI_DrawRect(7,148,257,168);
    GUI_DrawRect(10,152,30,164);
    GUI_DispStringAt("Courant de la DEL 2 (infrarouge)", 60, 152);
    
    //Choix de l'option 
       
    switch (FLAG_option)
    {
        case 1:
        GUI_FillRect(10,32,30,44);
        break;

        case 2:
        GUI_FillRect(10,56,30,68);
        break;
        
        case 3:
        GUI_FillRect(10,80,30,92);
        break;
        
        case 4:
        GUI_FillRect(10,104,30,116);
        break;
        
        case 5:
        GUI_FillRect(10,128,30,140);
        break;
        
        case 6:
        GUI_FillRect(10,152,30,164);
        break;

    }

    UpdateDisplay(CY_EINK_FULL_4STAGE, true);

}

void draw_SousMenu(void)
{
    GUI_Clear();
    int button0_pressed =0;
    int button1_pressed =0;
    int slider_pos = 0xFFFF;
    
    switch (FLAG_menu)
    {
        case 2:
        
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Choix de la courbe (R/IR)", 50, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Changer | SW2 : Appliquer", 40, 34);
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DispStringAt("R", 105, 100);
            GUI_DispStringAt("IR", 175, 100);
      
            if(FLAG_RED ==1)
            {
                GUI_FillRect(92,102,100,110);
                GUI_DrawRect(162,102,170,110);
            }
            else
            {
                GUI_DrawRect(92,102,100,110);
                GUI_FillRect(162,102,170,110);
            }

            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
          
            while(button0_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID); 
                }  
            } 
            
            if (button0_pressed)
            {
                FLAG_RED = !FLAG_RED;
            }
       
        break;
                
        case 3:
            
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Borne superieure HR", 55, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
          
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,95,232,110);
            GUI_DispStringAt("60 BPM", 28, 80);
            GUI_DispStringAt("120 BPM", 210, 80);
            GUI_FillRect(32,95,32+(HR_max_alarm-60)*200/60,110);
            
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DrawRect(80,130,190,160);
            
            char HR_max_alarm_str[20];
            sprintf(HR_max_alarm_str,"%d",HR_max_alarm);
            GUI_DispStringAt(HR_max_alarm_str, 120 , 135);
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                HR_max_alarm -=1; 
            }
            if (button1_pressed)
            {
                HR_max_alarm +=1; 
            }
            
            if (slider_pos < 0xFFFF)
            {
                HR_max_alarm = 60 + slider_pos*60/100;
            }
            
            if(HR_max_alarm > 120)
            {
            HR_max_alarm = 120;
            }
            
            if(HR_max_alarm < 60)
            {
            HR_max_alarm = 60;
            }
      
        break;
                
        case 4:
            
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Borne inferieure HR", 55, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
          
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,95,232,110);
            GUI_DispStringAt("30 BPM", 28, 80);
            GUI_DispStringAt("60 BPM", 210, 80);
            GUI_FillRect(32,95,32+(HR_min_alarm-30)*200/30,110);
            
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DrawRect(80,130,190,160);
            
            char HR_min_alarm_str[20];
            sprintf(HR_min_alarm_str,"%d",HR_min_alarm);
            GUI_DispStringAt(HR_min_alarm_str, 120 , 135);
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                HR_min_alarm -=1; 
            }
            if (button1_pressed)
            {
                HR_min_alarm +=1; 
            }
            
            if (slider_pos < 0xFFFF)
            {
                HR_min_alarm = 10 + slider_pos*30/100;
            }
            
            if(HR_min_alarm > 60)
            {
                HR_min_alarm = 60;
            }
            
            if(HR_min_alarm < 30)
            {
                HR_min_alarm = 30;
            }
      
        break;
                
        case 5:
            
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Borne inferieure SPO2", 55, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
          
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,95,232,110);
            GUI_DispStringAt("0 %", 28, 80);
            GUI_DispStringAt("100 %", 210, 80);
            GUI_FillRect(32,95,32+(SPO2_min_alarm)*200/100,110);
            
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DrawRect(80,130,190,160);
            
            char SPO2_min_alarm_str[20];
            sprintf(SPO2_min_alarm_str,"%d",SPO2_min_alarm);
            GUI_DispStringAt(SPO2_min_alarm_str, 120 , 135);
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                SPO2_min_alarm -=1; 
            }
            if (button1_pressed)
            {
                SPO2_min_alarm +=1; 
            }
            
            if (slider_pos < 0xFFFF)
            {
                SPO2_min_alarm = slider_pos;
            }
            
            if(SPO2_min_alarm > 100)
            {
                SPO2_min_alarm = 100;
            }
            
            if(SPO2_min_alarm < 0)
            {
                SPO2_min_alarm = 0;
            }
      
       
        break;
                
        case 6:
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Courant de la DEL 1 (Rouge)", 40, 8);
            GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
       
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,110,232,130);
            GUI_DispStringAt("0", 28, 90);
            GUI_DispStringAt("255", 222, 90);
            
            char str_Current_LED1[10];
            sprintf(str_Current_LED1,"%d / 255",Current_LED1);
            GUI_DispStringAt(str_Current_LED1, 106, 145);;
            GUI_DrawRect(100,134,164,170);
            
            GUI_FillRect(32,110,32+200*Current_LED1/0xFF,130);
   
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
      
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                Current_LED1 -=0x01; 
            }
            if (button1_pressed)
            {
                Current_LED1 +=0x01; 
            }
            if (slider_pos < 0xFFFF)
            {
                Current_LED1 = slider_pos*0xFF/100;
            }
            
            if (Current_LED1 > 0xFF)
            {
                Current_LED1 = 0xFF;
            }
            
            if (Current_LED1 < 0x00)
            {
                Current_LED1 = 0x00;
            }

        break;
        
        case 7:
            GUI_DrawRect(22,4,242,30);
            GUI_SetFont(GUI_FONT_16B_1);
            GUI_DispStringAt("Courant de la DEL 2 (infrarouge)", 32, 8);
             GUI_SetFont(GUI_FONT_13_1);
            GUI_DispStringAt("Bouton 0 : Diminuer | Bouton 1 : Augmenter", 30, 34);
            GUI_DispStringAt("Slider : Diminuer/Augmenter | SW2: Appliquer", 20, 50);
       
            GUI_SetFont(GUI_FONT_13B_1);
            GUI_DrawRect(32,110,232,130);
            GUI_DispStringAt("0", 28, 90);
            GUI_DispStringAt("255", 222, 90);
            
            char str_Current_LED2[10];
            sprintf(str_Current_LED2,"%d / 255",Current_LED2);
            GUI_DispStringAt(str_Current_LED2, 106, 145);
            GUI_DrawRect(100,134,164,170);
            
            GUI_FillRect(32,110,32+200*Current_LED2/0xFF,130);
   
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            
            
            while(slider_pos >=0xFFFF && button0_pressed == 0 && button1_pressed == 0 && FLAG_menu > 0){
                    
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                    CapSense_ProcessAllWidgets();
                    button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                    button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);
                    slider_pos = CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);   
                }  
            } 
            
            if (button0_pressed)
            {
                Current_LED2 -=0x01; 
            }
            if (button1_pressed)
            {
                Current_LED2 +=0x01;
            }
            if (slider_pos < 0xFFFF)
            {
                Current_LED2 = slider_pos*0xFF/100;
            }
            if (Current_LED2 > 0xFF) //Empêche le courant d'être plus grand que 0xFF
            {
                Current_LED2 = 0xFF;
            }
            if (Current_LED2 < 0x00) //Empêche le courant d'être négatif
            {
                Current_LED2 = 0x00;
            }
              
        break;

    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);


}

void isr_SW2(void){    
  
    Cy_GPIO_ClearInterrupt(SW2_0_PORT,SW2_0_NUM);
    NVIC_ClearPendingIRQ(SW2_isr_cfg.intrSrc);
    
    Cy_SysLib_Delay(100);
    
    //Modifier le courant dans les LEDs si SW2 est appuyé à partir d'un sous-menu des LEDs.
    if(FLAG_menu == 6 || FLAG_menu == 7 ){
    //set_LED_current(Current_LED1,Current_LED2) //Fonction définie dans communication.h 
    }
    
    FLAG_menu = !FLAG_menu;
    
    Cy_GPIO_ClearInterrupt(SW2_0_PORT,SW2_0_NUM);
    NVIC_ClearPendingIRQ(SW2_isr_cfg.intrSrc);

}



void interface(void *pvParameters) 
{
  (void) pvParameters;
  
    Cy_SysInt_Init(&SW2_isr_cfg, isr_SW2);
    NVIC_ClearPendingIRQ(SW2_isr_cfg.intrSrc);
    NVIC_EnableIRQ(SW2_isr_cfg.intrSrc);
    
    CapSense_Start();

    GUI_Init();
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
   
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    
    //vecteur signal long de 2000 éléments en uint32
    
  
   conversionVecteur(&signalLong[0],sizeof(signalLong)/sizeof(uint32_t),&signalCourt[0]);

    /* Start the eInk display interface and turn on the display power */
  
    for(;;){ /* Initialize emWin Graphics */
        
    if(!FLAG_menu)
    {
        //1-Échantillonnage (Mathieu)
        
        //2-Traitement (Ariane)
        
        //3-Vérification des alertes physiologiques 
        if(HR_max_alarm<HR || HR_min_alarm>HR || SPO2_min_alarm > SPO2)
        {
            Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,0);
        }
        else
        {
            Cy_GPIO_Write(BlueLED_0_PORT,BlueLED_0_NUM,1);  
        }

        //4-Affichage de la courbe (Andréa)
        drawAffichageCourbe();
        vTaskDelay(3000); //Délai fictif 
    }
    else
    {
        if(FLAG_menu==1)
        {   draw_MenuPrincipal();
            
            int button0_pressed =0;
            int button1_pressed =0;
            while(button0_pressed == 0 && button1_pressed ==0 && FLAG_menu > 0){
                
                CapSense_ScanAllWidgets();

                if(!CapSense_IsBusy())
                {
                     CapSense_ProcessAllWidgets();
                     button0_pressed = CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID);
                     button1_pressed = CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID);

                }  
            }
            
            if(button0_pressed)
            {
                if(FLAG_menu >0)
                {
                    FLAG_option += 1;
                    if(FLAG_option>6)
                    {
                        FLAG_option = 1;
                    }
                }
            }
            
            if(button1_pressed)
            {
                if(FLAG_menu >0)
                {
                    FLAG_menu = FLAG_option+1;
                 
                }
            }
        } 
        else
        {
            draw_SousMenu();
        }
        
    }
    
        
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    vTaskDelay(pdMS_TO_TICKS(1000));
}
}

/* [] END OF FILE */
