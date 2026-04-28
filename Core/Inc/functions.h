/*
 * functions.h
 *
 *  Created on: Feb 26, 2026
 *      Author: Fabio
 *
 *  Frame and sensor data management functions
 */

#ifndef INC_FUNCTIONS_H_
#define INC_FUNCTIONS_H_

#include "main.h"
#include "sensor_message.h"

/* =================== EXTERNAL VARIABLES =================== */

extern uint8_t last_tx_step;

/* =================== FUNCTION PROTOTYPES =================== */

void get_sensor_data_to_send(void);
void create_default_frames(void);
void reset_frames(void);


//void get_special_message_to_send(SpecialMessageIndex index);
Message_t *get_special_message_to_send(SpecialMessageIndex index);
Message_t *get_generated_point_to_send(uint8_t index);

//const Message_t *get_special_message(SpecialMessageIndex index);
const void get_special_message(SpecialMessageIndex index);
void update_selected_point_leds(void);
void uart_printf(const char *fmt, ...);


//function for debugging: print message content in hex format



#endif /* INC_FUNCTIONS_H_ */

