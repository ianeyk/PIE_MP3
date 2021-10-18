String command;
int led = 9;
 
void setup() {
    Serial.begin(9600);
    pinMode(led, OUTPUT);
}
 
void loop() {
    if(Serial.available()){
        command = Serial.readStringUntil('\n');
        if(command.equals("on")) {
          digitalWrite(led, HIGH);
          Serial.println("It's on!");
        }
        if(command.equals("off")) {
          digitalWrite(led, LOW);
          Serial.println("It's off!");
        }
        
    }
}
