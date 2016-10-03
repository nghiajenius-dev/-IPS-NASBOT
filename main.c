#include "global_include.h"

#include "systemControl/Config.h"
#include "systemControl/ConfigPWM.h"
#include "IMU/LSM9DS1.h"

volatile uint16_t x;
mrfiPacket_t packet;

uint8_t rev_data[10];
int16_t gx, gy, gz; // x, y, and z axis readings of the gyroscope
int16_t ax, ay, az; // x, y, and z axis readings of the accelerometer
int16_t mx, my, mz; // x, y, and z axis readings of the magnetometer
int16_t temperature; // Chip temperature

const float PI = 3.14159265358979323846;

#define RESET_SYSTEM_TIME 1000
#define STOP_PWM_TIME	50
TIMER_ID timer,stop_timer;

/*
 * Subfunction Prototypes
 */

void printAccel();
void printGyro();
void printMag();
void printAttitude(float ax, float ay, float az, float mx, float my, float mz);
void Timer_reset_all(void);
void Stop_pwm_timer(void);
void config_gpio(void);

void Stop_pwm_timer(void)
{
	if(stop_timer != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(stop_timer);
	stop_timer = INVALID_TIMER_ID;
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4,0);
}

void Timer_reset_all(void)
{
	if(timer != INVALID_TIMER_ID)
		TIMER_UnregisterEvent(timer);
	timer = INVALID_TIMER_ID;

	timer = TIMER_RegisterEvent(&Timer_reset_all, RESET_SYSTEM_TIME);
	config_gpio();
}

void onButtonDown(void) {
	if (GPIOIntStatus(GPIO_PORTE_BASE, false) & GPIO_PIN_4) {
		GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);
		GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4,GPIO_PIN_3 | GPIO_PIN_4);
		stop_timer = TIMER_RegisterEvent(&Stop_pwm_timer, STOP_PWM_TIME);
	}
}

// =============================== Main Control Loop ================================== //
/*
 *
 */
// ==================================================================================== //
int main()
{
	// Set the system clock to run at 50Mhz off PLL with external crystal as reference.
	//Config clock
	ConfigSystem();
	Timer_Init();
	ConfigPWM();
	SetPWM();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4,0); //disable PWM

	//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);        // Enable port F
	//	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);  // Init PF4 as input
	//	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF

//	BSP_Init();
//	MRFI_Init();
//	mrfiSpiWriteReg(PA_TABLE0,0xC0);
//	MRFI_WakeUp();
//	MRFI_RxOn();
//	packet.frame[0]=8+5;
	//config timer

	//!TODO-CONFLICT!!!
	//STOP AFTER 4 IMU DATA!!!
//	timer = TIMER_RegisterEvent(&Timer_reset_all, RESET_SYSTEM_TIME);
//	config_gpio();

	//TODO: IMU fuctions integrated
	Uart_RF_config();			// migrated from UART_init()
	IMU_init();
	UARTprintf("Hello, I'm NASBOT!\n");
	IMU_readWHOAMI_AG(&rev_data[0]);
	IMU_readWHOAMI_M(&rev_data[1]);

	if((rev_data[0]==0x68)&&(rev_data[1]==0x3D))
		UARTprintf("IMU check: ok!\n");

	while (1)
	{
		//MRFI_DelayMs(200);
		//MRFI_Transmit(&packet, MRFI_TX_TYPE_FORCED);
		//BSP_TOGGLE_LED1();

		//TODO: IMU fuctions integrated
		printAccel(); // Print "A: ax, ay, az"
		printGyro();  // Print "G: gx, gy, gz"
		printMag();   // Print "M: mx, my, mz"

		printAttitude(ax, ay, az, -my, -mx, mz);
		SysCtlDelay(10000000);
	}
}

void config_gpio(void)
{
	//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2,0); // trigger to RF2500
	//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4,0); //disable PWM
	//
	//     GPIOIntDisable(GPIO_PORTE_BASE, GPIO_PIN_4);        // Disable interrupt for PF4 (in case it was enabled)
	//     GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_4);      // Clear pending interrupts for PF4
	//     GPIOIntRegister(GPIO_PORTE_BASE, onButtonDown);     // Register our handler function for port F
	//     GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_4,GPIO_FALLING_EDGE);             // Configure PF4 for falling edge trigger
	//     GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_4);     // Enable interrupt for PF4
	//
	//     SysCtlDelay(3200);
	//     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2,GPIO_PIN_2);

	MRFI_Transmit(&packet, MRFI_TX_TYPE_FORCED); // send package
	MRFI_DelayMs(2);
	BSP_TOGGLE_LED1();
	// Enable PWM
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4,GPIO_PIN_3 | GPIO_PIN_4);
	stop_timer = TIMER_RegisterEvent(&Stop_pwm_timer, STOP_PWM_TIME);
}

void MRFI_RxCompleteISR()
{
	//MRFI_Transmit(&packet, MRFI_TX_TYPE_FORCED);
	BSP_TOGGLE_LED2();

}

void printAccel()
{
	// To read from the accelerometer, you must first call the
	// readAccel() function. When this exits, it'll update the
	// ax, ay, and az variables with the most current data.
	readAccel();

	// Now we can use the ax, ay, and az variables as we please.
	// Either print them as raw ADC values, or calculated in g's.
	UARTprintf("A: ");
	UARTprintf("x=%6d ",ax);
	UARTprintf("y=%6d ",ay);
	UARTprintf("z=%6d ",az);
	//  UARTprintf("\n");
}

void printGyro()
{
	// To read from the accelerometer, you must first call the
	// readAccel() function. When this exits, it'll update the
	// ax, ay, and az variables with the most current data.
	readGyro();

	// Now we can use the ax, ay, and az variables as we please.
	// Either print them as raw ADC values, or calculated in g's.
	UARTprintf("G: ");
	UARTprintf("x=%6d ",gx);
	UARTprintf("y=%6d ",gy);
	UARTprintf("z=%6d ",gz);
	//  UARTprintf("\n");
}

void printMag()
{
	// To read from the accelerometer, you must first call the
	// readAccel() function. When this exits, it'll update the
	// ax, ay, and az variables with the most current data.
	readMag();

	// Now we can use the ax, ay, and az variables as we please.
	// Either print them as raw ADC values, or calculated in g's.
	UARTprintf("M: ");
	UARTprintf("x=%6d ",mx);
	UARTprintf("y=%6d ",my);
	UARTprintf("z=%6d ",mz);
	//  UARTprintf("\n");
}

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
	float roll = atan2(ay, az);
	float pitch = atan2(-ax, sqrt(ay * ay + az * az));

	float heading;
	if (my == 0)
		heading = (mx < 0) ? 180.0 : 0;
	else
		heading = atan2(mx, my);

	//  heading -= DECLINATION * PI / 180;

	if (heading > PI) heading -= (2 * PI);
	else if (heading < -PI) heading += (2 * PI);
	else if (heading < 0) heading += 2 * PI;

	// Convert everything from radians to degrees:
	heading 		*= 180.0 / PI;
	pitch 		*= 180.0 / PI;
	roll  		*= 180.0 / PI;

	int16_t i16roll, i16pitch, i16heading;
	i16roll = roll;
	i16pitch = pitch;
	i16heading = heading;

	UARTprintf("Pitch: %4d,",i16pitch);
	UARTprintf("Roll: %4d,",i16roll);
	UARTprintf("Heading: %4d",i16heading);
	UARTprintf(" @degree\n");
}

