#ifndef HEADERS_H
#define HEADERS_H

#include <Arduino.h>
#include <optional>

#define MOTOR_PWM_LEFT D1
#define MOTOR_PWM_RIGHT D2
#define MOTOR_DIR_LEFT D3
#define MOTOR_DIR_RIGHT D4

#define MOTOR_BASE_SPEED 255  // 0-255
#define MOTOR_MAX_SPEED 255  // 0-255

#define Kp 1.0f
#define Ki 0.0f
#define Kd 0.0f

const float MAX_INTEGRAL_CONTRIBUTION = 50.0f;

const uint8_t IR_SENSORS[] = { D5, D6, D0, D7, D8 };  // from left to right
const uint8_t NUM_SENSORS = (sizeof(IR_SENSORS)/sizeof(*IR_SENSORS));

enum Side {
  LEFT,
  RIGHT,
  BOTH
};

enum Direction {
  FORWARD,
  BACKWARD
};

#endif