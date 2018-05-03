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
//#include "em_adc.h"

#include "sleep.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ultrasonic.h"

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
#define BFIND_CHAR		4
#define BFIND_SERVICE	5
#define BCHAR_VALUE     6
#define FIND_SERVICE2	7
#define PB0_PORT 		gpioPortF
#define PB1_PORT		gpioPortF
#define PB0_PIN			(6)
#define PB1_PIN			(7)
// custom service UUID: 8f820745123148cebc003f2c7ab8032e
const uint8 serviceUUID[16] = {0x2e, 0x03, 0xb8, 0x7a, 0x2c, 0x3f, 0x00, 0xbc, 0xce, 0x48, 0x31, 0x12, 0x45, 0x07, 0x82, 0x8f};

const uint8 bserviceUUID[2] = {0x0f, 0x18};
const uint8 bcharUUID[3] = {0x19,0x2A};
// SPP data UUID: 8490f48edfdb4f7ab024989d725083ac
const uint8 charUUID1[16] = {0xac, 0x83, 0x50, 0x72, 0x9d, 0x98, 0x24, 0xb0, 0x7a, 0x4f, 0xdb, 0xdf, 0x8e, 0xf4, 0x90, 0x84};

// SPP data UUID: 8490f48edfdb4f7ab024989d725083ac
const uint8 charUUID2[16] = {0x2d, 0xdf, 0xea, 0xa8, 0x97, 0xe4, 0xef, 0xb9, 0x82, 0x4b, 0x55, 0xbf, 0xc9, 0x91, 0x8d, 0x5e};

// SPP data UUID: 8490f48edfdb4f7ab024989d725083ac
const uint8 charUUID3[16] = {0x53, 0x56, 0x66, 0xd0, 0x0c, 0x7a, 0x3b, 0xa6, 0x5c, 0x48, 0x0f, 0xbe, 0x14, 0x7f, 0xe7, 0x70};

/*navigational readings*/
uint8_t x_axis = 0;
uint8_t y_axis = 0;
uint8_t z_axis = 0;

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
static uint32 _service_handle, _bservice_handle;
static uint16 _char_handle1, _char_handle2, _char_handle3, _bchar_handle;

static void reset_variables()
{
	_conn_handle = 0xFF;
	_main_state = DISCONNECTED;
	_service_handle = 0;
	_bservice_handle = 0;
	_char_handle1 = 0;
	_char_handle2 = 0;
	_char_handle3 = 0;
	_bchar_handle = 0;
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


        if (ad_type == 0x02 || ad_type == 0x03)
        {
            // type 0x02 = Incomplete List of 16-bit Service Class UUIDs
            // type 0x03 = Complete List of 16-bit Service Class UUIDs

            // note: this check assumes that the service we are looking for is first
            // in the list. To be fixed so that there is no such limitation...
            if(memcmp(bserviceUUID, &(pResp->data.data[i+2]),2) == 0)
            {
//            				GRAPHICS_Clear();
//            	    	    GRAPHICS_AppendString("pr_sc_res");
//            	    	    GRAPHICS_Update();

            	//printf("Found battery device");
            	ad_match_found = 1;
            }

        }


        //jump to next AD record
        i = i + ad_len + 1;
    }

    return(ad_match_found);
}

void schedule_event(x_axis, y_axis, z_axis)
{
	motor_drive(x_axis, y_axis, z_axis);
}



