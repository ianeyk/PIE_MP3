#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotorLeft = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorRight = AFMS.getMotor(1);

const int numSensors = 2; // set number of sensors to 2
const int sensorPins[numSensors] = {A2, A3}; // set sensor pins; we used A2 and A3
int sensorValues[numSensors]; // initialize array with length equal to the number of sensors in order to store sensor values
const int numMotors = 2; // set number of motors to 2
int motorValues[numSensors]; // initialize array with length equal to the number of motors in order to store motor values

const int THRESH = 382; // sensor value threshold; below 382 we are on tile, above on tape 

String readString; // set up string

int SPEEDc = 15; // initial speed
int DELAYc = 1; // initial delay
float ratio; // set up ratio
float RATIOc = 4; // initial ratio

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  if (!AFMS.begin()) // check if motor shield is working
  {
    while (1);
  } 

  // turn on motor
  myMotorLeft->run(RELEASE);
  myMotorRight->run(RELEASE);
}

void readSensors() // read the sensor values and fill them into the array sensorValues
{
  for (int sensor = 0; sensor < numSensors; sensor++)
  {
    sensorValues[sensor] = analogRead(sensorPins[sensor]);
  }
} 

void driveForward(int speed) // drive the robot forward
{
  float leftSpeed = speed; // set the left motor speed to initial speed
  float rightSpeed = speed; // set the right motor speed to initial speed
  myMotorLeft->setSpeed(leftSpeed); // set the left motor speed
  myMotorRight->setSpeed(rightSpeed); // set the right motor speed
  myMotorRight->run(BACKWARD); // drive right motor forward (wiring is flipped, backward command drives forward)
  myMotorLeft->run(BACKWARD); // drive left motor forward (wiring is flipped, backward command drives forward)

  motorValues[0] = speed; // set first value in motorValues array to the speed
  motorValues[1] = speed; // set second value in motorValues array to the speed
}

void turnLeft(int speed) // turn robot left
{
  float leftSpeed = speed; // set the left motor speed to speed
  float rightSpeed = RATIOc * speed; // set the right motor speed to the ratio times speed (right motor will go faster than left)
  myMotorLeft->setSpeed(leftSpeed); // set the left motor speed
  myMotorRight->setSpeed(rightSpeed); // set the right motor speed
  myMotorRight->run(BACKWARD); // drive right motor forward (wiring is flipped, backward command drives forward)
  myMotorLeft->run(BACKWARD); // drive left motor forward (wiring is flipped, backward command drives forward)

  motorValues[0] = leftSpeed; // set first value in motorValues array to the left wheel's speed
  motorValues[1] = rightSpeed; // set second value in motorValues array to the right wheel's speed
}

void turnRight(int speed)// turn the robot right
{
  float leftSpeed = RATIOc * speed; // set the left motor speed to the ratio times speed (left motor will go faster than left)
  float rightSpeed = speed; // set the right motor speed to speed
  myMotorLeft->setSpeed(leftSpeed); // set the left motor speed
  myMotorRight->setSpeed(rightSpeed); // set the right motor speed
  myMotorRight->run(BACKWARD); // drive right motor forward (wiring is flipped, backward command drives forward)
  myMotorLeft->run(BACKWARD); // drive left motor forward (wiring is flipped, backward command drives forward)

  motorValues[0] = leftSpeed; // set first value in motorValues array to the left wheel's speed
  motorValues[1] = rightSpeed; // set second value in motorValues array to the right wheel's speed
}

bool leftSensorOnTape() { // test if left sensor is on the tape or not
  return 1 - (sensorValues[0] < THRESH); // if the left sensor value is smaller than the threshold, then it is on the tape
}

bool rightSensorOnTape() { // test if right sensor is on the tape or not
  return 1 - (sensorValues[1] < THRESH); // if the right sensor value is smaller than the threshold, then it is on the tape
}

void printColumns() { // set up headers for printed data
  Serial.print("Time"); Serial.print(", ");
  Serial.print("Left Sensor"); Serial.print(", ");
  Serial.print("Right Sensor"); Serial.print(", ");
  Serial.print("Left Motor"); Serial.print(", ");
  Serial.print("Right Motor"); Serial.print(", ");
}

void printData() { // function to print information about timer, sensor values, and motor values
  Serial.print(millis()); Serial.print(", ");
  Serial.print(sensorValues[0]); Serial.print(", ");
  Serial.print(sensorValues[1]); Serial.print(", ");
  Serial.print(motorValues[0]); Serial.print(", ");
  Serial.print(motorValues[1]); Serial.println("");
}

void loop()
{
  while(Serial.available()){ // check for input from serial line
    delay(10); // debounce
    if(Serial.available() > 0) { // if there is information available
      char c = Serial.read(); // read the information
      readString += c;
    }
  }
  if(readString.length() > 0) {
    Serial.println(readString); // print the information from the serial line
    SPEEDc = (readString.substring(0,3)).toInt(); // set first three numbers to the new speed
    DELAYc = (readString.substring(3,7)).toInt(); // set the next four numbers to the new delay 
    ratio = (readString.substring(7,10)).toFloat(); // the last three numbers are the new ratio times 100
    RATIOc = ratio / 100; // set the last three numbers / 100 to the new ratio
    readString = ""; // end of string
  }

  readSensors(); // find the sensor data

  if (leftSensorOnTape() || rightSensorOnTape()) { // if the left or right sensors are on the tape
    turnLeft(SPEEDc); // turn left
  }

  else {
    turnRight(SPEEDc); // turn right
  }

  printData(); // print the sensor information to the serial port
  delay(DELAYc); // debounce
}
