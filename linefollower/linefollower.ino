#include "headers.h"

long previousError = 0;
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
  while (!Serial)
    ;
  setupPins();

  setupAnimation();

  Serial.println("Setup Complete. Loop starting...");
}

int lastError = 0;

/*
* getError() function:
* negative error: drifting right (line is to the robot's right, robot needs to turn right)
* 0: center
* positive error: drifting left (line is to the robot's left, robot needs to turn left)
*/
int getError() {
  uint8_t active = 0;
  int error = 0;

  for (uint8_t i = 0; i < IR_SENSORS_COUNT; i++) {
    if (digitalRead(IR_SENSORS[i]) == ON_LINE) {
      error += IR_SENSORS_WEIGHTS[i];
      active++;
    }
  }

  if (active == 0) {
    return lastError;
  }

  lastError = error;

  return error;
}

// void bangBang() {
//   uint8_t sensorValues[IR_SENSORS_COUNT];
//   for (int i = 0; i < IR_SENSORS_COUNT; i++) {
//     sensorValues[i] = digitalRead(IR_SENSORS[i]);
//   }

//   if (sensorValues[1] == ON_LINE && sensorValues[3] == OFF_LINE) {  // drifting right
//     Serial.println("drift right, go left");
//     setMotor(LEFT, 0, FORWARD);
//     setMotor(RIGHT, MOTOR_BASE_SPEED, FORWARD);
//   } else if (sensorValues[1] == OFF_LINE && sensorValues[3] == ON_LINE) {  // drifting left
//     Serial.println("drift left, go right");
//     setMotor(LEFT, MOTOR_BASE_SPEED, FORWARD);
//     setMotor(RIGHT, 0, FORWARD);
//   } else {
//     setMotor(BOTH, MOTOR_BASE_SPEED , FORWARD);
//   }
// }

int p = 0;
int i = 0;
int d = 0;

// PID Algorithm
void loop() {
  int error = getError();
  unsigned long now = millis();

  // bangBang();

  p = Kp * error;
  i = constrain(Ki * (i + error), float(MOTOR_MAX_SPEED * -1), float(MOTOR_MAX_SPEED));
  d = Kd * error - lastError;

  int correction = ( p) + ( i) + ( d);

  int leftSpeed = MOTOR_BASE_SPEED + correction;
  int rightSpeed = MOTOR_BASE_SPEED - correction;

  if (leftSpeed >= 0) setMotor(LEFT, leftSpeed, FORWARD);
  else setMotor(LEFT, abs(leftSpeed), BACKWARD);

  if (rightSpeed >= 0) setMotor(RIGHT, rightSpeed, FORWARD);
  else setMotor(RIGHT, abs(rightSpeed), BACKWARD);

  Serial.print(leftSpeed);
  Serial.print(" ");
  Serial.println(rightSpeed);

  last_time = now;

  // delay(10);
}