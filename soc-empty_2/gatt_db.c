// Copyright 2018 Silicon Laboratories, Inc.
//
//

/********************************************************************
 * Autogenerated file, do not edit.
 *******************************************************************/

#include <stdint.h>
#include "bg_gattdb_def.h"

#ifdef __GNUC__
#define GATT_HEADER(F) F __attribute__ ((section (".gatt_header"))) 
#define GATT_DATA(F) F __attribute__ ((section (".gatt_data"))) 
#else
#ifdef __ICCARM__
#define GATT_HEADER(F) _Pragma("location=\".gatt_header\"") F 
#define GATT_DATA(F) _Pragma("location=\".gatt_data\"") F 
#else
#define GATT_HEADER(F) F 
#define GATT_DATA(F) F 
#endif
#endif

GATT_DATA(const uint16_t bg_gattdb_data_uuidtable_16_map [])=
{
    0x2800,
    0x2801,
    0x2803,
    0x1800,
    0x2a00,
    0x2a01,
    0x180a,
    0x2a29,
    0x2a24,
    0x2a23,
    0x1801,
    0x2a05,
    0x2902,
};

GATT_DATA(const uint8_t bg_gattdb_data_uuidtable_128_map [])=
{
0xf0, 0x19, 0x21, 0xb4, 0x47, 0x8f, 0xa4, 0xbf, 0xa1, 0x4f, 0x63, 0xfd, 0xee, 0xd6, 0x14, 0x1d, 
0x63, 0x60, 0x32, 0xe0, 0x37, 0x5e, 0xa4, 0x88, 0x53, 0x4e, 0x6d, 0xfb, 0x64, 0x35, 0xbf, 0xf7, 
0x2e, 0x03, 0xb8, 0x7a, 0x2c, 0x3f, 0x00, 0xbc, 0xce, 0x48, 0x31, 0x12, 0x45, 0x07, 0x82, 0x8f, 
0xac, 0x83, 0x50, 0x72, 0x9d, 0x98, 0x24, 0xb0, 0x7a, 0x4f, 0xdb, 0xdf, 0x8e, 0xf4, 0x90, 0x84, 
0x2d, 0xdf, 0xea, 0xa8, 0x97, 0xe4, 0xef, 0xb9, 0x82, 0x4b, 0x55, 0xbf, 0xc9, 0x91, 0x8d, 0x5e, 
0x53, 0x56, 0x66, 0xd0, 0x0c, 0x7a, 0x3b, 0xa6, 0x5c, 0x48, 0x0f, 0xbe, 0x14, 0x7f, 0xe7, 0x70, 
};




uint8_t bg_gattdb_data_attribute_field_27_data[1]={0x00,};
GATT_DATA(const struct bg_gattdb_attribute_chrvalue	bg_gattdb_data_attribute_field_27 ) = {
	.properties=0x10,
	.index=5,
	.max_len=1,
	.data=bg_gattdb_data_attribute_field_27_data,
};

GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_26 ) = {
	.len=19,
	.data={0x10,0x1c,0x00,0x53,0x56,0x66,0xd0,0x0c,0x7a,0x3b,0xa6,0x5c,0x48,0x0f,0xbe,0x14,0x7f,0xe7,0x70,}
};
uint8_t bg_gattdb_data_attribute_field_24_data[1]={0x00,};
GATT_DATA(const struct bg_gattdb_attribute_chrvalue	bg_gattdb_data_attribute_field_24 ) = {
	.properties=0x10,
	.index=4,
	.max_len=1,
	.data=bg_gattdb_data_attribute_field_24_data,
};

GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_23 ) = {
	.len=19,
	.data={0x10,0x19,0x00,0x2d,0xdf,0xea,0xa8,0x97,0xe4,0xef,0xb9,0x82,0x4b,0x55,0xbf,0xc9,0x91,0x8d,0x5e,}
};
uint8_t bg_gattdb_data_attribute_field_21_data[1]={0x00,};
GATT_DATA(const struct bg_gattdb_attribute_chrvalue	bg_gattdb_data_attribute_field_21 ) = {
	.properties=0x12,
	.index=3,
	.max_len=1,
	.data=bg_gattdb_data_attribute_field_21_data,
};

GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_20 ) = {
	.len=19,
	.data={0x12,0x16,0x00,0xac,0x83,0x50,0x72,0x9d,0x98,0x24,0xb0,0x7a,0x4f,0xdb,0xdf,0x8e,0xf4,0x90,0x84,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_19 ) = {
	.len=16,
	.data={0x2e,0x03,0xb8,0x7a,0x2c,0x3f,0x00,0xbc,0xce,0x48,0x31,0x12,0x45,0x07,0x82,0x8f,}
};
GATT_DATA(const struct bg_gattdb_attribute_chrvalue	bg_gattdb_data_attribute_field_18 ) = {
	.properties=0x08,
	.index=2,
	.max_len=0,
	.data=NULL,
};

GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_17 ) = {
	.len=19,
	.data={0x08,0x13,0x00,0x63,0x60,0x32,0xe0,0x37,0x5e,0xa4,0x88,0x53,0x4e,0x6d,0xfb,0x64,0x35,0xbf,0xf7,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_16 ) = {
	.len=16,
	.data={0xf0,0x19,0x21,0xb4,0x47,0x8f,0xa4,0xbf,0xa1,0x4f,0x63,0xfd,0xee,0xd6,0x14,0x1d,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_15 ) = {
	.len=6,
	.data={0x00,0x01,0x02,0x03,0x04,0x05,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_14 ) = {
	.len=5,
	.data={0x02,0x10,0x00,0x23,0x2a,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_13 ) = {
	.len=10,
	.data={0x42,0x6c,0x75,0x65,0x20,0x47,0x65,0x63,0x6b,0x6f,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_12 ) = {
	.len=5,
	.data={0x02,0x0e,0x00,0x24,0x2a,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_11 ) = {
	.len=12,
	.data={0x53,0x69,0x6c,0x69,0x63,0x6f,0x6e,0x20,0x4c,0x61,0x62,0x73,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_10 ) = {
	.len=5,
	.data={0x02,0x0c,0x00,0x29,0x2a,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_9 ) = {
	.len=2,
	.data={0x0a,0x18,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_8 ) = {
	.len=2,
	.data={0x00,0x00,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_7 ) = {
	.len=5,
	.data={0x02,0x09,0x00,0x01,0x2a,}
};
uint8_t bg_gattdb_data_attribute_field_6_data[13]={0x45,0x6d,0x70,0x74,0x79,0x20,0x45,0x78,0x61,0x6d,0x70,0x6c,0x65,};
GATT_DATA(const struct bg_gattdb_attribute_chrvalue	bg_gattdb_data_attribute_field_6 ) = {
	.properties=0x0a,
	.index=1,
	.max_len=13,
	.data=bg_gattdb_data_attribute_field_6_data,
};

GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_5 ) = {
	.len=5,
	.data={0x0a,0x07,0x00,0x00,0x2a,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_4 ) = {
	.len=2,
	.data={0x00,0x18,}
};
uint8_t bg_gattdb_data_attribute_field_2_data[4]={0x00,0x00,0x00,0x00,};
GATT_DATA(const struct bg_gattdb_attribute_chrvalue	bg_gattdb_data_attribute_field_2 ) = {
	.properties=0x20,
	.index=0,
	.max_len=4,
	.data=bg_gattdb_data_attribute_field_2_data,
};

GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_1 ) = {
	.len=5,
	.data={0x20,0x03,0x00,0x05,0x2a,}
};
GATT_DATA(const struct bg_gattdb_buffer_with_len	bg_gattdb_data_attribute_field_0 ) = {
	.len=2,
	.data={0x01,0x18,}
};
GATT_DATA(const struct bg_gattdb_attribute bg_gattdb_data_attributes_map[])={
    {.uuid=0x0000,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_0},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_1},
    {.uuid=0x000b,.permissions=0x800,.caps=0xffff,.datatype=0x01,.min_key_size=0x00,.dynamicdata=&bg_gattdb_data_attribute_field_2},
    {.uuid=0x000c,.permissions=0x807,.caps=0xffff,.datatype=0x03,.min_key_size=0x00,.configdata={.flags=0x02,.index=0x00,.clientconfig_index=0x00}},
    {.uuid=0x0000,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_4},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_5},
    {.uuid=0x0004,.permissions=0x803,.caps=0xffff,.datatype=0x01,.min_key_size=0x00,.dynamicdata=&bg_gattdb_data_attribute_field_6},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_7},
    {.uuid=0x0005,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_8},
    {.uuid=0x0000,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_9},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_10},
    {.uuid=0x0007,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_11},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_12},
    {.uuid=0x0008,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_13},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_14},
    {.uuid=0x0009,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_15},
    {.uuid=0x0000,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_16},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_17},
    {.uuid=0x8001,.permissions=0x802,.caps=0xffff,.datatype=0x07,.min_key_size=0x00,.dynamicdata=&bg_gattdb_data_attribute_field_18},
    {.uuid=0x0000,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_19},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_20},
    {.uuid=0x8003,.permissions=0x821,.caps=0xffff,.datatype=0x01,.min_key_size=0x00,.dynamicdata=&bg_gattdb_data_attribute_field_21},
    {.uuid=0x000c,.permissions=0x827,.caps=0xffff,.datatype=0x03,.min_key_size=0x00,.configdata={.flags=0x01,.index=0x03,.clientconfig_index=0x01}},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_23},
    {.uuid=0x8004,.permissions=0x800,.caps=0xffff,.datatype=0x01,.min_key_size=0x00,.dynamicdata=&bg_gattdb_data_attribute_field_24},
    {.uuid=0x000c,.permissions=0x807,.caps=0xffff,.datatype=0x03,.min_key_size=0x00,.configdata={.flags=0x01,.index=0x04,.clientconfig_index=0x02}},
    {.uuid=0x0002,.permissions=0x801,.caps=0xffff,.datatype=0x00,.min_key_size=0x00,.constdata=&bg_gattdb_data_attribute_field_26},
    {.uuid=0x8005,.permissions=0x800,.caps=0xffff,.datatype=0x01,.min_key_size=0x00,.dynamicdata=&bg_gattdb_data_attribute_field_27},
    {.uuid=0x000c,.permissions=0x807,.caps=0xffff,.datatype=0x03,.min_key_size=0x00,.configdata={.flags=0x01,.index=0x05,.clientconfig_index=0x03}},
};

