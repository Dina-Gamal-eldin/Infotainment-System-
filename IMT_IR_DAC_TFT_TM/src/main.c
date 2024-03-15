
/*
 * main.c
 *
 *  Created on: Mar 1, 2024
 *      Author: Dina Gamal
 */

#include"LSTD_TYPES.h"
#include"LBIT_MATH.h"
#include "MRCC_interface.h"
#include "MGPIO_interface.h"
#include "MNVIC_Interface.h"
#include"MEXTI_interface.h"
#include "MSPI_Interface.h"
#include "HTFT_Interface.h"
#include"Delay_interface.h"
#include "MSYSTICK_Interface.h"
#include"MUSART_Interface.h"
#include "MGPIO_private.h"
#include "S2P_interface.h"
#include"GPTIM_interface.h"
#include "IR_Interface.h"

#include "all_is_well.h"
#include "speedwarning.h"
#include "welcome.h"
#include "welcome_msg.h"
#include"pic2.h"
#define NVIC_EXTI0     6
#define NVIC_UART1     37

static u8 L_u8msg=0;

#define LED_NUM   8
volatile u16 i = 0 ;
void App_voidSetDAC(void);

void App_voidSetDAC1(void);

extern u8 flag;

static u8 LD_flag=0;
static u8 ALL_flag=0;
int main(void)
{

	/*System Clock is 16MHz from HSI*/
	MRCC_voidInitSystemClk();

	/*Enable GPIO Peripheral clock*/
	MRCC_voidEnablePeripheralClock(AHB1,_PERIPHERAL_EN_GPIOB ) ;
	MRCC_voidEnablePeripheralClock(AHB1,_PERIPHERAL_EN_GPIOA ) ;
	MRCC_voidEnablePeripheralClock(APB2, PERIPHERAL_EN_SYSCFG) ;
	MRCC_voidEnablePeripheralClock(APB1, PERIPHERAL_EN_TIM2) ;


	/* Config RCC GPIO SPI */
	MRCC_voidEnablePeripheralClock(APB2, PERIPHERAL_EN_SPI1); //Enable CLK Of SPI1   At APB2
	/*Enable RCC For USART1*/
	MRCC_voidEnablePeripheralClock(APB2, PERIPHERAL_EN_USART1); //Enable CLK Of UART1   At APB2


	/*************************************************************************************************************************/
	/******************************************** Configur SPI1  Pins**************************************************/
	/*************************************************************************************************************************/


	/* GPIO At HW MODE MOSI(AF,OUTPUT)Like SPI1--->AF(5)*/
	/* GPIO At HW MODE NSS(AF,OUTPUT)Like SPI1--->AF(5) */
	/* GPIO At HW MODE MISO(AF,INPUT)Like SPI1--->AF(5) */

	MGPIO_voidSetPinMode(_GPIOA_PORT, _PIN_5, _MODE_ALTF);
	MGPIO_voidSetPinMode(_GPIOA_PORT, _PIN_7, _MODE_ALTF);

	MGPIO_voidSetPinAltFn(_GPIOA_PORT, _PIN_5, _ALTFN_5);    //SPI1_SCK-->SPI1
	MGPIO_voidSetPinAltFn(_GPIOA_PORT, _PIN_7, _ALTFN_5);     //SPI1_MOSI-->SPI1

	/*************************************************************************************************************************/
	/******************************************** Configur UART1  Pins******************************************/
	/*************************************************************************************************************************/

	MGPIO_voidSetPinMode(_GPIOA_PORT, _PIN_9, _MODE_ALTF);     //TX-->UART1
	MGPIO_voidSetPinMode(_GPIOA_PORT, _PIN_10, _MODE_ALTF);    //RX-->UART1
	MGPIO_voidSetPinAltFn(_GPIOA_PORT, _PIN_9, _ALTFN_7);      //TX-->UART1
	MGPIO_voidSetPinAltFn(_GPIOA_PORT, _PIN_10, _ALTFN_7);     //RX-->UART1

	/*************************************************************************************************************************/
	/******************************************** initialize Output Pins  in System ***********************************/
	/*************************************************************************************************************************/
	for(int PIN =1; PIN<4; PIN++)
	{                                                                              /* A0   */
		/*Set Pin Mode --> Output*/                                                /* RST  */
		MGPIO_voidSetPinMode(_GPIOA_PORT, PIN, _MODE_OUTPUT) ;                     /* CLK  */
		/*Out put Push Pull*/                                                      /* MOSI */
		MGPIO_voidSetPinOutPutType(_GPIOA_PORT,PIN,_OUTPUT_TYPE_PUSH_PULL);
		/*Pin Speed*/
		MGPIO_voidSetPinOutSpeed(_GPIOA_PORT, PIN, _OUTPUT_SPEED_LOW);
	}


	/*Set Pin Mode --> Input*/
	MGPIO_voidSetPinMode(_GPIOA_PORT, _PIN_0, _MODE_INPUT) ;     //EXTI0
	/*Input Pull Type*/
	MGPIO_voidSetPullType(_GPIOA_PORT, _PIN_0, _PULL_OFF);
	/*************************************************************************************************************************/
	/********************************************Step 6: Initialize SPI1 ***************************************************/
	/*************************************************************************************************************************/

	MSPI1_voidInit() ;

	/*************************************************************************************************************************/
	/********************************************Step 6:  initialize system Peripherials *************************************/
	/*************************************************************************************************************************/

	/* TFT Init */
	HTFT_voidInitialize();
	//led_matrix
	MGPIO_voidSetPinMode(_GPIOB_PORT, 8, _MODE_OUTPUT) ;
	MGPIO_voidSetPinMode(_GPIOB_PORT, 9, _MODE_OUTPUT) ;
	MGPIO_voidSetPinMode(_GPIOB_PORT, 10, _MODE_OUTPUT) ;



	for(u8 PIN_NUM = 0 ; PIN_NUM < LED_NUM ; PIN_NUM++)
	{
		/*Set Pin Mode --> Output*/
		MGPIO_voidSetPinMode(_GPIOB_PORT, PIN_NUM, _MODE_OUTPUT) ;

		/*Out put Push Pull*/
		MGPIO_voidSetPinOutPutType(_GPIOB_PORT,PIN_NUM,_OUTPUT_TYPE_PUSH_PULL);

		/*Pin Speed*/
		MGPIO_voidSetPinOutSpeed(_GPIOB_PORT, PIN_NUM, _OUTPUT_SPEED_LOW);

	}



	MEXTI0_voidSetCallBack(App_voidGetFrame);
	MEXTI_voidSetEXTIConfiguration(EXTI0, _GPIOA_PORT);
	MEXTI_voidSetSignalLatch(EXTI0, FALLING_EDGE);


	MNVIC_voidInitInterruptGroup(NVIC_GroupMode_g16s0);

	MNVIC_voidSetInterruptPriority(NVIC_UART1,NVIC_GroupMode_g16s0,1,0);
	MNVIC_voidSetInterruptPriority(NVIC_EXTI0,NVIC_GroupMode_g16s0,0,0);


	/*Enable Peripheral Interrupt*/
	MNVIC_voidEnableInterrupt(NVIC_EXTI0);
	/*Enable uart1 from NVIC*/
	MNVIC_voidEnableInterrupt(NVIC_UART1) ;
	/*************************************************************************************************************************/
	/******************************************** Initialize USART1 ***************************************************/
	/*************************************************************************************************************************/

	MSUART_voidInit();



	/*************************************************************************************************************************/
	/********************************************  initialize system Peripherials *************************************/
	/*************************************************************************************************************************/

	MUSART_voidEnable(1) ; //Enable UART 1
	TIM2_voidInit();
	/*SysTick Init*/
	MSTK_voidInit();

	while(flag==0){};

	/*Enable Interrupt*/
	MSTK_voidInit();
	/* Display  Welcome Voice */
	MSTK_voidINTStatus(MSTK_INT_EN) ;
	/*For 125 Micro Second*/
	MSTK_voidStart(250) ;
	/*Set  CallBack*/
	MSTK_voidSetCallBack(App_voidSetDAC1) ;

	/* Display  Welcome Image */
	HTFT_voidDisplayImage(welcome1);
	/* Display Welcome Word ON Led Matrix */
	print_welcome();
	Delay_vMsIn16MHz(1500);
	MSTK_voidINTStatus(MSTK_INT_DIS) ;

	MSTK_voidInit();


	while(1)
	{


		while(ALL_flag !=1)
		{
			/* Display  All is Well Image */
			HTFT_voidDisplayImage(all_is_well);
			if(flag ==1 )
			{
				break;
			}

		}
		if(L_u8msg=='G')
		{

			while (flag != 2)
			{
			LD_flag=1;
			//ALL_flag=1;
			/* Display  Warning Voice */
			/*Enable Interrupt*/
			MSTK_voidINTStatus(MSTK_INT_EN) ;

			/*For 125 Micro Second*/
			MSTK_voidStart(250) ;
			/*Set  CallBack*/
			MSTK_voidSetCallBack(App_voidSetDAC) ;
			/* Display  Warning Image */
			HTFT_voidDisplayImage(pic2);
			/* Display Warning Word ON Led Matrix */
			warning();
			if(flag == 2)
						{
							ALL_flag=0;
							L_u8msg=0;
							break;
						}
			}
//			if(flag == 2)
//			{
//				ALL_flag=0;
//				L_u8msg=0;
//
//			}
//			Delay_vMsIn16MHz(3500);
//			ALL_flag=0;
//			L_u8msg=0;
		}





	}
}

void App_voidSetDAC(void)
{

	GPIOB_ODR = speedwarning_raw[i] ;
	i++ ;
	if(i == 18040)
	{
		i = 0 ;
	}
}
void App_voidSetDAC1(void)
{

	GPIOB_ODR = welcome_msg_raw[i] ;
	i++ ;
	if(i == 7000)
	{
		i = 0 ;
	}
}
void USART1_IRQHandler(void){
	L_u8msg=MUSART_u8ReadData(1);
}
