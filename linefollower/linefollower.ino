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
  Serial.begin(9600);
  while (!Serial)
    ;
  setupPins();

  setMotor(BOTH, MOTOR_BASE_SPEED, Direction::FORWARD);
  Serial.println("Setup Complete. Loop starting...");
}

/*
* getError() function:
* negative error: drifting right (line is to the robot's right, robot needs to turn right)
* 0: center
* positive error: drifting left (line is to the robot's left, robot needs to turn left)
*/
int getError() {
  float positionSum = 0.0f;
  int activeSensorCount = 0;
  bool lineDetected = false;

  const float CENTER_POSITION = (NUM_SENSORS - 1.0f) / 2.0f;
  const int LINE_READING = LOW;

  for (int i = 0; i < NUM_SENSORS; i++) {
    int pinToRead = IR_SENSORS[i];
    int sensorValue = digitalRead(pinToRead);

    if (sensorValue == LINE_READING) {
      positionSum += i;
      activeSensorCount++;
      lineDetected = true;
    }
  }

  if (!lineDetected) {
    return 0;
  } else {
    float averagePosition = positionSum / activeSensorCount;
    int error = static_cast<int>((CENTER_POSITION - averagePosition) * 50.0f);
    return error;
  }
}

void setMotor(Side side, int speed, std::optional<Direction> directionOpt) {
  speed = constrain(speed, 0, MOTOR_MAX_SPEED);

  auto setSingleMotor = [&](int dirPin, int pwmPin) {
    if (directionOpt) {
      digitalWrite(dirPin, (*directionOpt == Direction::FORWARD ? LOW : HIGH));
    }
    analogWrite(pwmPin, speed);
  };

  if (side == Side::LEFT || side == Side::BOTH) {
    setSingleMotor(MOTOR_DIR_LEFT, MOTOR_PWM_LEFT);
  }
  if (side == Side::RIGHT || side == Side::BOTH) {
    setSingleMotor(MOTOR_DIR_RIGHT, MOTOR_PWM_RIGHT);
  }
}

void loop() {
  int error = getError();

  float P_term = Kp * error;

  integralAccumulator += error;
  float I_term = Ki * integralAccumulator;
  I_term = constrain(I_term, -MAX_INTEGRAL_CONTRIBUTION, MAX_INTEGRAL_CONTRIBUTION);
  if (Ki != 0.0f) {
    integralAccumulator = I_term / Ki;
  } else {
    integralAccumulator = 0.0f;
  }

  float D_term = Kd * (error - previousError);
  previousError = error;

  float totalCorrection = P_term + I_term + D_term;

  int leftMotorSpeed = MOTOR_BASE_SPEED - totalCorrection;
  int rightMotorSpeed = MOTOR_BASE_SPEED + totalCorrection;

  leftMotorSpeed = constrain(leftMotorSpeed, 0, MOTOR_MAX_SPEED);
  rightMotorSpeed = constrain(rightMotorSpeed, 0, MOTOR_MAX_SPEED);

  setMotor(Side::LEFT, leftMotorSpeed, Direction::FORWARD);
  setMotor(Side::RIGHT, rightMotorSpeed, Direction::FORWARD);

  Serial.print("Err: ");
  Serial.print(error);
  Serial.print(" P: ");
  Serial.print(P_term);
  Serial.print(" I: ");
  Serial.print(I_term);
  Serial.print(" D: ");
  Serial.print(D_term);
  Serial.print(" Corr: ");
  Serial.print(totalCorrection);
  Serial.print(" LSpd: ");
  Serial.print(leftMotorSpeed);
  Serial.print(" RSpd: ");
  Serial.println(rightMotorSpeed);

  delay(10);
}