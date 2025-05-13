#include "headers.h"

// float integralAccumulator = 0.0f;
unsigned long lastTime = 0;

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
  while (!Serial)
    ;
  setupPins();

  setupAnimation();

  Serial.println("Setup Complete. Loop starting...");
}

float lastError = 0;

/*
* getError() function:
* negative error: drifting right (line is to the robot's right, robot needs to turn right)
* 0: center
* positive error: drifting left (line is to the robot's left, robot needs to turn left)
*/
float getError() {
  unsigned int active = 0;
  float error = 0;
  
  for (uint8_t i = 0; i < IR_SENSORS_COUNT; i++) {
    if (digitalRead(IR_SENSORS[i]) == ON_LINE) {
      error += IR_SENSORS_WEIGHTS[i];
      active++;
    }
  }

  if (active == 0)
    return lastError;

  lastError = error;

  return error;
}

float p = 0;
float i = 0;
float d = 0;

// PID Algorithm
void loop() {
  float error = getError();
  unsigned long now = millis();

  // bangBang();

  p = Kp * error;
  i = constrain(Ki * (i + error), INTEGRAL_MAX_CONTRIBUTION * -1, INTEGRAL_MAX_CONTRIBUTION);
  d = Kd * error - lastError;

  float correction = p + i + d;

  int leftSpeed = round(MOTOR_BASE_SPEED + correction);
  int rightSpeed = round(MOTOR_BASE_SPEED - correction);

  setMotor(LEFT, abs(leftSpeed), leftSpeed >= 0 ? FORWARD : BACKWARD);
  setMotor(RIGHT, abs(rightSpeed), rightSpeed >= 0 ? FORWARD : BACKWARD);

  Serial.print(leftSpeed);
  Serial.print(" ");
  Serial.println(rightSpeed);

  lastTime = now;

  // delay(10);
}