/*
 * display.c
 *
 *  Created on: Nov 23, 2020
 *      Author: Stancoj
 */

#include "display.h"

display_data_ dDisplayData = {0};
uint64_t disp_time = 0, disp_time_saved = 0;

void updateDisplay(void);
void setDigit(uint8_t pos);

/*Reset (turn-off) all the segments of display*/
void resetSegments(void)
{
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_3);
}

/*Set (turn-on) all the segments of display*/
void setSegments(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_3);
}

/* Reset (turn-off) all digits*/
void resetDigits(void)
{
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_2);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
}


/* Reset (turn-on) all digits*/
void setDigits(void)
{
	DIGIT_1_ON;
	DIGIT_2_ON;
	DIGIT_3_ON;
	DIGIT_4_ON;
	DIGIT_TIME_ON;
}

void setDecimalPoint(void)
{
	LL_GPIO_ResetOutputPin(SEGMENTDP_PORT, SEGMENTDP_PIN);
}

const unsigned char seven_seg_digits_decode_abcdefg[75]=
{
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x0F, 0x3E, 0x1C, 0x5C, 0x13, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x06, 0x3C, 0x57,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F, 0x3E, 0x1C, 0x5C,
/*  x     y     z     */
    0x13, 0x3B, 0x6D
};
/* Functions to display numbers 0 - 9 */
void setOne(void)
{
	// B,C
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
}

void setTwo(void)
{
	// A,B,G,E,D
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
}

void setThree(void)
{
	// A,B,G,C,D
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
}

void setFour(void)
{
	// F,B,G,C
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
}

void setFive(void)
{
	// A,F,G,C,D
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
}

void setSix(void)
{
	// A,F,G,E,C,D
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);
}

void setSeven(void)
{
	// A,B,C
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
}

void setEight(void)
{
	// A,B,C,D,E,F,G
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
}

void setNine(void)
{
	// A,B,C
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
}

void setZero(void)
{
	// A,B,C,D,E,F
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
}

void setA(void){
	LL_GPIO_ResetOutputPin(SEGMENTA_PORT, SEGMENTA_PIN);
}

void setB(void){
	LL_GPIO_ResetOutputPin(SEGMENTB_PORT, SEGMENTB_PIN);
}

void setC(void){
	LL_GPIO_ResetOutputPin(SEGMENTC_PORT, SEGMENTC_PIN);
}

void setD(void){
	LL_GPIO_ResetOutputPin(SEGMENTD_PORT, SEGMENTD_PIN);
}

void setE(void){
	LL_GPIO_ResetOutputPin(SEGMENTE_PORT, SEGMENTE_PIN);
}

void setF(void){
	LL_GPIO_ResetOutputPin(SEGMENTF_PORT, SEGMENTF_PIN);
}

void setG(void){
	LL_GPIO_ResetOutputPin(SEGMENTG_PORT, SEGMENTG_PIN);
}
/**
 *
 * Pre-process number before it is displayed. Extract digits of the number
 */

void displayNumber(char char1,char char2,char char3,char char4)
{
	uint8_t i = 0;


	dDisplayData.digit_num = 4;
	dDisplayData.resolution = 0;

	dDisplayData.dispText[0] = char1;
	dDisplayData.dispText[1] = char2;
	dDisplayData.dispText[2] = char3;
	dDisplayData.dispText[3] = char4;

	/*if(num > 9999)
	{
		dDisplayData.digit_num = 4;
		dDisplayData.negative = 0;
		dDisplayData.resolution = 0;
	}
	else if(num >= 1000)
	{
		dDisplayData.digit_num = 4;
		dDisplayData.resolution = 0;

	    while(num > 1)
	    {
	    	dDisplayData.digit[i] = (uint32_t)num % 10;
	        num = num / 10;
	        i++;
	    }
	}
	else if(num >= 100)
	{
		dDisplayData.digit_num = 3;
		dDisplayData.resolution = 1;

		num = num * 10;

	    while(num > 1)
	    {
	    	dDisplayData.digit[i] = (uint32_t)num % 10;
	        num = num / 10;
	        i++;
	    }
	}
	else if(num >= 10)
	{
		dDisplayData.digit_num = 2;
		dDisplayData.resolution = 2;

		num = num * 100;

	    while(num > 1)
	    {
	    	dDisplayData.digit[i] = (uint32_t)num % 10;
	        num = num / 10;
	        i++;
	    }
	}
	else if(num >= 1)
	{
		dDisplayData.digit_num = 1;
		dDisplayData.resolution = 3;

		num = num * 1000;

	    while(num > 1)
	    {
	    	dDisplayData.digit[i] = (uint32_t)num % 10;
	        num = num / 10;
	        i++;
	    }
	}
	else if(num >= 0)
	{
		dDisplayData.digit_num = 1;
		dDisplayData.resolution = 3;

		num = num * 1000;
		dDisplayData.digit[3] = 0;

	    while(num > 1)
	    {
	    	dDisplayData.digit[i] = (uint32_t)num % 10;
	        num = num / 10;
	        i++;
	    }
	}*/
}

