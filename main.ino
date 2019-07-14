#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(3, 2); // RX, TX

#define LED_PIN 13 
#define PWM_PIN 11 //initialize digital pin 11（PWM output)

#define PWM_MIN 0
#define PWM_MAX 255
int pwm;
String readString;

void setup() {
  pinMode(PWM_PIN,OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Bluetooth On");
  Serial.println("Please send 00..99 (2 characters) to set the brightness of the LED strip");
  pinMode(LED_PIN,OUTPUT);
  BT.begin(9600);
  pwm = PWM_MIN;
  readString = "";
}

void loop() {
  
  while (BT.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (BT.available()) {
      char c = BT.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    } 
  }
  
  if (readString.length() > 0) {
    Serial.print(readString);
    
    //Logic of parsing the string into commands
    
    if (readString.length() > 2) {
      pwm = map(readString.substring(0,2).toInt(), 0, 99, PWM_MIN, PWM_MAX);
    }
    pwm = constrain(pwm, PWM_MIN, PWM_MAX);
    analogWrite(PWM_PIN, pwm);
  }
  delay(100);
  readString = "";
}
