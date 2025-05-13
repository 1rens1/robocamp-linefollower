#include "headers.h"

float previousError = 0.0f;
float integralAccumulator = 0.0f;
unsigned long last_time = 0;

void setupPins() {
  pinMode(MOTOR_DIR_LEFT, OUTPUT);
  pinMode(MOTOR_PWM_LEFT, OUTPUT);
  pinMode(MOTOR_DIR_RIGHT, OUTPUT);
  pinMode(MOTOR_PWM_RIGHT, OUTPUT);

  for (uint8_t pin : IR_SENSORS) {
    pinMode(pin, INPUT);
  }
}

void setupAnimation() {
  setMotor(BOTH, 100, FORWARD);
  delay(200);
  setMotor(BOTH, 0, FORWARD);
  delay(300);
  setMotor(BOTH, 100, BACKWARD);
  delay(200);
  setMotor(BOTH, 0, FORWARD);
  delay(1500);
}

void setup() {
  delay(800);
  Serial.begin(9600);

  setupPins();
  setupAnimation();

  Serial.println("Setup Complete. Loop starting...");
}

float lastError = 0.0f;

/*
 * getError() function:
 * negative error: drifting right (line is to the robot's right)
 * 0: centered
 * positive error: drifting left (line is to the robot's left)
 */
float getError() {
  uint8_t active = 0;
  float error = 0.0f;

  for (uint8_t i = 0; i < IR_SENSORS_COUNT; i++) {
    if (digitalRead(IR_SENSORS[i]) == ON_LINE) {
      float weight = IR_SENSORS_WEIGHTS[i];

      error += weight;
      active++;
    }
  }

  if (active == 0) {
    return lastError;
  }

  lastError = error;
  return error;
}

float p = 0.0f;
float i = 0.0f;
float d = 0.0f;

//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
//  THIS IS LINEFOLLOWER.INO NOT LINEFOLLOWER2.INO
void loop() {
  float error = getError();
  unsigned long now = millis();
  Serial.println(error);

  p = Kp * error;
  i = constrain(integralAccumulator + (Ki * error), -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
  d = Kd * (error - previousError);

  integralAccumulator = i;
  previousError = error;

  float correction = p + i + d;

  int leftSpeed = MOTOR_BASE_SPEED + correction;
  int rightSpeed = MOTOR_BASE_SPEED - correction;

  setMotor(LEFT, abs(leftSpeed), leftSpeed >= 0 ? FORWARD : BACKWARD);
  setMotor(RIGHT, abs(rightSpeed), rightSpeed >= 0 ? FORWARD : BACKWARD);

  Serial.print("Correction: ");
  Serial.print(correction);
  Serial.print(" | Left: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right: ");
  Serial.println(rightSpeed);

  last_time = now;

  // delay(10);
}