/*
 * Turns required digit ON
 */
void setDigit(uint8_t pos)
{
	switch(pos)
	{
		case 0:
			DIGIT_4_ON;
			break;
		case 1:
			DIGIT_3_ON;
			break;
		case 2:
			DIGIT_2_ON;
			break;
		case 3:
			DIGIT_1_ON;
			break;
	}
}

/**
 * Display data in dDisplayData.
 * Sets every digit to display its value and decimal point.
 */

unsigned char decode_7seg(unsigned char chr)
{ /* Implementation uses ASCII */
    if (chr > (unsigned char)'z')
        return 0x00;
    return seven_seg_digits_decode_abcdefg[chr - '0'];
    /* or
	return seven_seg_digits_decode_gfedcba[chr - '0']; */
}

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
void updateDisplay(void)
{
	for(uint8_t i = 0; i < 4; i++)
	{
		setDigit(i);
		unsigned char seg7 = decode_7seg(dDisplayData.dispText[i]);
		if(CHECK_BIT(seg7,6)){
			setA();}
		if(CHECK_BIT(seg7,5)){
			setB();}
		if(CHECK_BIT(seg7,4)){
			setC();}
		if(CHECK_BIT(seg7,3)){
			setD();}
		if(CHECK_BIT(seg7,2)){
			setE();}
		if(CHECK_BIT(seg7,1)){
			setF();}
		if(CHECK_BIT(seg7,0)){
			setG();}
		/*
		if((seg7 & 0b0100000) == 0b0100000){
			setB();}
		if((seg7 & 0b0010000) == 0b0010000){
			setC();}
		if((seg7 & 0b0001000) == 0b0001000){
			setD();}
		if((seg7 & 0b0000100) == 0b1000100){
			setE();}
		if((seg7 & 0b0000010) == 0b1000010){
			setF();}
		if((seg7 & 0b0000001) == 0b0000001){
			setG();}
		*/
		/*switch(dDisplayData.digit[i])
		{
			case 0:
			  setDigit(i);
			  setZero();
			  break;
			case 1:
			  setDigit(i);
			  setOne();
			  break;
			case 2:
			  setDigit(i);
			  setTwo();
			  break;
			case 3:
			  setDigit(i);
			  setThree();
			  break;
			case 4:
			  setDigit(i);
			  setFour();
			  break;
			case 5:
			  setDigit(i);
			  setFive();
			  break;
			case 6:
			  setDigit(i);
			  setSix();
			  break;
			case 7:
			  setDigit(i);
			  setSeven();
			  break;
			case 8:
			  setDigit(i);
			  setEight();
			  break;
			case 9:
			  setDigit(i);
			  setNine();
			  break;
		}

		if(dDisplayData.resolution == i)
		{
			setDecimalPoint();
		}
		*/
		disp_time_saved = disp_time;
		while((disp_time_saved + 2) > disp_time){};

		resetDigits();
		resetSegments();
	}
}

//Update displayed data and keep display ON
void TIM3_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3))
	{
		updateDisplay();
	}

	LL_TIM_ClearFlag_UPDATE(TIM3);
}

