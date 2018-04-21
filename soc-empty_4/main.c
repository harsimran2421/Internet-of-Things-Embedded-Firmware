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

/* Device initialization header */
#include "hal-config.h"

#include "graphics.h"
#include "em_usart.h"

#include "sleep.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#else
#include "bspconfig.h"
#endif

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

#define DISCONNECTED	0
#define SCANNING		1
#define FIND_SERVICE	2
#define FIND_CHAR		3

// custom service UUID: 8f820745123148cebc003f2c7ab8032e
const uint8 serviceUUID[16] = {0x2e, 0x03, 0xb8, 0x7a, 0x2c, 0x3f, 0x00, 0xbc, 0xce, 0x48, 0x31, 0x12, 0x45, 0x07, 0x82, 0x8f};

// SPP data UUID: 8490f48edfdb4f7ab024989d725083ac
const uint8 charUUID1[16] = {0xac, 0x83, 0x50, 0x72, 0x9d, 0x98, 0x24, 0xb0, 0x7a, 0x4f, 0xdb, 0xdf, 0x8e, 0xf4, 0x90, 0x84};

// SPP data UUID: 8490f48edfdb4f7ab024989d725083ac
const uint8 charUUID2[16] = {0xac, 0x83, 0x50, 0x72, 0x9d, 0x98, 0x24, 0xb0, 0x7a, 0x4f, 0xdb, 0xdf, 0x8e, 0xf4, 0x90, 0x84};

// SPP data UUID: 8490f48edfdb4f7ab024989d725083ac
const uint8 charUUID3[16] = {0xac, 0x83, 0x50, 0x72, 0x9d, 0x98, 0x24, 0xb0, 0x7a, 0x4f, 0xdb, 0xdf, 0x8e, 0xf4, 0x90, 0x84};

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

static uint8 _conn_handle = 0xFF;
static int _main_state;
static uint32 _service_handle;
static uint16 _char_handle1, _char_handle2, _char_handle3;

static void reset_variables()
{
	_conn_handle = 0xFF;
	_main_state = DISCONNECTED;
	_service_handle = 0;
	_char_handle1 = 0;
	_char_handle2 = 0;
	_char_handle3 = 0;
}



static int process_scan_response(struct gecko_msg_le_gap_scan_response_evt_t *pResp)
{
	// decoding advertising packets is done here. The list of AD types can be found
	// at: https://www.bluetooth.com/specifications/assigned-numbers/Generic-Access-Profile

    int i = 0;
    int ad_match_found = 0;
	int ad_len;
    int ad_type;

    char name[32];

    while (i < (pResp->data.len - 1))
    {

        ad_len  = pResp->data.data[i];
        ad_type = pResp->data.data[i+1];

        if (ad_type == 0x08 || ad_type == 0x09 )
        {
            // type 0x08 = Shortened Local Name
            // type 0x09 = Complete Local Name
            memcpy(name, &(pResp->data.data[i+2]), ad_len-1);
            name[ad_len-1] = 0;
        }

        // 4880c12c-fdcb-4077-8920-a450d7f9b907
        if (ad_type == 0x06 || ad_type == 0x07)
        {
        	// type 0x06 = Incomplete List of 128-bit Service Class UUIDs
        	// type 0x07 = Complete List of 128-bit Service Class UUIDs
        	if(memcmp(serviceUUID, &(pResp->data.data[i+2]),16) == 0)
        	{
        		ad_match_found = 1;
        	}
        }

        /*
        if (ad_type == 0x02 || ad_type == 0x03)
        {
            // type 0x02 = Incomplete List of 16-bit Service Class UUIDs
            // type 0x03 = Complete List of 16-bit Service Class UUIDs

            // note: this check assumes that the service we are looking for is first
            // in the list. To be fixed so that there is no such limitation...
            if(memcmp(serviceUUID, &(pResp->data.data[i+2]),2) == 0)
            {
            	printf("Found HTM device");
            	///ad_match_found = 1;
            }

        }
*/

        //jump to next AD record
        i = i + ad_len + 1;
    }

    return(ad_match_found);
}

