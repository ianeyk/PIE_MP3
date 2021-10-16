int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13;    // select the pin for the LED

const int numSensors = 2;
const int sensorPins[numSensors] = {A2, A3};
String sensorNames[numSensors] = {"Left", "Right"}; // variable to store the value coming from the sensor
int sensorValues[numSensors]; // variable to store the value coming from the sensor

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    for (int sensor = 0; sensor < numSensors; sensor++)
    {
        Serial.print("Sensor ");
        Serial.print(sensorNames[sensor]);
        Serial.print(": ");
        int sensorValue = analogRead(sensorPins[sensor]);
        for (int i = 0; i < 1024; i += 100)
        {
            if (i < sensorValue)
            {
                Serial.print("x");
            }
            else
            {
                Serial.print(" ");
            }
        }
        Serial.print("  ");
    }
    Serial.println("");
}