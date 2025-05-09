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


/*
* getError() function:
* negative error: drifting right (line is to the robot's right, robot needs to turn right)
* 0: center
* positive error: drifting left (line is to the robot's left, robot needs to turn left)
*/
int getError() {
  return 0;
}

void bangBang() {
  uint8_t sensorValues[IR_SENSORS_COUNT];
  for (int i = 0; i < IR_SENSORS_COUNT; i++) {
    sensorValues[i] = digitalRead(IR_SENSORS[i]);
  }

  if (sensorValues[1] == ON_LINE && sensorValues[3] == OFF_LINE) {  // drifting right
    Serial.println("drift right, go left");
    setMotor(LEFT, 0, FORWARD);
    setMotor(RIGHT, MOTOR_BASE_SPEED, FORWARD);
  } else if (sensorValues[1] == OFF_LINE && sensorValues[3] == ON_LINE) {  // drifting left
    Serial.println("drift left, go right");
    setMotor(LEFT, MOTOR_BASE_SPEED, FORWARD);
    setMotor(RIGHT, 0, FORWARD);
  } else {
    setMotor(BOTH, MOTOR_BASE_SPEED, FORWARD);
  }
}


void loop() {
  // int error = getError();

  bangBang();




  // delay(10);
}