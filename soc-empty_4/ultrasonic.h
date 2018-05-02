#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "udelay.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include "em_gpio.h"

#define ECHO_PORT gpioPortD
#define TRIGGER_PORT gpioPortD

#define ECHO_PIN 10
#define TRIGGER_PIN 12

#define PIN1_PORT gpioPortA
#define PIN2_PORT gpioPortA
#define PIN3_PORT gpioPortD

#define PIN1_PIN 2
#define PIN2_PIN 3
#define PIN3_PIN 11

int ultrasonic_read();

void gpio_init();

void motor_drive(uint8_t x, uint8_t y, uint8_t z);

void schedule_event(int x_axis,int y_axis,int z_axis);


#endif /* ULTRASONIC_H_ */
