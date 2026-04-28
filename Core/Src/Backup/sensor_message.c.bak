/*
 * sensor_message.c
 *
 *  Created on: Feb 26, 2026
 *      Author: Fabio
 *
 *  Sensor messaging system - Data structures, events, and I2C communication
 */

#include "sensor_message.h"
#include "generated_points.h"

/* ===================== I2C COMMUNICATION BUFFERS ==================== */

uint8_t rxCmd[2];
Message_t tx_0 = {0x00, 0x00, 0x00}; // address ; type : sensor type ; status : sensor status
Message_t buffer_tx_end = {0xFE, 0xFE, 0xFE};
Message_t *buffer_tx = &tx_0;
Message_t *msg_to_send=&tx_0;

/* ===================== I2C COMMUNICATION FLAGS ===================== */

volatile uint8_t buffer_ready = 0;
volatile uint8_t i2c_tx_flag = 0;
volatile uint8_t i2c_rx_flag = 0;
volatile uint8_t i2c_err_flag = 0;
volatile uint32_t i2c_last_error = 0;
volatile uint8_t tx_step = 0;
uint8_t last_tx_step = 0;

/* ======================== SENSOR DATA VALUES ======================== */

uint8_t type_of_sensor = 0x85;
uint8_t init_status_of_sensor = 0x10;
uint8_t id = 0x01;
uint8_t sensor_data[3] = {0};

DetectorProfile detector_status = {
  .type = OPTICAL_SMOKE,
  .alarm_from_io = NO_IO_ALARM,
  .limit_alarm = NO_LIMIT_ALARM,
  .discovery_multi = NOT_DISCOVERY_MULTISENSOR,
  .discovery_device = NOT_DISCOVERY_DEVICE
};

/* ========================== SPECIAL MESSAGES ========================== */

uint16_t counter_special_message = 0;
volatile uint8_t special_message_pending = 0; /* Flag: 1 if special message waiting to send, 0 otherwise */

SpecialMessageIndex special_message_index = SPECIAL_MESSAGE_NONE;
/* Table of special messages - DO NOT MODIFY ORDER */
//T

//uint8_t msg[3] = {0x00, 0x00, 0x00}; // exemple de message à envoyer

const Message_t special_messages[] =
{
    /* ===== Broken wire ===== */
    {0xFD, 0x00, 0x00}, /* broken wire standby, loop 1 */
    {0xFE, 0x00, 0x00}, /* broken wire standby, loop 2 */
    {0xFD, 0x01, 0x00}, /* broken wire, loop 1 */
    {0xFE, 0x01, 0x00}, /* broken wire, loop 2 */

    /* ===== Short-circuit standby ===== */
    {0xFD, 0x02, 0x00}, /* short-circuit standby, loop 1 */
    {0xFE, 0x02, 0x00}, /* short-circuit standby, loop 2 */

    /* ===== Short-circuit fault ===== */
    {0xFD, 0x03, 0x00}, /* short-circuit, loop 1 */
    {0xFE, 0x03, 0x00}, /* short-circuit, loop 2 */

    /* ===== Short-circuit standby (connection n = 1..4) ===== */
    {0xFD, 0x04, 0x01}, /* connection 1 */
    {0xFD, 0x04, 0x02}, /* connection 2 */
    {0xFD, 0x04, 0x03}, /* connection 3 */
    {0xFD, 0x04, 0x04}, /* connection 4 */

    /* ===== Short-circuit fault (connection n = 1..4) ===== */
    {0xFD, 0x05, 0x01}, /* connection 1 */
    {0xFD, 0x05, 0x02}, /* connection 2 */
    {0xFD, 0x05, 0x03}, /* connection 3 */
    {0xFD, 0x05, 0x04}, /* connection 4 */

    /* ===== Shielding standby (n = 1..8) ===== */
    {0xFD, 0x06, 0x01}, /* connection 1 */
    {0xFD, 0x06, 0x02}, /* connection 2 */
    {0xFD, 0x06, 0x03}, /* connection 3 */
    {0xFD, 0x06, 0x04}, /* connection 4 */
    {0xFD, 0x06, 0x05}, /* connection 5 */
    {0xFD, 0x06, 0x06}, /* connection 6 */
    {0xFD, 0x06, 0x07}, /* connection 7 */
    {0xFD, 0x06, 0x08}, /* connection 8 */

    /* ===== Shielding fault (n = 1..8) ===== */
    {0xFD, 0x07, 0x01}, /* connection 1 */
    {0xFD, 0x07, 0x02}, /* connection 2 */
    {0xFD, 0x07, 0x03}, /* connection 3 */
    {0xFD, 0x07, 0x04}, /* connection 4 */
    {0xFD, 0x07, 0x05}, /* connection 5 */
    {0xFD, 0x07, 0x06}, /* connection 6 */
    {0xFD, 0x07, 0x07}, /* connection 7 */
    {0xFD, 0x07, 0x08}, /* connection 8 */

    /* ===== Detector board EEPROM ===== */
    {0xFD, 0x08, 0x00}, /* standby */
    {0xFD, 0x08, 0x01}, /* fault */

    /* ===== Detector board voltage ===== */
    {0xFD, 0x09, 0x00}, /* standby */
    {0xFD, 0x09, 0x01}, /* fault */

    /* ===== Wireless / special ===== */
    {0xFD, 0xA5, 0xA5}  /* wireless triggering fault (from DBM013) */
};

const uint8_t special_messages_count = sizeof(special_messages) / sizeof(special_messages[0]);

/* ========================== EXECUTION STATE ========================== */
/* NO extern! Regular variable modified by main code */

/* Main execution step */
step_t step = step_init;

uint8_t current_tx_index = 0;
//uint16_t selected_point_index = 0;

SpecialMessageIndex selected_special_message_index = SPECIAL_MESSAGE_NONE;


//new feature variable

//GeneratedPointIndex selected_point_index = GENERATED_POINT_NONE;

















