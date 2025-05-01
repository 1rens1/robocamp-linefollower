#define MOTOR_PWM_LEFT 1
#define MOTOR_PWM_RIGHT 2
#define MOTOR_DIR_LEFT 3
#define MOTOR_DIR_RIGHT 4

#define MOTOR_MAX_SPEED 255  // 0-255

#define Kp 1
#define Ki 0
#define Kd 0

const uint8_t IR_SENSORS[] = { 5, 6, 7, 8, 9 };  // from left to right

enum Side {
  LEFT,
  RIGHT,
  BOTH
};

enum Direction {
  FORWARD,
  BACKWARD
}
