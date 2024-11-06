#include "esp32-hal-gpio.h"
#include <Stepper.h>

struct DEV_GarageDoor : Service::GarageDoorOpener {     
  int motorSpeed = 10;                                  // Farten til motor - 10 er max verdien for brukt stepper motor
  Stepper stepperMotor = Stepper(2048, 12,14,13,27);

  SpanCharacteristic *current;            // pekeren refererer til portens nårværende tilstand 
  SpanCharacteristic *target;             // pekeren refererer til portens valgt tilstand  
  SpanCharacteristic *obstruction;        // pekeren refererer detektering av hindring 

  DEV_GarageDoor() : Service::GarageDoorOpener(){                   // tjeneste satt til garasjedør åpner
        
    current = new Characteristic::CurrentDoorState(1);              // initialiserer startverdien 1 - lukket
    target = new Characteristic::TargetDoorState(1);                // initialiserer startverdien 1 - lukket
    obstruction = new Characteristic::ObstructionDetected(false);   // initialiserer false - ingen hindring oppståd

    this->motorSpeed = motorSpeed;
    stepperMotor.setSpeed(motorSpeed);
    Serial.print("Garasjeport er klar til bruk");  
    Serial.print("\n");

  } // slutt på kontruktor

  boolean update(){                              // oppdaterer inlest verdi
  int targetVal = target->getNewVal();           // om mulig henter nye verdier
  int currentVal = current->getNewVal();
  
  if(targetVal == 0){
    LOG1("Åpner garasjeporten \n");
    stepperMotor.step(2048);                    // åpner garasjeporten - maks verdi 
    obstruction->setVal(false);                 // ingen hindring oppståd

  } else if(targetVal == 1){
    LOG1("Lukker garasjeporten \n"); 
    stepperMotor.step(-2048);                   // lukker garasjeporten - min verdi 
    obstruction->setVal(false);
  }
    return(true);                             
  
  } // slutt på funksjonen

  void loop(){                                    

    if(current->getVal()==target->getVal())        // hvi nåværende-status er lik valgt-status avsluttes loop()
      return;

    if(current->getVal()==3 && random(100000)==0){    // hvis porten er på vei ned og oppdager en hindring - gitt som en tilfeldig verdi
      stepperMotor.step(0);                           // stopper motoren
      current->setVal(4);                             // nåværende-status satt til 4 - porten er "stoppet"
      obstruction->setVal(true);                      // Det har oppståd en hindring!
      LOG1("Det har oppståd en hindring!\n");
    }

    if(current->getVal()==4)                       // hvis åværende-status har blitt satt til 4 - avslutt loop()    
      return;

    if(target->timeVal()>5000)                     // Venter 5 sekunder før omgjøre valgt-status til nåværende-status
      current->setVal(target->getVal());          
  } // slutt på void loop()
  
};