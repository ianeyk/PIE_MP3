#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotorLeft = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorRight = AFMS.getMotor(1);

const int numSensors = 2;
const int sensorPins[numSensors] = {A2, A3};
int sensorValues[numSensors];

const int THRESH = 382;
const int SPEED = 35;

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

  Serial.print("Senosor 1: ");
  Serial.print(sensorValues[0]);
  Serial.print("   Senosor 2: ");
  Serial.println(sensorValues[1]);
}

void driveForward(int speed)
{
  Serial.println("Driving Forward");
  myMotorLeft->setSpeed(speed);
  myMotorRight->setSpeed(speed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);
}

void turnLeft(int speed)
{
  Serial.println("Turning Left");
  myMotorLeft->setSpeed(-speed);
  myMotorRight->setSpeed(1.5 * speed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);
}

void turnRight(int speed)
{
  Serial.println("Turning Right");
  myMotorLeft->setSpeed(1.5 * speed);
  myMotorRight->setSpeed(-speed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);
}

void stop()
{
  Serial.println("Stopping");
  myMotorLeft->run(RELEASE);
  myMotorRight->run(RELEASE);
}

bool leftSensorOnTape() {
  return sensorValues[0] < THRESH;
}

bool rightSensorOnTape() {
  return sensorValues[1] < THRESH;
}

void printForward() {
    Serial.println("                                  /\\");
    Serial.println("                                 /||\\");
    Serial.println("                                //||\\\\");
    Serial.println("                               ///||\\\\\\");
    Serial.println("                                  ||");
    Serial.println("                                  ||");
    Serial.println("                                  ||");
}

void printLeft() {
    Serial.println("                                 ///////");
    Serial.println("                                <<<<<<<------");
    Serial.println("                                 \\\\\\\\\\\\\\");
}

void printRight() {
    Serial.println("                                               \\\\\\\\\\\\\\");
    Serial.println("                                          ------>>>>>>>");
    Serial.println("                                               ///////");
}

void loop()
{
  // driveForward(SPEED);
  readSensors();

  if (leftSensorOnTape() && 1 - rightSensorOnTape()) {
    driveForward(SPEED);
    printForward();

  }

  else if (leftSensorOnTape() && rightSensorOnTape()) {
    turnRight(SPEED);
    printRight();

  }

  else if (1 - leftSensorOnTape() && 1 - rightSensorOnTape()) {
    turnLeft(SPEED);
    printLeft();
  }

  else if (1 - leftSensorOnTape() && rightSensorOnTape()){
    turnRight(SPEED);
    printRight();
  }

  else {
    Serial.print("error!");
  }

  stop();
  delay(500);

  // delay(1000;
}