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
////exemple : max = [20 000] et max en pixels = 120
//min =[10 000] -> pixel min = 20
//offset(x) = [5]
//offset(y) = [20]
//mapper pour que ça prenne de 20 à 120 avec offset de 20 dans ce cas
//plage d'affichage = (max-min)/nb pixels -> 100 pixels pour mon signal

/* Image buffer cache */
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

int bonneLongueur = 0;

//fonction pour convertir et raccourcir (nouvel échantantillonnage) le signal en vecteur de 
//uint16 pour pouvoir l'utiliser dans DrawGraph par la suite

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

// fonction pour dessiner graphique sur 100x250 pixels avec un offset de 20

void drawGraph(uint16_t *vecteur200elements){
    GUI_SetPenSize(1);
    GUI_DrawRect(7, 15, 257, 115);
    int x1 = 7;
    int y1 = 0;
    int x2 = 9;
    int y2 = 0;
    for(uint i=0;i<200;i++){
        y1 = (vecteur200elements[i]*50)+65;
        y2 = (vecteur200elements[i+1]*50)+65;       
        GUI_DrawLine(x1,y1,x2,y2);
        x1 = x2;
        x2 = x2 + 2;
    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}



// fonction pour afficher HR et SpO2
void updateParameters(int param1, int param2){

    char str[100];
    sprintf(str,"HR : %d bpm       SpO2: %d %%",param1, param2);
    
    GUI_DispStringHCenterAt(str, 129, 160);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

//void updateParameters(int param1, int param2){
//
//    char str[100];
//    sprintf(str,"HR : %d bpm       SpO2: %d %%",param1, param2);
//    
//    GUI_DispStringHCenterAt(str, 129, 160);
//}

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
/*******************************************************************************
* Function Name: void WaitforSwitchPressAndRelease(void)
********************************************************************************
*
* Summary: This implements a simple "Wait for button press and release"
*			function.  It first waits for the button to be pressed and then
*			waits for the button to be released.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function and exits only on a button press and release
*
*******************************************************************************/
void WaitforSwitchPressAndRelease(void)
{
    /* Wait for SW2 to be pressed */
    while(Status_SW2_Read() != 0);
    
    /* Wait for SW2 to be released */
    while(Status_SW2_Read() == 0);
}





uint32_t signalLong[]={205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416,204837,205671,204732,206338,204657,206251,204614,206460,204591,206662,204581,206367,204586,206402,204548,205967,204548,205683,204576,205312,204550,204995,204844,204647,205035,204328,205121,204143,205709,203943,205474,203995,205910,204074,206112,204207,205943,204379,206337,204543,206193,204879,206211,205289,206165,205581,206150,206104,206141,206566,206010,206910,206113,207618,206076,207602,206252,208240,206424,208563,206665,208406,206886,208919,207081,208835,207367,208762,207638,208729,207896,208526,208348,208387,208732,208203,209019,208070,209391,207916,209311,207900,209782,207913,209978,208038,209863,208165,210137,208232,209963,208477,209827,208599,209678,208829,209472,209224,209399,209521,209079,209731,208932,210161,208772,210177,208810,210728,208819,210815,208995,210858,209138,211288,209319,211079,209575,211060,209773,210901,209995,210738,210439,210645,210706,210529,211089,210404,211618,210365,211667,210429,212263,210423,212435,210609,212498,210811,212917,210990,212716,211242,212775,211454,212706,211741,212553,212006,212075,211822,211394,211512,210369,211017,209260,210003,208265,209523,207043,208474,206230,207583,205368,207020,204742,206069,204125,205441,203797,204749,203574,204319,203623,203937,203726,203711,204060,203522,204535,203455,204648,203514,205413,203600,205487,203925,205838,204139,206355,204422,206331,204765,206340,204905,206310,205237,206250,205671,206162,205966,205968,206378,205974,206914,205810,207094,205989,207848,206030,207872,206243,208126,206361,208540,206469,208161,206571,208104,206495,207865,206553,207469,206683,207157,206675,206745,206985,206432,207161,205979,207035,205823,207540,205630,207319,205637,207490,205725,207795,205872,207640,206063,207704,206191,207660,206451,207570,206852,207480,207082,207399,207523,207226,207973,207110,208152,207223,208946,207143,208810,207353,209287,207568,209686,207753,209513,208047,209837,208178,209768,208528,209740,208807,209638,209102,209523,209624,209366,210012,209201,210201,209221,210869,209090,210717,209231,211127,209364,211505,209471,211253,209641,211502,209749,211284,210005,211154,210212,210911,210296,210704,210713,210535,211061,210301,211173,210189,211774,210039,211600,210189,212038,210190,212362,210297,212096,210407,212316,210475,212075,210711,211948,210956,211803,211212,211653,211541,211458,211950,211297,212162,211147,212676,211065,212538,211107,213028,211128,213309,211291,213136,211550,213521,211655,213358,211962,213213,212192,213126,212324,212892,212745,212641,212897,212237,212817,211647,212736,210700,211734,209874,211225,208833,210424,207895,209190,207005,208490,206174,207482,205680,206645,205094,205800,204742,205171,204714,204613,204625,204101,204717,203793,205149,203572,205005,203696,205554,203726,205828,203942,205803,204307,206380,204622,206388,205060,206559,205421,206533,205667,206535,206180,206481,206490,206292,206883,206160,207464,206115,207521,206292,208305,206402,208474,206687,208511,206872,208939,206992,208721,207152,208503,207163,208264,207233,207947,207416,207560,207525,207132,207592,206828,207877,206444,207676,206334,208139,206136,207901,206112,207857,206140,208169,206238,207931,206358,207820,206447,207713,206638,207466,206977,207303,207277,207192,207706,207081,208103,206973,208188,207054,208950,207122,209005,207357,209176,207559,209677,207752,209623,208115,209711,208280,209649,208588,209549,208922,209296,209159,209208,209510,209029,209986,208906,210092,208903,210711,208890,210682,209036,210928,209164,211335,209311,211094,211720,211030,209525,210907,209785,210764,210028,210524,210284,210345,210516,210097,210907,209864,211006,209891,211597,209748,211450,209870,211707,209890,212048,209987,211765,210127,211854,210201,211623,210469,211470,210733,211316,210888,211118,211196,210930,211603,210667,211690,210606,212342,210484,212189,210579,212483,210694,212799,210842,212573,211019,212804,211122,212648,211327,212545,211738,212381,211884,212207,212313,211997,212555,211697,212417,211130,212474,210392,211663,209582,210986,208536,210186,207596,208845,206687,208006,205810,206859,205077,205887,204565,204967,204035,204126,203888,203512,203845,202960,203738,202628,204118,202439,203935,202488,204343,202575,204707,202795,204640,203013,205052,203336,204937,203668,204977,204040,204969,204330,204876,204830,204776,205237,204682,205516,204642,206110,204556,206120,204730,206553,204767,206984,205086,206843,205306,207283,205514,207187,205758,207073,206036,206817,205965,206536,206288,206252,206480,205757,206476,205521,206800,205189,206481,205014,206753,204868,206842,204892,206576,204847,206794,204864,206512,204991,206368,205193,206259,205479,206067,205917,206011,206185,205832,206505,205736,207049,205641,207054,205744,207677,205808,207841,205975,207873,206220,208307,206433,208201,206816,208156,206957,208166,207280,207951,207650,207851,207951,207751,208318,207524,208751,207360,208732,207447,209377,207486,209448,207647,209547,207845,209949,208064,209884,208361,209769,208443,209677,208653,209502,209006,209346,209267,209108,209543,208882,209991,208717,209936,208778,210564,208710,210582,208885,210665,208980,211015,209119,210898,209304,210890,209478,210768,209613,210563,209998,210388,210286,210144,210565,210008,210988,209826,210946,209720,211636,209803,211609,209905,211789,210059,212260,210340,212156,210567,212254,210845,212192,211165,212252,211804,212405,212285,212545,212971,212673,213594,212600,213848,212870,214737,212927,214762,213232,215114,213355,215520,213428,214996,213115,214297,212290,213075,211348,211815,210476,210447,209470,209150,208748,207828,208024,206645,207233,205750,207023,204975,206291,204508,206021,204099,205998,204007,205585,204018,205645,204029,205451,204382,205460,204681,205400,205052,205435,205558,205352,206132,205385,206300,205392,207043,205377,206977,205572,207429,205698,207808,205855,207670,206112,207954,206217,207802,206579,207710,206838,207691,207156,207571,207659,207520,208121,207396,208164,207266,208831,207087,208549,207031,208881,206937,209011,206953,208672,206911};

// écrire le code que j'aurais écrit dans main.cm4 ici
void interface(void *pvParameters) 
{
  (void) pvParameters;
    //Placer dans l'initialisation    
    GUI_Init();
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    
    //Draw a rectangle
    GUI_DrawRect(1,1,263,175);
    
    //Draw a line
    GUI_SetPenSize(4);
    GUI_DrawLine(20,65,243,65);
    
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    
   //Write a title and subtile	
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_SetTextAlign(GUI_TA_CENTER);
    GUI_DispStringAt("GBM2110", 132, 30);
    GUI_SetFont(GUI_FONT_16_1);
    GUI_DispStringAt("Laboratoire 123", 132, 70);
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    //vecteur signal long de 1000 éléments en uint32
    
  
   uint16_t signalCourt[sizeof(signalLong)/sizeof(uint32_t)]; // si problème, int taille = [...]
   conversionVecteur(&signalLong[0],sizeof(signalLong)/sizeof(uint32_t),&signalCourt[0]);

    /* Start the eInk display interface and turn on the display power */
  
    for(;;){ /* Initialize emWin Graphics */
    
    
    //Placé avant
    GUI_Clear();  
    //GUI_DrawGraph(signalCourt,200,0,0); 
    drawGraph(signalCourt);
    updateParameters(70,99); 
    //
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    vTaskDelay(pdMS_TO_TICKS(1000));
}
}
/* [] END OF FILE */
