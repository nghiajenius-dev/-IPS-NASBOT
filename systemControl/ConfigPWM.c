/*
 * ConfigPWM.c
 *
 *  Created on: 03-02-2015
 *      Author: TQS
 */

#include "../global_include.h"
#include "ConfigPWM.h"

void ConfigSystem(void)
{
	//Enable FPU
	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();
	// Configure clock
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void ConfigPWM(void)
{

    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinConfigure(GPIO_PE4_M0PWM4);
    GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);
    //40kHz Clock 16Mhz/400
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 400-1);
    // 50% duty cycles
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4,
                     200-1);
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, false);
    PWMGenDisable(PWM0_BASE, PWM_GEN_2);
}

void SetPWM()
{
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void StopPWM()
{
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, false);
    PWMGenDisable(PWM0_BASE, PWM_GEN_2);
}
uint32_t  GetPWM()
{
	return PWMPulseWidthGet(PWM0_BASE, PWM_OUT_4);
}



