/****************************************************************************
 * @file     sysclock.c
 * @version  V1.01
 * @Date     2019/04/11-13:04:10
 * @brief    NuMicro generated code file
 *
 * Copyright (C) 2013-2019 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

/********************
MCU: M0516LDE(LQFP48)
Base Clocks:
LIRC: 10kHz
HIRC: 22.1184MHz
HXT: 12MHz
PLL: 48MHz
HCLK: 48MHz
PCLK: 48MHz
Enabled-Module Frequencies:
PWM01=Bus Clock (PCLK): 48MHz/Engine Clock: 12MHz
TMR3=Bus Clock (PCLK): 48MHz/Engine Clock: 12MHz
UART0=Bus Clock (PCLK): 48MHz/Engine Clock: 12MHz
********************/

//#include "M051Series.h"
#include "NUC029xDE.h"

/*----------------------------------------------------------------------------
  Define HXT clock.
  Please locate and modify the real one in your project.
  Otherwise, the project may fail to build.
 *----------------------------------------------------------------------------*/
#define __HXT         (12000000UL)  /*!< High Speed External Crystal Clock Frequency */

/*
 * @brief This function updates clock registers to fulfil the configuration
 * @param None
 * @return None
 */
void SYS_Init()
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    //CLK->PWRCON = (CLK->PWRCON & ~(0x0000000Dul)) | 0x0000001Dul;
    //CLK->PLLCON = (CLK->PLLCON & ~(0x000FFFFFul)) | 0x0000C22Eul;
    //CLK->CLKDIV = (CLK->CLKDIV & ~(0x00FF0F0Ful)) | 0x00000000ul;
    //CLK->CLKSEL0 = (CLK->CLKSEL0 & ~(0x0000003Ful)) | 0x0000003Aul;
    //CLK->CLKSEL1 = (CLK->CLKSEL1 & ~(0xF377773Ful)) | 0xCC8FFFFFul;
    //CLK->CLKSEL2 = (CLK->CLKSEL2 & ~(0x000300FCul)) | 0x000200FFul;
    //CLK->AHBCLK = (CLK->AHBCLK & ~(0x0000001Cul)) | 0x00000001ul;
    //CLK->APBCLK = (CLK->APBCLK & ~(0xD0F3337Dul)) | 0x00110020ul;
    //CLK->FRQDIV = (CLK->FRQDIV & ~(0x0000003Ful)) | 0x00000000ul;
    //SysTick->CTRL = (SysTick->CTRL & ~(0x00000005ul)) | 0x00000000ul;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable clock source */
    CLK_EnableXtalRC(CLK_PWRCON_OSC10K_EN_Msk|CLK_PWRCON_OSC22M_EN_Msk|CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for clock source ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC10K_STB_Msk|CLK_CLKSTATUS_OSC22M_STB_Msk|CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Disable PLL first to avoid unstable when setting PLL */
    CLK_DisablePLL();

    /* Set PLL frequency */
    CLK->PLLCON = (CLK->PLLCON & ~(0x000FFFFFul)) | 0x0000C22Eul;

    /* Waiting for PLL ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_PLL_STB_Msk);

    /* If the defines do not exist in your project, please refer to the related clk.h in the clk_h folder appended to the tool package. */
    /* Set HCLK clock */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL, CLK_CLKDIV_HCLK(1));

    /* Enable IP clock */
    CLK_EnableModuleClock(PWM0_MODULE);
    CLK_EnableModuleClock(TMR3_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);

    /* Set IP clock */
    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL3_PWM0_S_PCLK, MODULE_NoMsk);
    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3_S_HXT, MODULE_NoMsk);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

    return;
}
/*** (C) COPYRIGHT 2013-2019 Nuvoton Technology Corp. ***/
