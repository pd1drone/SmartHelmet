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
int VisorDown = 0; // palitan mo to kung anong angle ng down ung visor
int VisorUp = 135; //palitan mo to kung anong angle ng up ung visor
int FitStrap = 180; // palitan mo to kung anong angle ng pag fit ng strap
int UnFitStrap = 0; // palitan mo to kung anong angle ng pag unfit ng strap

int IRSensorPreviousValue = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode(IRSensor, INPUT);
  pinMode(PushBtnDown, INPUT_PULLUP);
  pinMode(EnableManualVisor, INPUT_PULLUP);
  pinMode(VisorBtn, INPUT_PULLUP);
  servoVisor.attach(5);
  servoVisor.write(VisorDown);
  servoAutoFit.attach(3);
  servoAutoFit.write(UnFitStrap);
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


  while (enableVisorValue == 1) {
    int VisorValue = digitalRead(EnableManualVisor);
    if (VisorValue == 0) {
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
      servoVisor.write(VisorUp);
      IsVisorDown = false;
    }

    if (temp <= 28.0) {
      servoVisor.write(VisorDown);
      IsVisorDown = true;
    }

  } else {
    int VisorManualValue = digitalRead(VisorBtn);
    while (VisorManualValue == 1) {
      int VisorManualValue = digitalRead(VisorBtn);
      if (VisorManualValue == 0) {
        if (IsVisorDown) {
          servoVisor.write(VisorUp);
        } else if (!IsVisorDown) {
          servoVisor.write(VisorDown);
        }
        IsVisorDown = !IsVisorDown;
        break;
      }
    }
  }

  if (IRSensorValue == 0){
    IRSensorPreviousValue = 0;
  }

  if(IRSensorPreviousValue == 0){
    if (IRSensorValue == 1) {
      servoAutoFit.write(FitStrap);
      IRSensorPreviousValue = 1;
    }
  }

  if (PushbtnDownValue == 1) {
    while (true) {
      PushbtnDownValue = digitalRead(PushBtnDown);
      servoAutoFit.write(UnFitStrap);
      if (PushbtnDownValue == 0) {
        break;
      }
    }
  }
}