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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
  while (!Serial); // Optional: wait for Serial
>>>>>>> Stashed changes
=======
  while (!Serial); // Optional: wait for Serial
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
  if (digitalRead(IR_SENSORS[0]) == ON_LINE) return -100.0f;
  if (digitalRead(IR_SENSORS[IR_SENSORS_COUNT - 1]) == ON_LINE) return 100.0f;  // ✅ fixed parenthesis

>>>>>>> Stashed changes
=======
  if (digitalRead(IR_SENSORS[0]) == ON_LINE) return -100.0f;
  if (digitalRead(IR_SENSORS[IR_SENSORS_COUNT - 1]) == ON_LINE) return 100.0f;  // ✅ fixed parenthesis

>>>>>>> Stashed changes
  uint8_t active = 0;
  float error = 0.0f;

  for (uint8_t i = 0; i < IR_SENSORS_COUNT; i++) {
    if (digitalRead(IR_SENSORS[i]) == ON_LINE) {
      float weight = IR_SENSORS_WEIGHTS[i];

<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
=======
>>>>>>> Stashed changes
      if (i == 0 || i == IR_SENSORS_COUNT - 1) {
        weight *= 1.8f;  // amplify edge sensors
      }

<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
      error += weight;
      active++;
    }
  }

  if (active == 0) {
    return lastError;
  }

  error /= active;  // normalize
  lastError = error;
  return error;
}

float p = 0.0f;
float i = 0.0f;
float d = 0.0f;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
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

=======
void loop() {
  float error = getError();
  unsigned long now = millis();

  // === PID Calculation ===
>>>>>>> Stashed changes
=======
void loop() {
  float error = getError();
  unsigned long now = millis();

  // === PID Calculation ===
>>>>>>> Stashed changes
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
