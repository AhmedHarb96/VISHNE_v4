/*
 * Calculation.c
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */

#include "../INC/generalHeaders.h"


uint32_t spectral_data[NUM_WAVELENGTHS*2] = {0};
uint32_t spectral_data_256[20] = {0};
float bilirubin_concentration;
// Known concentrations of bilirubin in mg/dL and their approximate absorption values
//float known_concentrations[] = {1.0, 2.0, 5.0, 10.0, 20.0};
//float approximate_absorptions_450[] = {0.1, 0.2, 0.5, 1.0, 2.0}; // Example data
//float approximate_absorptions_570[] = {0.15, 0.3, 0.75, 1.5, 3.0}; // Example data

uint32_t adc_value = 0;
uint16_t idx = 0;
uint16_t index2 = 0;
uint8_t len;
uint8_t testDone=0;

//uint32_t absorption_at_450nm=0;
//uint32_t absorption_at_570nm=0;
//float bilirubin_concentration_450 = 0;
//float bilirubin_concentration_570 = 0;
int done=0;
int i;

uint32_t sum_filtered_data = 0;
float average_filtered_data = 0.0;
float sum_Reflectance;
float average_Reflectance;
float Reflectance[46]= {0};  //256
//float RefSpect[256]={-0.010105323,-0.001542245,-0.013827409,-0.016224875,-0.015162678,-0.016783329,-0.019090998,-0.020490656,-0.025685276,-0.02442543,-0.022746399,-0.022733002,-0.025591919,-0.020712872,-0.02507225,-0.023936944,-0.024211776,-0.022240257,-0.019938698,-0.024932874,-0.023849355,-0.02515621,-0.02169035,-0.017785813,-0.021399463,-0.0177543,-0.017066733,-0.019727384,-0.023106324,-0.019365872,-0.016117827,-0.014816203,-0.013262104,-0.010695715,-0.017246774,-0.012418471,-0.007498509,-0.012161742,-0.013836856,-0.006570155,-0.00692933,-0.00043138,-0.00693397,0.013101387,0.029237675,0.045484376,0.071342403,0.127393212,0.201550961,0.320963049,0.498962897,0.751700271,1.093130384,1.553503717,2.133724449,2.857026509,3.727092857,4.778994544,6.026648215,7.468517336,9.046113865,10.62824623,12.0214237,12.98246404,13.30466453,12.88126834,11.76750069,10.19426922,8.437028465,6.77645679,5.354340032,4.233947193,3.344487763,2.669834507,2.13121842,1.718408987,1.40132425,1.16945156,1.005574128,0.890910871,0.807043059,0.746887915,0.70711335,0.695674139,0.690381127,0.700472245,0.714920787,0.725353105,0.728404817,0.748875836,0.765798795,0.789111301,0.824326128,0.87436904,0.912955263,0.95732502,0.996816078,1.012878497,1.02266366,1.037042093,1.060319389,1.100197579,1.125467699,1.149560713,1.158267782,1.154884547,1.136763927,1.128551606,1.11377096,1.107474864,1.106044198,1.120658313,1.140855637,1.160095635,1.171055777,1.162204384,1.157684978,1.14137442,1.126809618,1.120498825,1.112271737,1.109026869,1.106361434,1.113550459,1.112812294,1.10808483,1.07401857,1.052534164,1.011848192,0.961434363,0.9113196,0.866481739,0.827956523,0.795853061,0.765593829,0.74908886,0.735191902,0.71673151,0.710648903,0.689423597,0.680082416,0.664666568,0.64396433,0.624859641,0.611678797,0.593828956,0.583780466,0.580157285,0.577283833,0.57491905,0.570493008,0.579474441,0.569706644,0.5595216,0.54377638,0.527413663,0.501622121,0.476070871,0.451567132,0.435447249,0.421438718,0.401227204,0.387149384,0.37335972,0.358682301,0.336753512,0.322345531,0.314853011,0.298619999,0.292306675,0.275951854,0.273490539,0.256958635,0.252099236,0.24914459,0.22818377,0.209385093,0.202750735,0.193503898,0.185502115,0.176264227,0.172084452,0.172503423,0.170579934,0.165982479,0.155821638,0.149530908,0.139065031,0.129122233,0.124082301,0.109021253,0.106667074,0.103518482,0.099912809,0.089572436,0.085274089,0.08624252,0.077655244,0.078390709,0.076765489,0.067807881,0.062537461,0.061491475,0.053393282,0.054650167,0.057361296,0.055657643,0.048567776,0.047993198,0.046423116,0.050743811,0.038207899,0.039428565,0.042294427,0.035114104,0.028929125,0.030219384,0.02615563,0.02247517,0.025092257,0.02261738,0.018778374,0.023536643,0.019434082,0.026410823,0.018587728,0.013891078,0.01569311,0.016296571,0.0162704,0.013332972,0.018031984,0.011010257,0.011616123,0.012862318,0.010308555,-0.000809098,0.005095059,0.006160678,0.005128117,0.002032918,0.002750123,0.002464255,0.005049266,0.010480315,0.0059428,0.012098481,0.000391609,0.002966483,0.005941514,0.008257784,0.003010043,-0.004766859,-0.000111068,0.002534322,0.002343606
//};
float Bil=0;
float BilResult=0.0;
//uint32_t BilResult =1;

