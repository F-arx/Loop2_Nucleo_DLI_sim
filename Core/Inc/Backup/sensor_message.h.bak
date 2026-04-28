/*
 * sensor_message.h
 *
 *  Created on: Feb 26, 2026
 *      Author: Fabio
 *
 *  Sensor messaging system - Data structures, events, and I2C communication
 */

#ifndef SENSOR_MESSAGE_H
#define SENSOR_MESSAGE_H


#include <stdint.h>
/* ========================= I2C EVfENTS ========================= */
/* Events related to I2C communication callbacks (receiver, transmitter, error) */
/* Enumeration only - no extern needed */

typedef enum {
	I2C_EVENT_NONE        = 0,
	I2C_EVENT_RX_COMPLETE = 1 << 0,  /* I2C data received */
	I2C_EVENT_TX_COMPLETE = 1 << 1,  /* I2C data transmitted */
	I2C_EVENT_ERROR       = 1 << 2   /* I2C error occurred */
} I2C_Event_t;

/* ========================= BUTTON EVENTS ========================= */
/* Events related to button presses and EXTI interrupts */
/* Enumeration only - no extern needed */

typedef enum {
	EVENT_NONE                  = 0,
	EVENT_BTN_NEXT              = 1 << 3,  /* Button NEXT pressed */
	EVENT_BTN_PREV              = 1 << 4,  /* Button PREV pressed */
	EVENT_BTN_CHANGE_STATUS     = 1 << 5,  /* Button STATUS pressed */
	EVENT_BTN_VALIDATE          = 1 << 6,  /* Button VALIDATE pressed (generic button event) */
	EVENT_BTN_RESET_ALL_STATUS  = 1 << 7,  /* Button RESET pressed */
	EVENT_BTN_SPECIAL_SEL       = 1 << 8,  /* Button special select pressed */
	EVENT_BTN_SPECIAL_VAL       = 1 << 9   /* Button special validate pressed */
} Event_BTN_t;

/* ====================== DETECTOR TYPE ENUMS ===================== */
/* Enumerations only - no extern needed */

// Detector type (bits 0-2)
typedef enum {
  SHOP_MONITOR        = 0x00, // 000
  SOUNDER_CONTROLLER  = 0x01, // 001
  IO_UNIT             = 0x02, // 010
  IONISATION_DETECTOR = 0x03, // 011
  ZONE_MONITOR        = 0x04, // 100
  OPTICAL_SMOKE       = 0x05, // 101
  HEAT_DETECTOR       = 0x06, // 110
  MANUAL_CALLPOINT    = 0x07  // 111
} DetectorType;

typedef enum {
  NO_IO_ALARM = 0,
  IO_ALARM_ACTIVE = 1
} AlarmFromIOUnit;

typedef enum {
  NO_LIMIT_ALARM = 0,
  LIMIT_ALARM_ACTIVE = 1
} LimitAlarm;

typedef enum {
  NOT_DISCOVERY_MULTISENSOR = 0,
  DISCOVERY_MULTISENSOR = 1
} DiscoveryMultisensor;

typedef enum {
  NOT_DISCOVERY_DEVICE = 0,
  DISCOVERY_DEVICE = 1
} DiscoveryDevice;

/* ======================== DATA STRUCTURES ======================== */

typedef struct {
  DetectorType type;
  AlarmFromIOUnit alarm_from_io;
  LimitAlarm limit_alarm;
  DiscoveryMultisensor discovery_multi;
  DiscoveryDevice discovery_device;
} DetectorProfile;



typedef struct {
    uint8_t address;
    uint8_t type;
    uint8_t status;
} Message_t;

//typedef uint8_t Message_t[3];

extern const uint8_t special_messages_count;





/* ======================== DATA STRUCTURES ======================== */
/* Structure definitions only - no extern needed */

/* ==================== SPECIAL MESSAGE ENUMS ==================== */
/* Enumeration only - no extern needed */

typedef enum {
	SPECIAL_MESSAGE_NONE = 0,
    BROKEN_WIRE_STANDBY_LOOP1,
    BROKEN_WIRE_STANDBY_LOOP2,
    BROKEN_WIRE_LOOP1,
    BROKEN_WIRE_LOOP2,
    SHORT_STANDBY_LOOP1,
    SHORT_STANDBY_LOOP2,
    SHORT_LOOP1,
    SHORT_LOOP2,
    SHORT_STANDBY_CONN1,
    SHORT_STANDBY_CONN2,
    SHORT_STANDBY_CONN3,
    SHORT_STANDBY_CONN4,
    SHORT_FAULT_CONN1,
    SHORT_FAULT_CONN2,
    SHORT_FAULT_CONN3,
    SHORT_FAULT_CONN4,
    SHIELDING_STANDBY_CONN1,
    SHIELDING_STANDBY_CONN2,
    SHIELDING_STANDBY_CONN3,
    SHIELDING_STANDBY_CONN4,
    SHIELDING_STANDBY_CONN5,
    SHIELDING_STANDBY_CONN6,
    SHIELDING_STANDBY_CONN7,
    SHIELDING_STANDBY_CONN8,
    SHIELDING_FAULT_CONN1,
    SHIELDING_FAULT_CONN2,
    SHIELDING_FAULT_CONN3,
    SHIELDING_FAULT_CONN4,
    SHIELDING_FAULT_CONN5,
    SHIELDING_FAULT_CONN6,
    SHIELDING_FAULT_CONN7,
    SHIELDING_FAULT_CONN8,
    EEPROM_STANDBY = 32,
    EEPROM_FAULT,
    VOLTAGE_STANDBY,
    VOLTAGE_FAULT,
    INTERNAL_STANDBY,
    INTERNAL_FAULT,
    WIRELESS_STANDBY,
    WIRELESS_FAULT,
    SPECIAL_MESSAGE_MAX
} SpecialMessageIndex;