GATT_DATA(const uint16_t bg_gattdb_data_attributes_dynamic_mapping_map[])={
	0x0003,
	0x0007,
	0x0013,
	0x0016,
	0x0019,
	0x001c,
};

GATT_DATA(const uint8_t bg_gattdb_data_adv_uuid16_map[])={0x0};
GATT_DATA(const uint8_t bg_gattdb_data_adv_uuid128_map[])={0x2e, 0x03, 0xb8, 0x7a, 0x2c, 0x3f, 0x00, 0xbc, 0xce, 0x48, 0x31, 0x12, 0x45, 0x07, 0x82, 0x8f, };
GATT_HEADER(const struct bg_gattdb_def bg_gattdb_data)={
    .attributes=bg_gattdb_data_attributes_map,
    .attributes_max=29,
    .uuidtable_16_size=13,
    .uuidtable_16=bg_gattdb_data_uuidtable_16_map,
    .uuidtable_128_size=6,
    .uuidtable_128=bg_gattdb_data_uuidtable_128_map,
    .attributes_dynamic_max=6,
    .attributes_dynamic_mapping=bg_gattdb_data_attributes_dynamic_mapping_map,
    .adv_uuid16=bg_gattdb_data_adv_uuid16_map,
    .adv_uuid16_num=0,
    .adv_uuid128=bg_gattdb_data_adv_uuid128_map,
    .adv_uuid128_num=1,
    .caps_mask=0xffff,
    .enabled_caps=0xffff,
};

const struct bg_gattdb_def *bg_gattdb=&bg_gattdb_data;