void capture_and_send_data(void)
{

    for ( i = 0; i < NUM_WAVELENGTHS*2; i++)
    {
        HAL_ADC_Start_DMA(&hadc3, &adc_value, 1);
        //HAL_Delay(1);
        DWT_Delay(9);
        // Store the adc_value in spectral_data
        spectral_data[idx++] = adc_value;     //- 200; due to Voltage Level difference in ADC on BATT power
        if (idx >= NUM_WAVELENGTHS*2) idx=0;
    }
    // Filter out baseline values
	/*index2=0;
	for (int i = 0; i < Filtered_Spec_Len; i++) spectral_data_256[i] = 0;

	for (int i = 1; i < NUM_WAVELENGTHS * 2; i++)
	{
		uint32_t prev_value = spectral_data[i - 1];
		uint32_t curr_value = spectral_data[i];

		// Consider a value as high if it's significantly higher than the previous one
		if (curr_value > prev_value + (prev_value / 10))       // Lower threshold Less indeces (4-only 600's)
		{
			spectral_data_256[index2++] = curr_value;
			if (index2 >= Filtered_Spec_Len) index2=0;
		}
	}*/

	sum_filtered_data=0;
	average_filtered_data=0;
	// Calculate the average of the filtered data
	for (int i = main_spec_start; i < main_spec_end; i++)
	{   //45 main samples
		sum_filtered_data += spectral_data[i];
	}
	//if (index2 > 0)
	{
		average_filtered_data = (float)sum_filtered_data / (main_spec_end-main_spec_start);
	}

	/*for(int r=0; r<(main_spec_end-main_spec_start); r++)
	{
		Reflectance[r]=spectral_data[r+99]/average_filtered_data;
	}*/

	/*for(int r=0; r<index2; r++) //
	{
		Reflectance[r]=Reflectance[r]/RefSpect[r];
	}
	//BilResult[1]=0;
	Bil=0;
	for(int r=0; r<index2; r++)
	{
		Bil+=Reflectance[r];
	}
	BilResult[0]=(1.7016*Bil*Bil)-(20.6755*Bil)+(66.9023);
*/

	/*BilResult=0;sum_Reflectance=0;average_Reflectance=0;
	for (int i = 0; i < (main_spec_end-main_spec_start); i++)
	{
		 sum_Reflectance += Reflectance[i];
		 average_Reflectance = (float)sum_Reflectance / (main_spec_end-main_spec_start);
	}*/

	//BilResult[0] = average_Reflectance*3;
	//BilResult[0]=(1.7016*average_Reflectance*average_Reflectance)-(20.6755*average_Reflectance)+(66.9023);
	 BilResult = (average_filtered_data-478)/120;     //avgSpecAtBlank_Ref = 478   //CalFactor=56;//80
	 if(BilResult<=0.1) {
		 HAL_GPIO_WritePin(SPEC_LED_GPIO_Port, SPEC_LED_Pin, GPIO_PIN_RESET);
		 testDone = 0;
		 BilResult=0;
		 HAL_GPIO_WritePin(ERR_BUZZER_GPIO_Port, ERR_BUZZER_Pin, GPIO_PIN_SET);   // ***** ERROR Buzzer ***//
		 HAL_Delay(200);
		 HAL_GPIO_WritePin(ERR_BUZZER_GPIO_Port, ERR_BUZZER_Pin, GPIO_PIN_RESET); // ***** ERROR Buzzer ***//
	 }
	 else{
	 testDone = 1;
	 }

	 //if(currentTest<=avgValue) testDone = 1;

    // Format the concentration values as a string
	/*char message[12];            					//Result to be sent to UART and LCD
    for (int j = 0; j < NUM_WAVELENGTHS*2; j++)
    {
      //printf("%lu\n", spectral_data_256[j]);
      snprintf(message, sizeof(message), "%lu\n",spectral_data[j]);
      // Send the concentration values via UART
      HAL_UART_Transmit(&huart2, (uint8_t *)message, sizeof(spectral_data[j]), HAL_MAX_DELAY);    //to PC

      //len = sizeof(spectral_data[j]);

      HAL_UART_Transmit(&huart3, (uint8_t *)message, sizeof(spectral_data[j]), HAL_MAX_DELAY);    //to ESP

    }*/
}
