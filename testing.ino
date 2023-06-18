#include <Servo.h>

// Push button
const int PushBtnDown = 6;
const int VisorBtn = 10;

// Servo motor
Servo servoAutoFit;
Servo servoVisor;

bool IsVisorDown = true;
bool IsUnfit = true;
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

    int PushbtnDownValue = digitalRead(PushBtnDown);
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

    while(PushbtnDownValue == 1){
        int PushbtnDownValue= digitalRead(PushBtnDown);
        if (PushbtnDownValue == 0) {
        if (IsUnfit) {
            servoAutoFit.write(FitStrap);
        } else if (!IsUnfit) {
            servoAutoFit.write(UnFitStrap);
        }
        IsUnfit = !IsUnfit;
        break;
        }
    }
    
}