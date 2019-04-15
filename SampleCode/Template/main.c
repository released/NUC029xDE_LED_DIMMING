/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 14/06/30 4:51p $
 * @brief    Software Development Template.
 * @note
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NUC029xDE.h"

#define PLL_CLOCK       50000000

enum
{
	led_init = 0,
	led_start,

	led_default,	
};

uint8_t DUTY_LED = 0;
uint8_t FLAG_LED = 1;
uint8_t CNT_LED = 0;	

extern void SYS_Init(void); 

void PWM0_CH0_Init(void)
{
    SYS->GPA_MFP &= ~(SYS_GPA_MFP_PA12_Msk);
    SYS->GPA_MFP |= SYS_GPA_MFP_PA12_PWM0_CH0;


    PWM_ConfigOutputChannel(PWM0, 0, 20000, 0);
    PWM_EnableOutput(PWM0, PWM_CH_0_MASK);

    /* Start PWM module */
    PWM_Start(PWM0, PWM_CH_0_MASK);

}

void TMR3_IRQHandler(void)
{
	static uint32_t LOG = 0;
	static uint16_t CNT = 0;

    if(TIMER_GetIntFlag(TIMER3) == 1)
    {
        TIMER_ClearIntFlag(TIMER3);

		if (PB3 ==1)
		{
			if (CNT_LED++ >= 18)
			{		
				CNT_LED = 0;
				PWM_ConfigOutputChannel(PWM0, 0, 20000, DUTY_LED);
//				printf("DUTY : %4d\r\n" ,DUTY_LED );
				if (FLAG_LED)
				{
					if ( ++DUTY_LED == 100)
					{
						FLAG_LED = 0;
						DUTY_LED = 100;
					}
				}
				else
				{
					if ( --DUTY_LED == 0)
					{
						FLAG_LED = 1;
						DUTY_LED = 0;
					}			
				}
			}
		}

		if (CNT++ >= 1000)
		{		
			CNT = 0;
        	printf("%s : %4d\r\n",__FUNCTION__,LOG++);
		}
    }
}

void BasicTimer_TIMER3_Init(void)
{
    TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER3);
    NVIC_EnableIRQ(TMR3_IRQn);	
    TIMER_Start(TIMER3);
}

void GPAB_IRQHandler(void)
{
    /* To check if PB.3 interrupt occurred */
    if(GPIO_GET_INT_FLAG(PB, BIT3))
    {
        GPIO_CLR_INT_FLAG(PB, BIT3);
//        printf("PB.3 INT occurred.\n");

		DUTY_LED = 0;
		FLAG_LED = 1;
 		CNT_LED = 0;
		PWM_ConfigOutputChannel(PWM0, 0, 20000, DUTY_LED);		
    }
    else
    {
        /* Un-expected interrupt. Just clear all PA, PB interrupts */
        PA->ISRC = PA->ISRC;
        PB->ISRC = PB->ISRC;
        printf("Un-expected interrupts.\n");
    }
}

void GPIO_Init(void)
{
    GPIO_SetMode(PA, BIT12, GPIO_PMD_OUTPUT);	//led TEST
	PA12 = 0;

    GPIO_SetMode(PB, BIT3, GPIO_PMD_INPUT);
	GPIO_EnableInt(PB, 3, GPIO_INT_FALLING);
    NVIC_EnableIRQ(GPAB_IRQn);
	
    GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_HCLK, GPIO_DBCLKSEL_1024);
    GPIO_ENABLE_DEBOUNCE(PB, BIT3);	
	
}


void UART0_Init(void)
{
    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
    SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);

    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);

	/* Set UART receive time-out */
	UART_SetTimeoutCnt(UART0, 20);

	printf("\r\nCLK_GetCPUFreq : %8d\r\n",CLK_GetCPUFreq());
	printf("CLK_GetHCLKFreq : %8d\r\n",CLK_GetHCLKFreq());	
	printf("CLK_GetPCLK0Freq : %8d\r\n",CLK_GetHXTFreq());
	printf("CLK_GetPCLK1Freq : %8d\r\n",CLK_GetPCLKFreq());

	
}

int main()
{
    SYS_Init();

    UART0_Init();

	GPIO_Init();

    TIMER_Delay(TIMER3, 1000000);
    TIMER_Delay(TIMER3, 1000000);

	PWM0_CH0_Init();
			
	BasicTimer_TIMER3_Init();

    while(1)
    {

    }
}

/*** (C) COPYRIGHT 2018 Nuvoton Technology Corp. ***/
