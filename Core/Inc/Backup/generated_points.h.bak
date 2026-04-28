#ifndef GENERATED_POINTS_H
#define GENERATED_POINTS_H

#include <stdint.h>
#include "sensor_message.h"



typedef enum {
    GENERATED_POINT_NONE= 0,
    GENERATED_POINT_1,
    GENERATED_POINT_2,
    GENERATED_POINT_3,
    GENERATED_POINTS_MAX
} GeneratedPointIndex;

typedef enum {
    STATE_FAULT = 0x05,
    STATE_STANDBY = 0x22,
    STATE_CONTAMINATION = 0x2C,
    STATE_PRE_ALARM_ALARM = 0x7F
} StateOfDetector_t;


extern Message_t generated_points[];
extern const uint16_t generated_points_count;
extern const uint16_t generated_points_without_markers_count;
extern GeneratedPointIndex selected_point_index;

#endif
