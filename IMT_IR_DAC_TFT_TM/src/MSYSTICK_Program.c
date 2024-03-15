
 /*******************************************************************************************************/
/* Author            :Dina Gamal                                                                              */

/*#####################################################################################################*/

/*******************************************************************************************************/
/*                                      Standard Types LIB                                             */
/*******************************************************************************************************/

#include "LSTD_TYPES.h"
#include "LERROR_STATES.h"
#include "LBIT_MATH.h"


/*#####################################################################################################*/
/*#####################################################################################################*/

/*******************************************************************************************************/
/*                                      MCAL Components                                                */
/*******************************************************************************************************/

#include "MSYSTICK_private.h"
#include "MSYSTICK_interface.h"
#include "MSYSTICK_config.h"

/*#####################################################################################################*/
/*#####################################################################################################*/

/*******************************************************************************************************/
/*                                      Functions Implementations                                      */
/*******************************************************************************************************/

/*#####################################################################################################*/
/*#####################################################################################################*/

/*******************************************************************************************************/
/*                                      00- SysTick_Handler                                            */
/*-----------------------------------------------------------------------------------------------------*/
/* 1- Function Description -> Enable Peripheral Clock                                                  */
/* 2- Function Input       -> Copy_uBusName ,   Copy_u8PerNum                                          */
/*   - BusName_t      : The Bus Of The Prepheral ( AHB1 , AHB2 , APB1 , APB2 ) Every One               */
/*                      Detect Which REG To Choice Pripheral From It                                   */
/*   - Copy_u8PerName :  The Order Of Prepheral On The Selected Reg                                    */
/* 3- Function Return      -> No Thing                                                                 */
/*******************************************************************************************************/



void ( * MSTK_CallBack ) ( void );

#define MSTK_SINGLE_INTERVAL        1

/* Define Variable for interval mode */
static u8 MSTK_u8ModeOfInterval ;

void SysTick_Handler(void){

	volatile u8 Local_u8Temporary;



	/* Callback notification */
	MSTK_CallBack();

	/* Clear interrupt flag */
	Local_u8Temporary = GET_BIT(MSTK->CTRL,16);

}



/*******************************************************************************************************/
/*                                      01- MSTK_voidInit                                              */
/*-----------------------------------------------------------------------------------------------------*/
/* 1- Function Description -> Enable Peripheral Clock                                                  */
/* 2- Function Input       -> Copy_uBusName ,   Copy_u8PerNum                                          */
/*   - BusName_t      : The Bus Of The Prepheral ( AHB1 , AHB2 , APB1 , APB2 ) Every One               */
/*                      Detect Which REG To Choice Pripheral From It                                   */
/*   - Copy_u8PerName :  The Order Of Prepheral On The Selected Reg                                    */
/* 3- Function Return      -> No Thing                                                                 */
/*******************************************************************************************************/


void MSTK_voidInit( void ){

	// Disable Systick Interrupt  -  Clock = AHB / 8 - Stop Systic
	MSTK->CTRL = 0x00000000;
}

/*******************************************************************************************************/
/*                                      02- MSTK_voidStart                                              */
/*-----------------------------------------------------------------------------------------------------*/
/* 1- Function Description -> Enable Peripheral Clock                                                  */
/* 2- Function Input       -> Copy_uBusName ,   Copy_u8PerNum                                          */
/*   - BusName_t      : The Bus Of The Prepheral ( AHB1 , AHB2 , APB1 , APB2 ) Every One               */
/*                      Detect Which REG To Choice Pripheral From It                                   */
/*   - Copy_u8PerName :  The Order Of Prepheral On The Selected Reg                                    */
/* 3- Function Return      -> No Thing                                                                 */
/*******************************************************************************************************/


void MSTK_voidStart( u32 Copy_PreloadValue ){

	// AHB = EXT = 8MHZ & CLOCK OF MSTK = AHB / 8 = 8MHZ / 8 = 1MHZ = 1 Micro For Each Count
	//Load Reload Value
	MSTK->LOAD = Copy_PreloadValue - 1;
	//Clear Val Register
	MSTK->VAL  = 0 ;
	//Enable Systick
	SET_BIT( MSTK->CTRL , 0 );

}

/*******************************************************************************************************/
/*                                      03- MSTK_voidINTStatus                                              */
/*-----------------------------------------------------------------------------------------------------*/
/* 1- Function Description -> Enable Peripheral Clock                                                  */
/* 2- Function Input       -> Copy_uBusName ,   Copy_u8PerNum                                          */
/*   - BusName_t      : The Bus Of The Prepheral ( AHB1 , AHB2 , APB1 , APB2 ) Every One               */
/*                      Detect Which REG To Choice Pripheral From It                                   */
/*   - Copy_u8PerName :  The Order Of Prepheral On The Selected Reg                                    */
/* 3- Function Return      -> No Thing                                                                 */
/*******************************************************************************************************/


void MSTK_voidINTStatus( u8 Copy_u8Status ){

	MSTK->CTRL &= ~( 1 << 1 );
	MSTK->CTRL |=  ( Copy_u8Status << 1 );

}