void main(void)
{
  char printbuf[128];
  char *test;
  char *test1;
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();

  // Initialize stack
  gecko_init(&config);
  GRAPHICS_Init();

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
    	  /*begin scanning */
    	  gecko_cmd_le_gap_discover(le_gap_discover_generic);
        break;

      case gecko_evt_le_gap_scan_response_id:

          	// process scan responses: this function returns 1 if we found the service we are looking for
          	if(process_scan_response(&(evt->data.evt_le_gap_scan_response)) > 0)
          	{
          		struct gecko_msg_le_gap_open_rsp_t *pResp;

          		// match found -> stop discovery and try to connect
          		gecko_cmd_le_gap_end_procedure();

          		pResp = gecko_cmd_le_gap_open(evt->data.evt_le_gap_scan_response.address, evt->data.evt_le_gap_scan_response.address_type);

          		// make copy of connection handle for later use (for example, to cancel the connection attempt)
          		_conn_handle = pResp->connection;

          	}
          break;

      case gecko_evt_le_connection_opened_id:

      	//	 start service discovery (we are only interested in one UUID)
      	gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID);
      	_main_state = FIND_SERVICE;
			GRAPHICS_Clear();
			GRAPHICS_AppendString("Hello World!\n");
			GRAPHICS_Update();
      	break;

      case gecko_evt_le_connection_closed_id:

          	reset_variables();

          	SLEEP_SleepBlockEnd(sleepEM2); // enable sleeping after disconnect

          	gecko_cmd_le_gap_discover(le_gap_discover_generic);
         break;

       case gecko_evt_le_connection_parameters_id:
         break;

       case gecko_evt_gatt_service_id:
          	if(evt->data.evt_gatt_service.uuid.len == 16)
          	{
          		if(memcmp(serviceUUID, evt->data.evt_gatt_service.uuid.data,16) == 0)
          		{
//          			GRAPHICS_AppendString("Sasuke\n");
//          			GRAPHICS_Update();
          			_service_handle = evt->data.evt_gatt_service.service;
          		}
          	}
       break;

       case gecko_evt_gatt_procedure_completed_id:

           	switch(_main_state)
           	{
           	case FIND_SERVICE:

           		if (_service_handle > 0)
           		{
           			// Service found, next search for characteristics
           			gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle);
           			_main_state = FIND_CHAR;
           		}
           		else
           		{
           			// no service found -> disconnect
           			gecko_cmd_endpoint_close(_conn_handle);
           		}

           		break;

           	case FIND_CHAR:
           		if (_char_handle1 > 0)
           		{
           			// Char found, turn on indications
           			gecko_cmd_gatt_set_characteristic_notification(_conn_handle, _char_handle1, gatt_notification);
           		}
           		if (_char_handle2 > 0)
           		{
           			// Char found, turn on indications
           			gecko_cmd_gatt_set_characteristic_notification(_conn_handle, _char_handle2, gatt_notification);
           		}
           		if (_char_handle3 > 0)
           		{
           			// Char found, turn on indications
           			gecko_cmd_gatt_set_characteristic_notification(_conn_handle, _char_handle3, gatt_notification);
           		}
           		else
           		{
           			// no characteristic found? -> disconnect
           			gecko_cmd_endpoint_close(_conn_handle);
           		}
           		break;

           	default:
           		break;
           	}
           	break;

       case gecko_evt_gatt_characteristic_id:
    	  test = (char *)malloc(sizeof(char)*16);
    	  itoa(evt->data.evt_gatt_characteristic.uuid.len,test,10);
    	  GRAPHICS_AppendString(test);
    	  GRAPHICS_Update();
          		if(evt->data.evt_gatt_characteristic.uuid.len == 16)
          		{
          			//GRAPHICS_Clear();
//          			GRAPHICS_AppendString("Naruto\n");
//          			GRAPHICS_Update();
          			if(memcmp(charUUID1, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
          			{
          				GRAPHICS_AppendString("Characteristic 1\n");
          				GRAPHICS_Update();
          				_char_handle1 = evt->data.evt_gatt_characteristic.characteristic;
          			}
          			if(memcmp(charUUID2, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
          			{
          				GRAPHICS_AppendString("Characteristic 2\n");
          				GRAPHICS_Update();
          			    _char_handle2 = evt->data.evt_gatt_characteristic.characteristic;
          			          			}
          			if(memcmp(charUUID3, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
          			          			{
          			   GRAPHICS_AppendString("Characteristic 3\n");
          			   GRAPHICS_Update();
          			   _char_handle3 = evt->data.evt_gatt_characteristic.characteristic;
          			}

          		}

        break;

          	case gecko_evt_gatt_characteristic_value_id:
//          		GRAPHICS_AppendString("Entered characteristic value\n");
//          		          				GRAPHICS_Update();
          		if(evt->data.evt_gatt_characteristic_value.characteristic == _char_handle1)
          		{
          			// data received from SPP server -> print to UART
          			// NOTE: this works only with text (no binary) because printf() expects null-terminated strings as input
          			memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
          			printbuf[evt->data.evt_gatt_characteristic_value.value.len] = 0;
          			test1 = (char *)malloc(sizeof(char)*1);
          			itoa(*(evt->data.evt_gatt_characteristic_value.value.data),test1,10);
          			GRAPHICS_AppendString(test1);
          			GRAPHICS_Update();
          		}
          		if(evt->data.evt_gatt_characteristic_value.characteristic == _char_handle2)
          		{
          			// data received from SPP server -> print to UART
          			// NOTE: this works only with text (no binary) because printf() expects null-terminated strings as input
          			memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
          			printbuf[evt->data.evt_gatt_characteristic_value.value.len] = 0;
          			test1 = (char *)malloc(sizeof(char)*1);
          			itoa(*(evt->data.evt_gatt_characteristic_value.value.data),test1,10);
          			GRAPHICS_AppendString(test1);
          			GRAPHICS_Update();
          		}
          		if(evt->data.evt_gatt_characteristic_value.characteristic == _char_handle3)
          		{
          			// data received from SPP server -> print to UART
          			// NOTE: this works only with text (no binary) because printf() expects null-terminated strings as input
          			memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
          			printbuf[evt->data.evt_gatt_characteristic_value.value.len] = 0;
          			test1 = (char *)malloc(sizeof(char)*1);
          			itoa(*(evt->data.evt_gatt_characteristic_value.value.data),test1,10);
          			GRAPHICS_Clear();
          			GRAPHICS_AppendString(test1);
          			GRAPHICS_Update();
          		}
          	break;

      default:
        break;
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
