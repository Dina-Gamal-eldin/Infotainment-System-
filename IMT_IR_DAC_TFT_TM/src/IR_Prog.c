/*
 * IR.c
 *
 *  Created on: Mar 13, 2024
 *      Author: Dina Gamal
 */


#include"LSTD_TYPES.h"
#include"LBIT_MATH.h"
#include "MGPIO_interface.h"
#include "MSYSTICK_Interface.h"
#include "IR_Interface.h"

u8 flag=0;


volatile u8 Global_u8Flag = 0 ;

volatile u32 Global_u32FrameData[50] = {0} ; // 32 + 1 start = 33

volatile u8 Global_u8EdgeCounter   = 0 ;


volatile u8 Global_u8Data =  0 ;
// 28 -> RED BUTTON
// 9 ->  1   BUTTON
// 29 -> 2   BUTTON
// 31 -> 3   BUTTON
// 13 -> 4   BUTTON
// 25 -> 5   BUTTON
// 27 -> 6   BUTTON
// 17 -> 7   BUTTON
// 21 -> 8   BUTTON
// 23 -> 9   BUTTON
// 18 -> 0   BUTTON
void App_voidPlay(void)
{

	switch(Global_u8Data)
	{

	case 28: flag=1;
	break ;
	case 29 : flag=2;
	break ;
	}
}

/*Called in the End of Frame */
void App_voidTakeAction(void)
{
	//asm("NOP") ;
	u8 i ;

	Global_u8Data = 0 ;
	if((Global_u32FrameData[0]>= 10000) && (Global_u32FrameData[0] <= 14000))
	{
		MGPIO_voidWriteData(_GPIOA_PORT, _PIN_2, _HIGH);
		for(i = 0 ; i<8 ;i++)
		{
			if ((Global_u32FrameData[17+i]>= 2000) && (Global_u32FrameData[17+i] <= 2300))
			{
				/*Logic one*/
				SET_BIT(Global_u8Data,i);
			}
			else
			{
				/*Logic Zero*/
				CLR_BIT(Global_u8Data,i);
			}
		}
		App_voidPlay();
	}
	else
	{
		/*invalid Frame*/
	}
	Global_u8Flag = 0 ;
	Global_u32FrameData[0] = 0 ;
	Global_u8EdgeCounter   = 0 ;
}

void App_voidGetFrame()
{

	// asm("NOP") ;

	if(Global_u8Flag == 0)
	{
		/*Start Timer*/
		MSTK_voidSetIntervalSingle(1000000,App_voidTakeAction);
		Global_u8Flag = 1 ;
	}
	else
	{
		Global_u32FrameData[Global_u8EdgeCounter] = MSTK_u32GetElapsedTime() ;
		MSTK_voidSetIntervalSingle(1000000,App_voidTakeAction);
		Global_u8EdgeCounter++;
	}

}