/*******************************************************************************************************/
/*                                      04- MSTK_u8ReadFlag                                            */
/*-----------------------------------------------------------------------------------------------------*/
/* 1- Function Description -> Enable Peripheral Clock                                                  */
/* 2- Function Input       -> Copy_uBusName ,   Copy_u8PerNum                                          */
/*   - BusName_t      : The Bus Of The Prepheral ( AHB1 , AHB2 , APB1 , APB2 ) Every One               */
/*                      Detect Which REG To Choice Pripheral From It                                   */
/*   - Copy_u8PerName :  The Order Of Prepheral On The Selected Reg                                    */
/* 3- Function Return      -> No Thing                                                                 */
/*******************************************************************************************************/

u8 MSTK_u8ReadFlag( void ){

	return ( GET_BIT( MSTK->CTRL , 16 ) );

}
/*******************************************************************************************************/
/*                                      05- MSTK_voidSetCallBack                                       */
/*-----------------------------------------------------------------------------------------------------*/
/* 1- Function Description -> Enable Peripheral Clock                                                  */
/* 2- Function Input       -> Copy_uBusName ,   Copy_u8PerNum                                          */
/*   - BusName_t      : The Bus Of The Prepheral ( AHB1 , AHB2 , APB1 , APB2 ) Every One               */
/*                      Detect Which REG To Choice Pripheral From It                                   */
/*   - Copy_u8PerName :  The Order Of Prepheral On The Selected Reg                                    */
/* 3- Function Return      -> No Thing                                                                 */
/*******************************************************************************************************/


void MSTK_voidSetCallBack( void (*ptr)(void) ){

	MSTK_CallBack = ptr;
}



void MSTK_voidSetIntervalSingle( u32 Copy_u32Ticks, void (*Copy_ptr)(void) )
{
	/* Disable Timer */
	CLR_BIT(MSTK->CTRL,0);
	MSTK -> VAL = 0;

	/* Load ticks to load register */
	MSTK -> LOAD = Copy_u32Ticks*2;

	/* Start Timer */
	SET_BIT(MSTK->CTRL, 0);

	/* Save CallBack */
	MSTK_CallBack = Copy_ptr;

	/* Set Mode to Single */
	MSTK_u8ModeOfInterval = MSTK_SINGLE_INTERVAL;

	/* Enable STK Interrupt */
	SET_BIT(MSTK->CTRL, 1);
}


u32  MSTK_u32GetElapsedTime(void)
{
	u32 Local_u32ElapsedTime;

	Local_u32ElapsedTime = MSTK -> LOAD - MSTK -> VAL ;

	return Local_u32ElapsedTime/2;
}


void MSTK_voidDelayMs(u32 Copy_u32Delay)
{

	MSTK_voidInit();
	/* Disable INT */
	MSTK_voidINTStatus( MSTK_INT_DIS );
	MSTK_voidStart( Copy_u32Delay * 1000 );
	/* Wiat Flag Polling */
	while( MSTK_u8ReadFlag() == 0 );



}
void STK_voidSetBusyWaitInterval(u32 Copy_u32Ticks)
{
	//set the load value
	MSTK -> LOAD=Copy_u32Ticks;
	// Disable the interrupt.
		//CLR_BIT( SYSTICK->CTRL, 1 ) ;//Added
	//start timer
	SET_BIT(MSTK->CTRL,0);
	//wait flag
	while(GET_BIT(MSTK->CTRL,16)==0);
	//once flag is set ->stop timer /reset load value
	CLR_BIT(MSTK->CTRL,0);

	SET_BIT(MSTK->CTRL,16);

	MSTK -> LOAD=0;
	MSTK->VAL=0;


}


void MSYSYTICK_voidStart(u32 Copy_u32Loadvalue)
{
	MSTK->VAL = 0;
	MSTK->LOAD = Copy_u32Loadvalue;
	MSTK->CTRL |= (1<<1);

}
u8   MSYSYTICK_voidReadFlag(void)
{
	return GET_BIT(MSTK->CTRL,16);
}

void MSYSYTICK_voidCtrlIntState(SYSTICKState Copy_State )
{
	switch (Copy_State) {
	case SYSTICKEnable:
		MSTK->CTRL |= (1<<1);
		break;
	case SYSTICKDisable:
		MSTK->CTRL &= ~(1<<1);
		break;
	default:
		break;
	}

}

// 16MHZ --> /8 ---> 2MHZ
void MSYSYTICK_voidDelayUs(u32 Copy_u32Delayvalue)
{
	MSYSYTICK_voidCtrlIntState(SYSTICKDisable);
	MSYSYTICK_voidStart(Copy_u32Delayvalue*2);
	while (MSYSYTICK_voidReadFlag() == 0);
	CLR_BIT(MSTK->CTRL,0);
	MSTK->LOAD = 0 ;
	MSTK->VAL  = 0 ;
}

void MSYSYTICK_voidDelayms(u32 Copy_u32Delayvalue)
{
	MSYSYTICK_voidCtrlIntState(SYSTICKDisable);
	MSYSYTICK_voidStart(Copy_u32Delayvalue*2000);
	while (MSYSYTICK_voidReadFlag() == 0);
	CLR_BIT(MSTK->CTRL,0);
	MSTK->LOAD = 0 ;
	MSTK->VAL  = 0 ;
}
