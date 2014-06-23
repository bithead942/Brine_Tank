/**************************************
BRINE TANK METER
by Bithead942

Loops continuously waiting for a button press or network connection.
If a button press is detected, check the level of the brine tank using the IR rangefinder.  It then converts the range to a percentage
   and lights the appropriate number of LEDs on the wall panel for 3 seconds.
If a network connection is detected (TCP 8888), check the level of the brine tank, convert the range to a percentage and write
   the value to the socket.
   
Uses:
-  Arduino Duemilanove with ATmega328
-  Arduino Ethernet Shield
-  Sharp GP2D12 IR distance sensor

****************************************/
#include <Ethernet.h>
#include <SPI.h>

const int RangeFinderPin = 0;    // select the input pin for the Range Finder (Analog)
const int TransistorPin = 0;     // select the output pin used to activate the Range Finder
const int ButtonPin = 15;         // select the input pin used to activate LED display
const int LED1Pin = 2;           // select the output pin used to light pin 1
const int LED2Pin = 3;           // select the output pin used to light pin 2
const int LED3Pin = 4;           // select the output pin used to light pin 3
const int LED4Pin = 5;           // select the output pin used to light pin 4
const int LED5Pin = 6;           // select the output pin used to light pin 5
const int LED6Pin = 7;           // select the output pin used to light pin 6
const int LED7Pin = 8;           // select the output pin used to light pin 7
const int LED8Pin = 9;           // select the output pin used to light pin 8
const int LED9Pin = 18;           // select the output pin used to light pin 9
const int LED10Pin = 19;           // select the output pin used to light pin 10

int RangeValue = 0;  // variable to store the value coming from the sensor
int ReadValue1 = 0;
int ReadValue2 = 0;
int ReadValue3 = 0;
int sensorMin = 90;
int sensorMax = 300;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 199 };

Server server(8888);

void setup() {
  pinMode(TransistorPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  pinMode(LED3Pin, OUTPUT);
  pinMode(LED4Pin, OUTPUT);
  pinMode(LED5Pin, OUTPUT);
  pinMode(LED6Pin, OUTPUT);
  pinMode(LED7Pin, OUTPUT);
  pinMode(LED8Pin, OUTPUT);
  pinMode(LED9Pin, OUTPUT);
  pinMode(LED10Pin, OUTPUT);
  
  digitalWrite (TransistorPin, LOW);
  digitalWrite (ButtonPin, LOW);
  digitalWrite (LED1Pin, LOW);
  digitalWrite (LED2Pin, LOW);
  digitalWrite (LED3Pin, LOW);
  digitalWrite (LED4Pin, LOW);
  digitalWrite (LED5Pin, LOW);
  digitalWrite (LED6Pin, LOW);
  digitalWrite (LED7Pin, LOW);
  digitalWrite (LED8Pin, LOW);
  digitalWrite (LED9Pin, LOW);
  digitalWrite (LED10Pin, LOW);
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  //Debug
  Serial.begin(9600);
  //
  
  CheckRange();
}

void loop() {
    if (digitalRead(ButtonPin) == HIGH) {
      CheckRange();
      ShowLEDs();
    }
  
    Client client = server.available();
    if (client.connected()) {
      CheckRange(); 
      ConvertToPercent();
      server.print(RangeValue, DEC);
      delay(1);
      client.stop();
    }
}

void CheckRange() {
      RangeValue = 0;
      // turn on the Range Finder
      digitalWrite(TransistorPin, HIGH);
      // allow the Range Finder to warm up
      delay(250);
      // read the value from the Range Finder sensor
      ReadValue1 = analogRead(RangeFinderPin);    
      ReadValue1 = constrain(ReadValue1, sensorMin, sensorMax);
      
      ReadValue2 = analogRead(RangeFinderPin);    
      ReadValue2 = constrain(ReadValue2, sensorMin, sensorMax);
      
      ReadValue3 = analogRead(RangeFinderPin);  
      ReadValue3 = constrain(ReadValue3, sensorMin, sensorMax);
      
      // turn off the Range Finder
      digitalWrite(TransistorPin, LOW);
      
      // average the 3 read results
      RangeValue = (ReadValue1 + ReadValue2 + ReadValue3)/3;
      
      //Debug
      Serial.println(RangeValue, DEC);
      //
}

void ShowLEDs() {
  // Turn on LED
  if( RangeValue <= 92) {
    digitalWrite(LED1Pin, HIGH);
  }
  if( RangeValue <= 105) { 
    digitalWrite(LED2Pin, HIGH);
    }  
  if( RangeValue <= 120) { 
    digitalWrite(LED3Pin, HIGH);
    }
  if( RangeValue <= 140) { 
    digitalWrite(LED4Pin, HIGH);
    }
  if( RangeValue <= 160) { 
    digitalWrite(LED5Pin, HIGH);
    }
  if( RangeValue <= 180) { 
    digitalWrite(LED6Pin, HIGH);
    }
  if( RangeValue <= 240) { 
    digitalWrite(LED7Pin, HIGH);
    }
  if( RangeValue <= 250) { 
    digitalWrite(LED8Pin, HIGH);
    }
  if( RangeValue <= 260) { 
    digitalWrite(LED9Pin, HIGH);
    }
  digitalWrite(LED10Pin, HIGH); 

  // Wait 3 Seconds
  delay(3000);
  
  // Turn off LEDs
  digitalWrite(LED1Pin, LOW);
  digitalWrite(LED2Pin, LOW);
  digitalWrite(LED3Pin, LOW);
  digitalWrite(LED4Pin, LOW);
  digitalWrite(LED5Pin, LOW);
  digitalWrite(LED6Pin, LOW);
  digitalWrite(LED7Pin, LOW);
  digitalWrite(LED8Pin, LOW);
  digitalWrite(LED9Pin, LOW);
  digitalWrite(LED10Pin, LOW);
  
}

void ConvertToPercent() {
int iPercent = 0;

  if (RangeValue <= 91) {
    iPercent = 100;
  }
  if (RangeValue <= 104 && RangeValue >= 92) {
    iPercent = 90;
  }
  if (RangeValue <= 119 && RangeValue >= 105) {
    iPercent = 80;
  }
  if (RangeValue <= 139 && RangeValue >= 120) {
    iPercent = 70;
  }
  if (RangeValue <= 159 && RangeValue >= 140) {
    iPercent = 60;
  }
  if (RangeValue <= 179 && RangeValue >= 160) {
    iPercent = 50;
  }
  if (RangeValue <= 239 && RangeValue >= 180) {
    iPercent = 40;
  }
  if (RangeValue <= 249 && RangeValue >= 240) {
    iPercent = 30;
  }
  if (RangeValue <= 259 && RangeValue >= 250) {
    iPercent = 20;
  }
  if (RangeValue  >= 260) {
    iPercent = 10;
  }

  RangeValue = iPercent;
}
