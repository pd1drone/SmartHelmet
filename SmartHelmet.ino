
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
int VisorDown = 0; // palitan mo ung value neto kung sobra sa pagbaba yung visor (increase mo lang ung 0)
int VisorUp = 135; // palitan mo lang value neto kung kulang sa pagtaas ng visor (increase mo lang ung 135)

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
  servoAutoFit.write(0);
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
      servoVisor.write(VisorUp);
      IsVisorDown = false;
    }

    if (temp <= 28.0) {
      servoVisor.write(VisorDown);
      IsVisorDown = true;
    }

  } else {
    int VisorManualValue = digitalRead(VisorBtn);
    while (VisorManualValue == 0) {
      int VisorManualValue = digitalRead(VisorBtn);
      if (VisorManualValue == 1) {
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

  if (IRSensorValue == 1){
    IRSensorPreviousValue = 1;
  }

  if(IRSensorPreviousValue == 1){
    if (IRSensorValue == 0) {
      servoAutoFit.write(180); // eto ung mag sisikip ng strap palitan mo lang value neto kung sobra ung ikot pagka 180 (decrease mo lang ung 180)
      IRSensorPreviousValue = 0;
    }
  }

  if (PushbtnDownValue == 0) {
    while (true) {
      PushbtnDownValue = digitalRead(PushBtnDown);
      servoAutoFit.write(0); // eto yung magluluwag ng strap palitan mo lang value neto kung sobra ung ikot pagka 0 (increase mo lang ung 0)
      if (PushbtnDownValue == 1) {
        break;
      }
    }
  }
}
