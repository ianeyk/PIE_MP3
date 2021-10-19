#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotorLeft = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorRight = AFMS.getMotor(1);

const int numSensors = 2;
const int sensorPins[numSensors] = {A2, A3};
int sensorValues[numSensors];
const int numMotors = 2;
int motorValues[numSensors];

const int THRESH = 382;
int SPEED = 15;

String readString;

int SPEEDc = 15;
int DELAYc = 100;
float ratio;
float RATIOc = 4;

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  // Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin())
  { // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    // Serial.println("Could not find Motor Shield. Check wiring.");
    while (1)
      ;
  }
  // Serial.println("Motor Shield found.");

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
}

void driveForward(int speed)
{
  float leftSpeed = speed;
  float rightSpeed = speed;
  // Serial.println("Driving Forward");
  myMotorLeft->setSpeed(leftSpeed);
  myMotorRight->setSpeed(rightSpeed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);

  motorValues[0] = speed;
  motorValues[1] = speed;
}

void turnLeft(int speed)
{
  float leftSpeed = speed;
  float rightSpeed = RATIOc * speed;
  // Serial.println("Turning Left");
  myMotorLeft->setSpeed(leftSpeed);
  myMotorRight->setSpeed(rightSpeed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);

  motorValues[0] = leftSpeed;
  motorValues[1] = rightSpeed;
}

void turnRight(int speed)
{
  float leftSpeed = RATIOc * speed;
  float rightSpeed = speed;
  // Serial.println("Turning Right");
  myMotorLeft->setSpeed(leftSpeed);
  myMotorRight->setSpeed(rightSpeed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);

  motorValues[0] = leftSpeed;
  motorValues[1] = rightSpeed;
}

void pivotLeft(int speed)
{
  float leftSpeed = speed;
  float rightSpeed = RATIOc * speed;

  // Serial.println("Turning Left");
  myMotorLeft->setSpeed(leftSpeed);
  myMotorRight->setSpeed(rightSpeed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(FORWARD);

  motorValues[0] = -1 * leftSpeed;
  motorValues[1] = rightSpeed;
}

void pivotRight(int speed)
{
  float leftSpeed = RATIOc * speed;
  float rightSpeed = speed;
  // Serial.println("Turning Right");
  myMotorLeft->setSpeed(leftSpeed);
  myMotorRight->setSpeed(rightSpeed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(FORWARD);

  motorValues[0] = leftSpeed;
  motorValues[1] = -1 * rightSpeed;
}

void stop()
{
  // Serial.println("Stopping");
  myMotorLeft->run(RELEASE);
  myMotorRight->run(RELEASE);

  motorValues[0] = 0;
  motorValues[1] = 0;
}

bool leftSensorOnTape() {
  return 1 - (sensorValues[0] < THRESH);
}

bool rightSensorOnTape() {
  return 1 - (sensorValues[1] < THRESH);
}

void printColumns() {
  Serial.print("Left Sensor"); Serial.print(", ");
  Serial.print("Right Sensor"); Serial.print(", ");
  Serial.print("Left Motor"); Serial.print(", ");
  Serial.print("Right Motor"); Serial.print(", ");
}

void printData() {
  Serial.print(sensorValues[0]); Serial.print(", ");
  Serial.print(sensorValues[1]); Serial.print(", ");
  Serial.print(motorValues[0]); Serial.print(", ");
  Serial.print(motorValues[1]); Serial.print(", ");
}

void loop()
{
  while(Serial.available()){
    delay(10);
    if(Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }
  if(readString.length() > 0) {
    Serial.println(readString);
    SPEEDc = (readString.substring(0,3)).toInt();
    DELAYc = (readString.substring(3,7)).toInt();
    ratio = (readString.substring(7,10)).toFloat();
    RATIOc = ratio / 100;
    readString = "";
  }

  readSensors();

  if (leftSensorOnTape() || rightSensorOnTape()) {
    // printLeft();
    turnLeft(SPEEDc);
  }

  else {
    // printRight();
    turnRight(SPEEDc);
  }

  printData();

  delay(DELAYc);
  stop();
  // delay(DELAYc);

  // delay(1000;
}
