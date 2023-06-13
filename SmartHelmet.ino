
#include <Servo.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Push button
const int IRSensor = 2;
const int PushBtnDown = 6;
const int EnableManualVisor = 9;
const int VisorBtn = 10;

// Servo motor
Servo servoAutoFit;
Servo servoVisor;

bool IsAutomaticVisor = true;
bool IsVisorDown = true;
int VisorDown = 0;
int VisorUp = 135;

int IRSensorPreviousValue = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(IRSensor, INPUT);
  pinMode(PushBtnDown, INPUT_PULLUP);
  pinMode(EnableManualVisor, INPUT_PULLUP);
  pinMode(VisorBtn, INPUT_PULLUP);
}

void loop() {

  float temp = dht.readTemperature();
  if (isnan(temp)) {
    temp = 0.0;
  }
  int IRSensorValue = digitalRead(IRSensor);
  int PushbtnDownValue = digitalRead(PushBtnDown);
  int enableVisorValue = digitalRead(EnableManualVisor);

  Serial.println(enableVisorValue);


  while (enableVisorValue == 0) {
    int VisorValue = digitalRead(EnableManualVisor);
    if (VisorValue == 1) {
      IsAutomaticVisor = !IsAutomaticVisor;
      break;
    }
  }

  Serial.print("Automatic: ");
  Serial.println(IsAutomaticVisor);
  if (IsAutomaticVisor) {
    Serial.print(temp);
    Serial.print(F("°C "));
    Serial.println();
    if (temp >= 31.0) {
      servoVisor.attach(5);
      servoVisor.write(180); // palitan mo to ng 0 para umikot ng opposite
      delay(3000); // palitan mo to kung need mo magdagdag or magbawas ng time ng pagikot ng servo
      servoVisor.detach();
      IsVisorDown = false;
    }

    if (temp <= 28.0) {
      servoVisor.attach(5);
      servoVisor.write(0); // palitan mo to ng 180 para umikot ng opposite
      delay(3000); // palitan mo to kung need mo magdagdag or magbawas ng time ng pagikot ng servo
      servoVisor.detach();
      IsVisorDown = true;
    }

  } else {
    int VisorManualValue = digitalRead(VisorBtn);
    while (VisorManualValue == 0) {
      int VisorManualValue = digitalRead(VisorBtn);
      if (VisorManualValue == 1) {
        if (IsVisorDown) {
          servoVisor.attach(5);
          servoVisor.write(180); // palitan mo to ng 0 para umikot ng opposite
          delay(3000); // palitan mo to kung need mo magdagdag or magbawas ng time ng pagikot ng servo
          servoVisor.detach();
        } else if (!IsVisorDown) {
          servoVisor.attach(5);
          servoVisor.write(0); // palitan mo to ng 0 para umikot ng opposite
          delay(3000); // palitan mo to kung need mo magdagdag or magbawas ng time ng pagikot ng servo
          servoVisor.detach();
        }
        IsVisorDown = !IsVisorDown;
        break;
      }
    }
  }

  if (IRSensorValue == 1){
    IRSensorPreviousValue = 1;
  }

  if(IRSensorPreviousValue == 1){
    if (IRSensorValue == 0) {
      servoAutoFit.attach(3);
      servoAutoFit.write(180);
      delay(5000);
      servoAutoFit.detach();
      IRSensorPreviousValue = 0;
    }
  }

  if (PushbtnDownValue == 0) {
    while (true) {
      PushbtnDownValue = digitalRead(PushBtnDown);
      servoAutoFit.attach(3);
      servoAutoFit.write(0);

      if (PushbtnDownValue == 1) {
        servoAutoFit.detach();
        break;
      }
    }
  }
}
