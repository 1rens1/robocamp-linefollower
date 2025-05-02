#include "headers.h"

int P = 0;
int I = 0;
int D = 0;

void setup() {
  Serial.begin(9600);

  setupPins();

  setMotor(BOTH, MOTOR_MAX_SPEED);

void setupPins() {
  for (int i = 1; i <= 4; i++)
    pinMode(i, OUTPUT);

    
  for (irSensorPin : IR_SENSORS)
    pinMode(irSensorPin, INPUT);
}

/*
* negative: drifting right
*
* 0: center
*
* positive: drifting left
*/

double getError() {
  int error = 0;
  bool lineRead = false;
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
  }

  float averagePosition = positionSum / activeSensorCount;
  int error = static_cast<int>((averagePosition - CENTER_POSITION) * 50.0f);

  return error;
}

void setMotor(Side side, uint8_t speed, std::optional<Direction> direction) {
  auto set = [&](int dirPin, int pwmPin) {
    if (direction)
      digitalWrite(dirPin, *direction == Direction::FORWARD ? LOW : HIGH);
    digitalWrite(pwmPin, speed);
  };
  if (side == Side::LEFT || side == Side::BOTH) set(MOTOR_DIR_LEFT, MOTOR_PWM_LEFT);
  if (side == Side::RIGHT || side == Side::BOTH) set(MOTOR_DIR_RIGHT, MOTOR_PWM_RIGHT);
}

void loop() {
}
