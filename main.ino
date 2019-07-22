#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial BT(3, 2); // RX, TX

#define LED_PIN 13 
#define PWM_PIN1 11 //initialize digital pin 11（PWM output)
#define PWM_PIN2 10

#define PWM_MIN 0
#define PWM_MAX 255
#define EEPROM_ADDR 0
#define DELAY 10

int pwm;
String readString;

// Parameters to be saved in EEPROM
struct Params {
  int pwm1; // last brightness for pwm1
  int pwm2; // last brightness for pwm2
  int switchTime; //default time in ms for switching from one brightness to other

  public:
    Params() {
      pwm1 = 20;
      pwm2 = 20;
      switchTime = 1000;
    }
} params;

void setup() {
  
  pinMode(PWM_PIN1, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Bluetooth On");
  EEPROM.get(EEPROM_ADDR, params);
  String output = "";
  Serial.println(output + "Saved params are: {" + params.pwm1 + ", " + params.pwm2 + ", " + params.switchTime + "}");
  Serial.println("Please send 00..99 (2 characters) to set the brightness of the LED strip");
  pinMode(LED_PIN,OUTPUT);
  BT.begin(9600);
  readString = "";
  changeBrightness(PWM_PIN1, 0, params.pwm1, params.switchTime);
  changeBrightness(PWM_PIN2, 0, params.pwm2, params.switchTime);
}

void loop() {
  getStringFromBT(readString);

  // if a command was sent
  if (readString.length() > 0) {
    bool nC = false; //whether normal command received
    
    Serial.println("Something received");
    Serial.print(readString);
    if (readString.length() > 3) {
      String cmd = readString.substring(0,2);
      
      if (cmd == "B1") {
        // Change the brightness of the first LED.
        int pwm = constrain(readString.substring(3, readString.length()).toInt(), PWM_MIN, PWM_MAX);
        String output = "";
        Serial.println(output + "Changing the brightness of the LED1: current " + params.pwm1 + ", new " + pwm);
        changeBrightness(PWM_PIN1, params.pwm1, pwm, params.switchTime);
        params.pwm1 = pwm;
        nC = true;
      }
     
      if (cmd == "B2") {
        // Change the brightness of the second LED.
        int pwm = constrain(readString.substring(3, readString.length()).toInt(), PWM_MIN, PWM_MAX);
        String output = "";
        Serial.println(output + "Changing the brightness of the LED2: current " + params.pwm2 + ", new " + pwm);
        changeBrightness(PWM_PIN2, params.pwm2, pwm, params.switchTime);
        params.pwm2 = pwm;
        nC = true;
      }

      if (cmd == "ST") {
        // Set the switching time (constrain from DELAY to 10 sec)
        int st = constrain(readString.substring(3, readString.length()).toInt(), DELAY, 10000);
        String output = "";
        Serial.println(output + "Changing the switching time: current " + params.switchTime + ", new " + st);
        params.switchTime = st;
        nC = true;
      }
    }
    
    if (nC) {
      EEPROM.put(EEPROM_ADDR, params);
    }
    else {
      Serial.println("Wrong imput command");
    }
  }
 
  delay(100);
}

// Put in s the value of a string from bluetooth module
void getStringFromBT(String& s){
  s = "";
  while (BT.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (BT.available()) {
      char c = BT.read();  //gets one byte from serial buffer
      s += c; //makes the string readString
    } 
  }
}

// Change brightness of the LED strip, uses params.switchTime
void changeBrightness(const int pin, const int curr, const int next, int switchTime) {
  int st = switchTime / DELAY;
  for (int i = 0; i < st; i++) {
    analogWrite(pin, constrain( curr + int(1.0f * (next - curr)*(i + 1) / st), PWM_MIN, PWM_MAX));
    delay(DELAY);
  }
}