/* ===================== I2C COMMUNICATION BUFFERS ==================== */

extern uint8_t rxCmd[2];
extern Message_t tx_0;

extern Message_t buffer_tx_end;
extern Message_t *buffer_tx;
extern Message_t *msg_to_send;
/* ===================== I2C COMMUNICATION FLAGS ===================== */
/* Use: extern volatile - modified by ISR, don't optimize away */
// Flags and state used by I2C/communication
extern volatile uint8_t buffer_ready;
extern volatile uint8_t i2c_tx_flag;
extern volatile uint8_t i2c_rx_flag;
extern volatile uint8_t i2c_err_flag;
extern volatile uint32_t i2c_last_error;
extern volatile uint8_t tx_step;
extern uint8_t last_tx_step;

/* ======================== SENSOR DATA VALUES ======================== */

extern uint8_t type_of_sensor;
extern uint8_t init_status_of_sensor;
extern uint8_t id;
extern uint8_t sensor_data[3];
extern DetectorProfile detector_status;
extern uint8_t current_tx_index;
//extern uint16_t selected_point_index;
extern SpecialMessageIndex selected_special_message_index;

/* ========================== SPECIAL MESSAGES ========================== */

extern const Message_t special_messages[];
extern SpecialMessageIndex special_message_index;
extern uint16_t counter_special_message;
extern volatile uint8_t special_message_pending; /* Flag: 1 if special message waiting to send, 0 otherwise */

/* ========================== EXECUTION STATE ========================== */

typedef enum {
	step_none = 0,
	step_init,
	step_loop,
	step_max
} step_t;

extern step_t step; // voir pour supprimer cette commande

/* ======================= COMMANDS FROM MASTER ======================= */

typedef enum { // list of commands from master to sensor, based on DBM013 documentation

    // CONFIRMATIONS
    COMMAND_PRE_ALARM_CONFIRMATION        = 0x2C,
    COMMAND_ALARM_CONFIRMATION_1          = 0x2E,
    COMMAND_ALARM_CONFIRMATION_2          = 0x40,

    // LINE CONTROL
    COMMAND_ADDRESS_DISABLED              = 0x80,
    COMMAND_SWITCH_OFF_LIMIT_LINE         = 0x81,
    COMMAND_SWITCH_ON_LIMIT_LINE          = 0x82,

    // RESET
    COMMAND_RESET_ALARM_LINE              = 0x91,
    COMMAND_RESET_GROUP_ALARM             = 0x92,
    COMMAND_RESET_SIGNALLING_UNIT         = 0x93,
    COMMAND_GROUP_RESET_LIMIT_CARD        = 0x94,

    // SENSITIVITY NO FLASH
    COMMAND_SENSITIVITY_LEVEL_1_NO_FLASH  = 0x97,
    COMMAND_SENSITIVITY_LEVEL_2_NO_FLASH  = 0x98,
    COMMAND_SENSITIVITY_LEVEL_3_NO_FLASH  = 0x99,
    COMMAND_SENSITIVITY_LEVEL_4_NO_FLASH  = 0x9A,
    COMMAND_SENSITIVITY_LEVEL_5_NO_FLASH  = 0x9B,

    // INSPECTION / ADDRESS
    COMMAND_READ_ADDRESS_INSPECTION_DATE  = 0x9C,
    COMMAND_SET_INSPECTION_DATE           = 0x9D,

    // RELAY
    COMMAND_ACTIVATE_RELAY                = 0xA1,
    COMMAND_RESET_RELAY                   = 0xA7,

    // SINGLE READ
    COMMAND_DETECTOR_TEST                 = 0xB1,
    COMMAND_READ_MANUFACTURE_DATE         = 0xB4,
    COMMAND_READ_CONTAMINATION            = 0xB5,
    COMMAND_READ_SENSITIVITY              = 0xB6,
    COMMAND_READ_INSPECTION_DATE          = 0xB7,

    // MULTI READ
    COMMAND_MULTI_READ_MANUFACTURE_DATE   = 0xB8,
    COMMAND_MULTI_READ_CONTAMINATION      = 0xB9,
    COMMAND_MULTI_READ_SENSITIVITY        = 0xBA,
    COMMAND_MULTI_READ_INSPECTION_DATE    = 0xBB,
    COMMAND_MULTI_READ_END                = 0xBC,

    // IMPORT
    COMMAND_IMPORT_DETECTOR               = 0xC0,

    // EXPLOSION
    COMMAND_EXPLOSION_LIMIT_LINE_OFF      = 0xD0,
    COMMAND_EXPLOSION_LIMIT_LINE_ON       = 0xD1,

    // SENSITIVITY FLASH
    COMMAND_SENSITIVITY_LEVEL_1_FLASH     = 0xD7,
    COMMAND_SENSITIVITY_LEVEL_2_FLASH     = 0xD8,
    COMMAND_SENSITIVITY_LEVEL_3_FLASH     = 0xD9,
    COMMAND_SENSITIVITY_LEVEL_4_FLASH     = 0xDA,
    COMMAND_SENSITIVITY_LEVEL_5_FLASH     = 0xDB,

    // OUTPUT
    COMMAND_ALARM_OUTPUT_OFF              = 0xE0,
    COMMAND_ALARM_OUTPUT_ON               = 0xE1,

    // SPECIAL
    COMMAND_ICHECK                        = 0xFE

} DLI_Command_t;



#endif /* SENSOR_MESSAGE_H */
