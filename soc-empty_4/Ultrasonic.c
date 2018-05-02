#include "ultrasonic.h"

int ultrasonic_read()
{
	GPIO_PinOutClear(TRIGGER_PORT,TRIGGER_PIN);
	UDELAY_Delay(10);
	GPIO_PinOutSet(TRIGGER_PORT,TRIGGER_PIN);
	UDELAY_Delay(10);
	GPIO_PinOutClear(TRIGGER_PORT,TRIGGER_PIN);
	int counter = 0;
	while((counter=GPIO_PinInGet(ECHO_PORT, ECHO_PIN)) == 0)
	{
		;
	}
	int count = 0;
	while(GPIO_PinInGet(ECHO_PORT, ECHO_PIN) != 0)
	{
		count++;
	}
	return count;
}

void gpio_init()
{
	CMU_ClockEnable(cmuClock_HFLE, true);
	//CMU_OscillatorEnable(, true, true);
	//CMU_HFXOAutostartEnable(0, true, true);
	//CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
	GPIO_PinModeSet(TRIGGER_PORT,TRIGGER_PIN,gpioModePushPull, 1);
	GPIO_PinModeSet(ECHO_PORT,ECHO_PIN,gpioModeInputPullFilter, 1);
	GPIO_DriveStrengthSet(TRIGGER_PORT,gpioDriveStrengthStrongAlternateStrong);
	CMU_ClockEnable(cmuClock_GPIO,true);
	CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, true);
	GPIO_PinModeSet(PIN1_PORT,PIN1_PIN,gpioModePushPull, 1);
	GPIO_PinModeSet(PIN2_PORT,PIN2_PIN,gpioModePushPull, 1);
	GPIO_PinModeSet(PIN2_PORT,PIN2_PIN,gpioModePushPull, 1);
	//CMU_ClockEnable(cmuClock_HFPER, true);
}

void motor_drive(uint8_t x, uint8_t y, uint8_t z)
{
	char *dist = (char *)malloc(sizeof(char)*4);
	int distance = 0;
	distance = ultrasonic_read();
	itoa(distance,dist,10);
	//GRAPHICS_Clear();
	GRAPHICS_AppendString(dist);
	GRAPHICS_Update();
	  if(distance<3000)
	  {
		  GPIO_PinOutClear(PIN1_PORT,PIN1_PIN);
		  GPIO_PinOutClear(PIN2_PORT,PIN2_PIN);
		  GPIO_PinOutClear(PIN3_PORT,PIN3_PIN);
			GRAPHICS_Clear();
			GRAPHICS_AppendString("STOP\n");
			GRAPHICS_Update();
	  }
	  else
	  {
		  if((y > 20 && y<100) && (x <15 || x>245))
		  {
		  GPIO_PinOutSet(PIN1_PORT,PIN1_PIN);
		  GPIO_PinOutSet(PIN2_PORT,PIN2_PIN);
		  GPIO_PinOutSet(PIN3_PORT,PIN3_PIN);
			//GRAPHICS_Clear();
			GRAPHICS_AppendString("Running\n");
			GRAPHICS_Update();
		  }
		  else if((y > 180 && y<235) && (x <15 || x>245))
		 		  {
		 		  GPIO_PinOutSet(PIN1_PORT,PIN1_PIN);
		 		  GPIO_PinOutClear(PIN2_PORT,PIN2_PIN);
		 		  GPIO_PinOutClear(PIN3_PORT,PIN3_PIN);
		 			//GRAPHICS_Clear();
		 			GRAPHICS_AppendString("Running\n");
		 			GRAPHICS_Update();
		 		  }
		  else
		  {
			  GPIO_PinOutClear(PIN1_PORT,PIN1_PIN);
			  GPIO_PinOutClear(PIN2_PORT,PIN2_PIN);
			  GPIO_PinOutClear(PIN3_PORT,PIN3_PIN);
		  }
		  }
	  }
