/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
//#include "em_gpio.h"
#include "gpio.h"
//#include "gpiointerrupt.h"
//#include "em_adc.h"

/* Device initialization header */
#include "hal-config.h"

#include "em_usart.h"
#include "mx25flash_spi.h"
#include "graphics.h"
//#include "retargetserial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif


#define ECHO_PORT gpioPortD
#define TRIGGER_PORT gpioPortD

#define ECHO_PIN 10
#define TRIGGER_PIN 12
#define PB0_PORT 		gpioPortF
#define PB1_PORT		gpioPortF
#define PB0_PIN			(6)
#define PB1_PIN			(7)
/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

/**
 * @brief  Main function
 */
void spi_init()
{

  // EMLIB documentation online:

  // PIN PC9 = SPI_CS
  GPIO_PinModeSet(gpioPortC, 9, gpioModePushPull, 1);

  // PIN PC8 = SPI_SCK
  GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 1);

  // PIN PC7 = SPI_MISO
  GPIO_PinModeSet(gpioPortC, 7, gpioModeInput, 0);

  // PIN PC6 = SPI_MOSI
  GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 1);


  USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;

  CMU_ClockEnable(cmuClock_HFPER, true);		// Enable HF peripheral clock
  CMU_ClockEnable(cmuClock_USART1, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  config.enable = usartDisable;
  config.baudrate = 100000; /* 100 kbits/s. */
  config.clockMode = usartClockMode3;   /* Clock idle high, sample on rising edge. */
  config.msbf = true; /* Send most significant bit first. */


  config.autoCsEnable = true;

  USART_InitSync(USART1, &config);

  // USART1 signal routing: (see EFR32BG datas sheet)
  // US0_CLK: location 11 -> PC8
  // US0_CS: location 11 -> PC9
  // US0_RX: location 11 -> PC7
  // US0_TX: location 11 -> PC6

  USART1->ROUTELOC0 = USART_ROUTELOC0_CLKLOC_LOC11 | USART_ROUTELOC0_TXLOC_LOC11 | USART_ROUTELOC0_RXLOC_LOC11 | USART_ROUTELOC0_CSLOC_LOC11;
  USART1->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_CSPEN;

  USART_Enable(USART1, usartEnable);

}

uint8_t ACC_Read(USART_TypeDef *usart, uint8_t offset)
{
	uint16_t data;
	uint32_t tmp;

	data = 0x0080; // note: byte order when sending is 0x80 0x00 . MSB is set to indicate read for BMA280 chip

	data = data | offset;  // set offset field

  while (!(usart->STATUS & USART_STATUS_TXBL))
    ;
  usart->TXDOUBLE = (uint32_t)data;
  while (!(usart->STATUS & USART_STATUS_TXC))
    ;

  tmp = usart->RXDOUBLE;
  tmp = tmp >> 8;
  return (uint8_t)tmp;

}

void ACC_readings()
{
	uint8_t x_axis;
	uint8_t y_axis;
	uint8_t z_axis;
	uint8_t battery;
	spi_init();
	x_axis = 0xAA;
	y_axis = 0xAA;
	z_axis = 0xAA;

	x_axis = ACC_Read(USART1, 0x03);
	gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_X_Axis_Measured_Value, 1, &x_axis);

	UDELAY_Delay(2);
	y_axis = ACC_Read(USART1, 0x05);
	gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_X_Axis_Measured_Value, 1, &y_axis);

	UDELAY_Delay(2);
	z_axis = ACC_Read(USART1, 0x07);
	gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_X_Axis_Measured_Value, 1, &z_axis);

	UDELAY_Delay(20);

	gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_battery_level, 1, &running);
}

void gpio_init()
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInput, 0);
	GPIO_IntConfig(PB0_PORT, PB0_PIN , TRUE, true, true);
	GPIO_IntConfig(PB1_PORT, PB1_PIN , TRUE, true, true);
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void GPIO_ODD_IRQHandler(void)
{
	__disable_irq();
	GPIO_IntClear(GPIO_IntGet());
    running = 1;
    __enable_irq();
}

void GPIO_EVEN_IRQHandler(void)
{
	__disable_irq();
	GPIO_IntClear(GPIO_IntGet());
	running = 0;
	__enable_irq();
}


