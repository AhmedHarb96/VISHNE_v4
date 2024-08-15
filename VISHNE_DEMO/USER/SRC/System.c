/*
 * System.c
 *
 *  Created on: Aug 5, 2024
 *      Author: pc
 */
#include "../INC/generalHeaders.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

int logo_time = 2500;    //2500
int text_time = 2500;    //2000


void systemLoop(void)
{
	  spectrometer();
	  LCD();
}

void systemSetup(void)
{
	   SpectrometerSetup();
	   LCD_Setup();
}

//################################################################################################//

void SpectrometerSetup(void)
{
	  TIM1->CCR1=5;  //50   50% Duty Cycle ==> to generate 135khz
	  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

	  HAL_GPIO_WritePin(GPIOB, SPEC_START_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOB, SPEC_CLK_Pin|SPEC_EOS_Pin|SPEC_GAIN_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET); // Turn off the LED
}

void LCD_Setup(void)
{
	  ssd1306_Init();
	  Aymed_Logo();
	  Aymed_Text();

}

void Aymed_Logo(void)
{
	 ssd1306_Fill(Black);
	 ssd1306_SetCursor(0, 0);
     ssd1306_DrawBitmap(0,0,image_data_ss,128,64, White );
     ssd1306_UpdateScreen();
     HAL_Delay(logo_time);
}

void Aymed_Text(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0, 0);
    ssd1306_DrawBitmap(0,0,image_data_logo_text,128,32, White );          //AYMED text
    //---------------------------------------------------//
    ssd1306_SetCursor(34, 48);
    ssd1306_WriteString("VISHNE v4", Font_7x10, White);					  //Device's Version
    //---------------------------------------------------//
    ssd1306_UpdateScreen();

    HAL_Delay(text_time);

}
