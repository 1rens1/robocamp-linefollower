#include "headers.h"

long previousError = 0;
float integralAccumulator = 0.0f;

void setupPins() {
  pinMode(MOTOR_DIR_LEFT, OUTPUT);
  pinMode(MOTOR_PWM_LEFT, OUTPUT);
  pinMode(MOTOR_DIR_RIGHT, OUTPUT);
  pinMode(MOTOR_PWM_RIGHT, OUTPUT);


  for (uint8_t pin : IR_SENSORS) {
    pinMode(pin, INPUT);
  }
}

void setup() {
  delay(800);
  Serial.begin(9600);
  while (!Serial)
    ;
  setupPins();

  setupAnimation();

  Serial.println("Setup Complete. Loop starting...");

}

void setupAnimation() {
  setMotor(BOTH, MOTOR_BASE_SPEED, FORWARD);
  delay(200);
  setMotor(BOTH, 0, FORWARD);
  delay(200);
  setMotor(BOTH, MOTOR_BASE_SPEED, BACKWARD);
  delay(200);
  setMotor(BOTH, 0, FORWARD);
  delay(1500);
}

/*
* getError() function:
* negative error: drifting right (line is to the robot's right, robot needs to turn right)
* 0: center
* positive error: drifting left (line is to the robot's left, robot needs to turn left)
*/
int getError() {
  return 0;
}

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

void loop() {
  int error = getError();

  

  delay(10);
}