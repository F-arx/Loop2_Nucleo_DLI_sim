/*
 * functions.c
 *
 *  Created on: Feb 26, 2026
 *      Author: Fabio
 *
 *  Frame and sensor data management functions
 */

#include "functions.h"
#include "sensor_message.h"
#include "generated_points.h"
#include "main.h"
#include <stdio.h>
#include "usart.h"
#include <stdarg.h>
#include <string.h>


Message_t *get_generated_point_to_send(uint8_t index)
{
    if (index <= generated_points_count)
    {
        return &generated_points[index];
    }
    return &generated_points[0];
}

void update_selected_point_leds(void)
{
    if ((selected_point_index <= GENERATED_POINT_NONE) ||
        (selected_point_index >= GENERATED_POINTS_MAX))
    {
        HAL_GPIO_WritePin(LED_FAULT_STATE_GPIO_Port, LED_FAULT_STATE_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_STANDBY_STATE_GPIO_Port, LED_STANDBY_STATE_Pin, GPIO_PIN_RESET);
        return;
    }

    if (generated_points[selected_point_index].status == STATE_STANDBY)
    {
        HAL_GPIO_WritePin(LED_FAULT_STATE_GPIO_Port, LED_FAULT_STATE_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_STANDBY_STATE_GPIO_Port, LED_STANDBY_STATE_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(LED_FAULT_STATE_GPIO_Port, LED_FAULT_STATE_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_STANDBY_STATE_GPIO_Port, LED_STANDBY_STATE_Pin, GPIO_PIN_RESET);
    }
}



/* =================== NEW DEBUG FUNCTIONS =================== */


void uart_printf(const char *fmt, ...)
{
  char buf[256];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  size_t len = strlen(buf);
  if (len) {
    // Transmit in blocking mode with increased timeout for FreeRTOS compatibility
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, (uint16_t)len, 1000);
  }
}

/**
 * @brief Debug: Print current selected point index and status (used on next/prev button press)
 *
 *
 *
 *
 */

void debug_print_current_point_next_prev(void)
{
#ifdef USE_UART_PRINTF
    if ((selected_point_index > GENERATED_POINT_NONE) &&
        (selected_point_index < GENERATED_POINTS_MAX))
    {
        uart_printf(
            "[NEXT/PREV] Current point: %d | Status: 0x%02X\r\n",
            selected_point_index,
            generated_points[selected_point_index].status
        );
    }
    else
    {
        uart_printf("[NEXT/PREV] No point selected (index: %d)\r\n", selected_point_index);
    }
#endif
}

/**
 * @brief Debug: Print last sent message
 */
void debug_print_last_sent_message(void)
{
#ifdef USE_UART_PRINTF
    uart_printf(
        "[LAST TX] Message sent: [%02X %02X %02X]\r\n",
        tx_0.address,
        tx_0.type,
        tx_0.status
    );
#endif
}

/**
 * @brief Debug: Print currently selected special message
 */
void debug_print_current_special_selected(void)
{
#ifdef USE_UART_PRINTF
    if (selected_special_message_index > SPECIAL_MESSAGE_NONE &&
        selected_special_message_index < SPECIAL_MESSAGE_MAX)
    {
        int msg_index = selected_special_message_index - 1;
        uart_printf(
            "[SPECIAL SEL] Selected special message: %d | [%02X %02X %02X]\r\n",
            selected_special_message_index,
            special_messages[msg_index].address,
            special_messages[msg_index].type,
            special_messages[msg_index].status
        );
    }
    else
    {
        uart_printf("[SPECIAL SEL] No special message selected (index: %d)\r\n", selected_special_message_index);
    }
#endif
}

/**
 * @brief Debug: Print last sent special message
 */
void debug_print_last_special_sent(void)
{
#ifdef USE_UART_PRINTF
    uart_printf(
        "[SPECIAL TX] Last special message sent: [%02X %02X %02X]\r\n",
        tx_0.address,
        tx_0.type,
        tx_0.status
    );
#endif
}
