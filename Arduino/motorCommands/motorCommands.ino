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

String readString, SPEEDc, DELAYc, ratio, RATIOc;

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
  myMotorLeft->setSpeed(speed);
  myMotorRight->setSpeed(RATIOc.toInt() * speed);
  myMotorRight->run(BACKWARD);
  myMotorLeft->run(BACKWARD);
}

void turnRight(int speed)
{
  Serial.println("Turning Right");
  myMotorLeft->setSpeed(RATIOc.toInt() * speed);
  myMotorRight->setSpeed(speed);
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
  while(Serial.available()){
    delay(10);
    if(Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
    }
  }
  if(readString.length() > 0) {
    Serial.println(readString);
    SPEEDc = readString.substring(0,3);
    DELAYc = readString.substring(3,7);
    ratio = readString.substring(7,10);
    RATIOc = ratio.toFloat()/100;
    Serial.println("Speed =" + SPEEDc);
    Serial.println("Delay = " + DELAYc);
    Serial.println("Ratio = " + RATIOc);
    readString = "";
  }


  // driveForward(SPEEDc);
  readSensors();

  if (leftSensorOnTape() && 1 - rightSensorOnTape()) {
    driveForward(SPEEDc.toInt());
   printForward();
   Serial.println(SPEEDc);

  }

  else if (leftSensorOnTape() && rightSensorOnTape()) {
    turnRight(SPEEDc.toInt());
   printRight();
  Serial.println(SPEEDc);
  }

  else if (1 - leftSensorOnTape() && 1 - rightSensorOnTape()) {
    turnLeft(SPEEDc.toInt());
    printLeft();
    Serial.println(SPEEDc);
  }

  else if (1 - leftSensorOnTape() && rightSensorOnTape()){
    turnRight(SPEEDc.toInt());
    printRight();
    Serial.println(SPEEDc);
  }

  else {
    Serial.print("error!");
  }

  stop();
  delay(DELAYc.toInt());

  // delay(1000;

}
