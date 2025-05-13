#ifndef HEADERS_H
#define HEADERS_H

#include <Arduino.h>

#define MOTOR_PWM_LEFT D2
#define MOTOR_PWM_RIGHT D1
#define MOTOR_DIR_LEFT D3
#define MOTOR_DIR_RIGHT D4

#define MOTOR_BASE_SPEED 150  // 0-255
#define MOTOR_MAX_SPEED 255  // 0-255

#define INTEGRAL_MAX_CONTRIBUTION 255.0f

#define Kp 1.0f * MOTOR_BASE_SPEED
#define Ki 0.05f
#define Kd 10.0f

const uint8_t IR_SENSORS[] = { D5, D6, D0, D7, D8 };  // from left to right
const uint8_t IR_SENSORS_COUNT = 5;
const float IR_SENSORS_WEIGHTS[IR_SENSORS_COUNT] = { -2.5f, -1.0f, 0.0f, 1.0f, 2.5f };


#define ON_LINE LOW
#define OFF_LINE HIGH

enum Side {
  LEFT,
  RIGHT,
  BOTH
};

enum Direction {
  FORWARD,
  BACKWARD
};



void setMotor(Side side, int speed, Direction direction) {
  speed = constrain(side == LEFT ? speed * 0.95f : speed, 0, MOTOR_MAX_SPEED);
  int ctrl = direction == Direction::FORWARD ? HIGH : LOW;

  auto setSingleMotor = [&](int pwmPin, int dirPin) {
    digitalWrite(dirPin, ctrl);
    analogWrite(pwmPin, speed);
  };

  if (side == LEFT || side == BOTH) {
    setSingleMotor(MOTOR_PWM_LEFT, MOTOR_DIR_LEFT);
  }
  if (side == RIGHT || side == BOTH) {
    setSingleMotor(MOTOR_PWM_RIGHT, MOTOR_DIR_RIGHT);
  }
}

#endif