void main(void)
{
	char *test;
	char *test1;
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();
  int accept = 0;
  // Initialize stack
  gecko_init(&config);

  spi_init();
  gpio_init();
  GPIO_PinModeSet(PB0_PORT,PB0_PIN, gpioModeInput, 1);
  GPIO_PinModeSet(PB1_PORT,PB1_PIN, gpioModeInput, 1);
  struct gecko_msg_le_connection_opened_evt_t * activeConnectionId;

  while (1) {
    /* Event pointer for handling events */
    struct gecko_cmd_packet* evt;

    /* Check for stack event. */
    evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {
      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

    	  /*delete all the bonding information on Booting up*/
    	 gecko_cmd_sm_delete_bondings();

    	 /*to configure the bonding details required*/
    	 gecko_cmd_sm_configure(0x07, sm_io_capability_displayyesno); /* Numeric comparison */

    	 gecko_cmd_sm_set_bondable_mode(1);
        /* Set advertising parameters. 100ms advertisement interval. All channels used.
         * The first two parameters are minimum and maximum advertising interval, both in
         * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
        gecko_cmd_le_gap_set_adv_parameters(160, 160, 7);

        /* Start general advertising and enable connections. */
        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        gecko_cmd_hardware_set_soft_timer(16384/*32768*/, 0, 0);
        break;
      case gecko_evt_le_connection_opened_id:

    	  	 /* Store the connection ID */
    	  	 activeConnectionId = evt->data.evt_le_connection_opened.connection;
    	  	gecko_cmd_le_connection_set_parameters(evt->data.evt_le_connection_opened.connection, 0X3C,0X3C,0X5,0X6E);
    	  	 /* force encryption immediately after connecting */
    	  	 gecko_cmd_sm_increase_security(activeConnectionId);
    	  	  break;

      case gecko_evt_sm_confirm_passkey_id:
    	  	test1 = (char *)malloc(5*sizeof(char));
    	  	itoa(evt->data.evt_sm_confirm_passkey.passkey,test1, 10);
    	  	GRAPHICS_Init();
    	  	GRAPHICS_Clear();
    	  	GRAPHICS_AppendString(test1);
    	  	GRAPHICS_Update();
    	  	/*command to confirm the Recieved passkey*/
    	  	//while((accept = (GPIO_PinInGet(PB0_PORT, PB0_PIN)) == 1) && (reject = (GPIO_PinInGet(PB1_PORT, PB1_PIN)) == 1));
    	  	//if(accept == 0)
    	  	//{
    	  		gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_passkey.connection,1);
        	  	GRAPHICS_AppendString("\n confirmed");
        	  	GRAPHICS_Update();
    	  	//}
    	  	/*To Print that the passkey is conform*/
                break;

   /*Timer case to trigger the Accelerometer read function after every 1 second*/
      case gecko_evt_hardware_soft_timer_id:
    	  if(accept == 10)
    	  {
    		  ACC_readings();
    	  }
          break;
      case gecko_evt_sm_bonded_id:
    	  GRAPHICS_Init();
    	  GRAPHICS_Clear();
    	  GRAPHICS_AppendString("\n confirmed");
    	  GRAPHICS_Update();
    	  accept = 10;
    	  break;

      case gecko_evt_sm_bonding_failed_id:
    	  GRAPHICS_Init();
    	  GRAPHICS_Clear();
    	  GRAPHICS_AppendString("\n FAILED");
    	  GRAPHICS_Update();
    	  accept = 0;
    	  gecko_cmd_sm_delete_bondings();
    	   gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
    	  break;
  /*Case when the bluetooth connection is terminated*/
      case gecko_evt_le_connection_closed_id:
    	GRAPHICS_Init();
    	GRAPHICS_Clear();
        /* Check if need to boot to dfu mode */
        if (boot_to_dfu) {
          /* Enter to DFU OTA mode */
          gecko_cmd_system_reset(2);
        } else {
          /* Restart advertising after client has disconnected */
          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
        }
        break;

      /* Events related to OTA upgrading
         ----------------------------------------------------------------------------- */

      /* Check if the user-type OTA Control Characteristic was written.
       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
      case gecko_evt_gatt_server_user_write_request_id:

        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
          /* Set flag to enter to OTA mode */
          boot_to_dfu = 1;
          /* Send response to Write Request */
          gecko_cmd_gatt_server_send_user_write_response(
            evt->data.evt_gatt_server_user_write_request.connection,
            gattdb_ota_control,
            bg_err_success);

          /* Close connection to enter to DFU OTA mode */
          gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
        }
        break;

      default:
        break;
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
