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
  pinMode(PushBtnDown, INPUT_PULLUP);
  pinMode(VisorBtn, INPUT_PULLUP);
  servoVisor.attach(5);
  servoVisor.write(VisorDown);
  servoVisor.detach();
  servoAutoFit.attach(3);
  servoAutoFit.write(UnFitStrap);
  servoAutoFit.detach();
}

void loop() {

    int PushbtnDownValue = digitalRead(PushBtnDown);
    int VisorManualValue = digitalRead(VisorBtn);

    Serial.println(VisorManualValue);

    while (VisorManualValue == 0) {
        int VisorManualValue = digitalRead(VisorBtn);
        if (VisorManualValue == 1) {
        if (IsVisorDown) {
            servoVisor.write(VisorUp);
            Serial.println("VISOR UP")

        } else if (!IsVisorDown) {
            servoVisor.write(VisorDown);
            Serial.println("VISOR DOWN")
        }
        IsVisorDown = !IsVisorDown;
        break;
        }
    }

    while(PushbtnDownValue == 0){
        int PushbtnDownValue= digitalRead(PushBtnDown);
        if (PushbtnDownValue == 1) {
        if (IsUnfit) {
           servoAutoFit.write(FitStrap);
           Serial.println("FIT Strap")
        } else if (!IsUnfit) {
           servoAutoFit.write(UnFitStrap);
           Serial.println("UNFIT Strap")
        }
        IsUnfit = !IsUnfit;
        break;
        }
    }

}