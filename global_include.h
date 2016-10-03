/*
 * global_include.h
 *
 *  Created on: Oct 1, 2016
 *      Author: nghiajenius
 */

#ifndef GLOBAL_INCLUDE_H_
#define GLOBAL_INCLUDE_H_

// C library
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// CC1101 Stuff
#include "bsp.h"
#include "bsp_leds.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"

// Tivaware
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"

#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/qei.h"
#include "driverlib/fpu.h"
#include "driverlib/pwm.h"

// User define
#include "Timer/Timer.h"
#include "Bluetooth/uartstdio.h"
#include "Bluetooth/ustdlib.h"
#include "Bluetooth/Bluetooth.h"
#include "HostComm/HostComm.h"

#endif /* GLOBAL_INCLUDE_H_ */
