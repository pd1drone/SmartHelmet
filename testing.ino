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
  delay(2000);
  servoVisor.detach();
  servoAutoFit.attach(3);
  servoAutoFit.write(UnFitStrap);
  delay(2000);
  servoAutoFit.detach();
}

void loop() {

    int PushbtnDownValue = digitalRead(PushBtnDown);
    int VisorManualValue = digitalRead(VisorBtn);

    while (VisorManualValue == 1) {
        int VisorManualValue = digitalRead(VisorBtn);
        if (VisorManualValue == 0) {
        if (IsVisorDown) {
            //servoVisor.write(VisorUp);
            servoVisor.attach(5);
            Serial.println("VISOR UP");
            for (int posUP = VisorDown; VisorUp >= posUP ; posUP += 5) { 
                servoVisor.write(posUP);   
                delay(15);      
            }
            servoVisor.detach();
        } else if (!IsVisorDown) {
            //servoVisor.write(VisorDown);
            servoVisor.attach(5);
            Serial.println("VISOR DOWN");
            for (int posDown = VisorUp; VisorDown <= posDown ; posDown -= 5) { 
                servoVisor.write(posDown);   
                delay(15);      
            }
            servoVisor.detach();
        }
        IsVisorDown = !IsVisorDown;
        break;
        }
    }

    while(PushbtnDownValue == 1){
        int PushbtnDownValue= digitalRead(PushBtnDown);
        if (PushbtnDownValue == 0) {
        if (IsUnfit) {
            //servoAutoFit.write(FitStrap);
            servoAutoFit.attach(3);
            Serial.println("FIT Strap");
            for (int pos = UnFitStrap; pos <= FitStrap; pos += 5) { 
                servoAutoFit.write(pos);   
                delay(15);      
            }
          servoAutoFit.detach();
        } else if (!IsUnfit) {
           // servoAutoFit.write(UnFitStrap);
            servoAutoFit.attach(3);
            Serial.println("UNFIT Strap");
            for (int pos = FitStrap; pos >= UnFitStrap; pos -= 5) { 
                servoAutoFit.write(pos);   
                delay(15);      
            }
          servoAutoFit.detach();
        }
        IsUnfit = !IsUnfit;
        break;
        }
    }

}