void main(void)
{
  char printbuf[128];
  char *test;
  char *test1;
  char *test2;
  char *btest;
  int accept = 0;
  int reject = 0;
  char *test3;
  int service_count = 0;
  int count = 0;
  int ser_count = 0;
  int running = 1;
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();
  GPIO_PinModeSet(PB0_PORT,PB0_PIN, gpioModeInput, 1);
  GPIO_PinModeSet(PB1_PORT,PB1_PIN, gpioModeInput, 1);
  // Initialize stack

  gecko_init(&config);

  GRAPHICS_Init();
  gpio_init();
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

     	 gecko_cmd_sm_delete_bondings();

     	 gecko_cmd_sm_configure(0x07, sm_io_capability_displayyesno); /* Numeric comparison */
//
     	 gecko_cmd_sm_set_bondable_mode(1);
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

    	  _main_state = FIND_SERVICE;

      	break;

      case gecko_evt_sm_bonded_id:
    	  //GRAPHICS_Clear();
    	  GRAPHICS_AppendString("\n bonded_id");
      	  GRAPHICS_Update();
     	gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID);
    	  break;

      case gecko_evt_sm_bonding_failed_id:
  	  	  GRAPHICS_Clear();
  	  	  GRAPHICS_AppendString("\n bonding FAILED");
  	  	  GRAPHICS_Update();
  	  	  gecko_cmd_sm_delete_bondings();
  	  	  gecko_cmd_le_gap_discover(le_gap_discover_generic);
  	  	  break;

      case gecko_evt_sm_confirm_passkey_id:
    	  test1 = (char *)malloc(5*sizeof(char));
    	  	itoa(evt->data.evt_sm_confirm_passkey.passkey,test1, 10);
    	  	GRAPHICS_Clear();
    	  	GRAPHICS_AppendString(test1);
    	  	GRAPHICS_Update();
    	  	while((accept = (GPIO_PinInGet(PB0_PORT, PB0_PIN)) == 1) && (reject = (GPIO_PinInGet(PB1_PORT, PB1_PIN)) == 1));
    	  	if(accept == 0)
    	  	{
    	  		gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_passkey.connection,1);
    	  		//GRAPHICS_Clear();
    	  		//GRAPHICS_AppendString("\n Bonding confirmed");
    	  		//GRAPHICS_Update();
    	  	}
                break;

      case gecko_evt_le_connection_closed_id:

          	reset_variables();

          	GRAPHICS_Clear();
          	GRAPHICS_AppendString("Closed\n");
          	GRAPHICS_Update();
          	SLEEP_SleepBlockEnd(sleepEM2); // enable sleeping after disconnect
        	 gecko_cmd_sm_set_bondable_mode(1);
          	gecko_cmd_le_gap_discover(le_gap_discover_generic);
          	gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID);
          	_main_state = FIND_SERVICE;
         break;

       case gecko_evt_le_connection_parameters_id:
         break;

       case gecko_evt_gatt_service_id:
          	if(evt->data.evt_gatt_service.uuid.len == 16)
          	{
          		if(memcmp(serviceUUID, evt->data.evt_gatt_service.uuid.data,16) == 0)
          		{
          			_service_handle = evt->data.evt_gatt_service.service;
          		}
          	}
          	else if(evt->data.evt_gatt_service.uuid.len == 2)
          	          	{
          	          		if(memcmp(bserviceUUID, evt->data.evt_gatt_service.uuid.data,2) == 0)
          	          		{
          	          			_bservice_handle = evt->data.evt_gatt_service.service;
          	          		}
          	          	}
       break;

       case gecko_evt_gatt_procedure_completed_id:

           	switch(_main_state)
           	{
           	case FIND_SERVICE:

           		if (_service_handle > 0)
           		{
           			gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle);
           			_main_state = FIND_CHAR;
           		}
           		else
           		{
           			GRAPHICS_Clear();
           			GRAPHICS_AppendString("STOPPED\n");
           			GRAPHICS_Update();
           			gecko_cmd_endpoint_close(_conn_handle);
           		}
           		break;

           	case FIND_SERVICE2:
           		gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 2, bserviceUUID);
           		_main_state = BFIND_SERVICE;
           		break;
           	case BFIND_SERVICE:
			if(1)
           		{
				gecko_cmd_gatt_discover_characteristics(_conn_handle, _bservice_handle);
           			_main_state = BFIND_CHAR;
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
           			 if(ser_count <3)
           			 {
           				_main_state = FIND_SERVICE;
           				ser_count++;
           			 }
           			 else
           			 {
           				 ser_count = 0;
           				_main_state = FIND_SERVICE2;
           			 }
           	    }
           		else
           		{
           			// no characteristic found? -> disconnect
           			gecko_cmd_endpoint_close(_conn_handle);
           		}
           		break;

           	case BFIND_CHAR:
         		if (1)
           		{
           			// Char found, turn on indications
         			gecko_cmd_gatt_set_characteristic_notification(_conn_handle, _bchar_handle, gatt_notification);
//         			_main_state = BCHAR_VALUE;
         			_main_state = FIND_SERVICE;
           		}
           		else
           		{
           			gecko_cmd_endpoint_close(_conn_handle);
           		}
        		break;
           	default:
           		break;
           	}
           	break;

       case gecko_evt_gatt_characteristic_id:
				//test1 = (char *)malloc(sizeof(char)*16);
          	    //itoa(evt->data.evt_gatt_characteristic.characteristic,test1,10);
    	   if(evt->data.evt_gatt_characteristic.uuid.len == 16)
          		{
          			if(memcmp(charUUID1, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
          			{
          				_char_handle1 = evt->data.evt_gatt_characteristic.characteristic;
          			}
          		}
          else if(evt->data.evt_gatt_characteristic.uuid.len == 2)
          				{
          					if(memcmp(bcharUUID, evt->data.evt_gatt_characteristic.uuid.data,2) == 0)
          			        {	//GRAPHICS_Clear();
          			        	//GRAPHICS_AppendString("recog_char");
          			        	//GRAPHICS_Update();
          						_bchar_handle = evt->data.evt_gatt_characteristic.characteristic;
          			        }
          				}
        break;

          	case gecko_evt_gatt_characteristic_value_id:
          		if(evt->data.evt_gatt_characteristic_value.characteristic == _char_handle1)
          		{
          		  if(running == 1)
          		  {
          			memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
          			printbuf[evt->data.evt_gatt_characteristic_value.value.len] = 0;
          			test1 = (char *)malloc(sizeof(char)*1);
          			test2 = (char *)malloc(sizeof(char)*1);
          			test3 = (char *)malloc(sizeof(char)*1);
          			//itoa(*(evt->data.evt_gatt_characteristic_value.value.data),test1,10);
         			//GRAPHICS_AppendString(test1);
          			//GRAPHICS_Update();
          			count++;
          				if(count==1)
          				{
          					x_axis = *(evt->data.evt_gatt_characteristic_value.value.data);
          					itoa(x_axis,test1,10);
							GRAPHICS_Clear();
							GRAPHICS_AppendString(test1);
							GRAPHICS_Update();
						}
						else if(count==2)
						{
							y_axis = *(evt->data.evt_gatt_characteristic_value.value.data);
							itoa(y_axis,test2,10);
							GRAPHICS_AppendString(test2);
							GRAPHICS_Update();
						}
						else if(count == 3)
						{
							z_axis = *(evt->data.evt_gatt_characteristic_value.value.data);
							itoa(z_axis,test3,10);
							GRAPHICS_AppendString(test3);
							GRAPHICS_Update();
							schedule_event(x_axis, y_axis, z_axis);
							count = 0;
						}
          		}
          	else
          	{
          		GRAPHICS_Clear();
          		GRAPHICS_AppendString("Stop Command\n");
          		GRAPHICS_Update();
          		count = 0;
          	}
          }
          else if(evt->data.evt_gatt_characteristic_value.characteristic == _bchar_handle)
          {
         	memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
         	btest = (char *)malloc(sizeof(char)*1);
         	itoa(*(evt->data.evt_gatt_characteristic_value.value.data),btest,10);
         	running = *(evt->data.evt_gatt_characteristic_value.value.data);
          }
          	break;

      default:
        break;
    }
  }
}
/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
