#include "headers.h"

int P = 0;
int I = 0;
int D = 0;

void setup() {
  Serial.begin(9600);

  setupPins();

  setMotor(BOTH, MOTOR_MAX_SPEED);

@@ -19,33 +21,27 @@
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
int getError() {
  // TODO: get error of the robot's distance to the center of the line using the 5 ir sensor array
  return 0;
  int error = 0;
  bool lineRead = false;
  
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
