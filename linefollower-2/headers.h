#ifndef HEADERS_H
#define HEADERS_H

#include <Arduino.h>

#define MOTOR_PWM_LEFT D2
#define MOTOR_PWM_RIGHT D1
#define MOTOR_DIR_LEFT D3
#define MOTOR_DIR_RIGHT D4

#define MOTOR_BASE_SPEED 100  // 0-255
#define MOTOR_MAX_SPEED 255  // 0-255

#define Kp 1.0f
#define Ki 0.0f
#define Kd 0.0f

const float MAX_INTEGRAL_CONTRIBUTION = 50.0f;

const uint8_t IR_SENSORS[] = { D5, D6, D0, D7, D8 };  // from left to right
const uint8_t IR_SENSORS_COUNT = (sizeof(IR_SENSORS)/sizeof(*IR_SENSORS));

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
  speed = constrain(speed, 0, MOTOR_MAX_SPEED);
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