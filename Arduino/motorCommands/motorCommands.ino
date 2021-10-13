#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *myMotorRight = AFMS.getMotor(2);

const int numSensors = 2;
const int sensorPins[numSensors] = {A1, A2};
int sensorValues[numSensors];

const int THRESH = 400;
const int SPEED = 127;

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin())
  { // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1)
      ;
  }
  Serial.println("Motor Shield found.");

  // turn on motor
  myMotorLeft->run(RELEASE);
  myMotorRight->run(RELEASE);
}

void readSensors()
{
  for (int sensor = 0; sensor < numSensors; sensor++)
  {
    sensorValues[sensor] = analogRead(sensorPins[sensor]);
  }

  Serial.print("Senosor 1");
  Serial.print(sensorValues[0]);
  Serial.print("   Senosor 2");
  Serial.println(sensorValues[1]);
}

void driveForward(int speed)
{
  myMotorLeft->run(FORWARD);
  myMotorRight->run(FORWARD);
  myMotorLeft->setSpeed(speed);
  myMotorRight->setSpeed(speed);
}

void turnLeft(int ang, int radius, int speed)
{
  myMotorLeft->run(FORWARD);
  myMotorRight->run(FORWARD);
  myMotorLeft->setSpeed(speed);
  myMotorRight->setSpeed(3 * speed);
}

void turnRight(int ang, int radius, int speed)
{
  myMotorLeft->run(FORWARD);
  myMotorRight->run(FORWARD);
  myMotorLeft->setSpeed(3 * speed);
  myMotorRight->setSpeed(speed);
}

void stop()
{
  myMotorLeft->run(RELEASE);
  myMotorRight->run(RELEASE);
}

void loop()
{
  readSensors();

  if (sensorValues[0] < THRESH && sensorValues[1] < THRESH) {
    driveForward(SPEED);
  }
  if (sensorValues[0] > THRESH && sensorValues[1] < THRESH) {
    turnRight(0, 0, SPEED);
  }
  if (sensorValues[0] < THRESH && sensorValues[1] > THRESH) {
    turnLeft(0, 0, SPEED);
  }
}