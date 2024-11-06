#include "esp32-hal-gpio.h"

struct DEV_FAN : Service::Fan {
  int motorPin1;
  int motorPin2; //Ikke i bruk! - brukes om vil at vifte skal rotere andre retning.

  SpanCharacteristic *fanpower; 

  DEV_FAN(int motorPin1, int motorPin2) : Service:: Fan(){ // tjeneste satt til garasjedør åpner

    fanpower = new Characteristic::Active(); // har egenskapet aktivert/ikke-aktiberkt
    this->motorPin1 = motorPin1;  // Setter pinner fra parameter til motorpinnene
    this->motorPin2 = motorPin2;

    pinMode(motorPin1, OUTPUT); 
    pinMode(motorPin2, OUTPUT);

    digitalWrite(motorPin1, LOW); // Starter LOW

  } 

  boolean update(){
    digitalWrite(motorPin1, !fanpower->getNewVal()); // Setter verdien ut i fra det den henter fra HomeKit-appen.
    //digitalWrite(motorPin2, !fanpower->getNewVal()); // Får motoren til å rotere andre veien.(Ikke i bruk)
    return(true);
  }
};


