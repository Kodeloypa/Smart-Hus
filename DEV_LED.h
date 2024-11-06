#include "HomeSpan.h"
//#include "extras/PwmPin.h"  

struct DEV_LED : Service::LightBulb {         // Standard light bulb service

  int ledPin1;                                // First LED pin
  int ledPin2;                                // Second LED pin
  SpanCharacteristic *power;                  // Power characteristic - on/off control
  
  DEV_LED(int pin1) : Service::LightBulb() {    // Constructor with two pins

    power = new Characteristic::On();         // Create the On/Off characteristic
    this->ledPin1 = pin1;                     // Assign pin1 to ledPin1

    pinMode(ledPin1, OUTPUT);                 // Set ledPin1 as output
  } 

  boolean update() {                          // Update method for changing the LED state
    int value = power->getNewVal();           // Get the new On/Off value

    digitalWrite(ledPin1, value);             // Set state of ledPin1
    return true;                              // Return true to indicate successful update
  }
};


/*
* Dimmebar lys Stue
*/
struct DEV_LED2 : Service::LightBulb {         // Vanlig lyspære

  LedPin *ledPin1;                             // oppretter en ledpin-peker for første LED
  LedPin *ledPin2;                             // oppretter en ledpin-peker for andre LED
  LedPin *ledPin3;                             // oppretter en ledpin-peker for tredje LED
  SpanCharacteristic *power;                   // power-peker - refererer til egenskapen av/på
  SpanCharacteristic *level;                   // level-peker - refererer til dimme-funksjonalitet
  
  DEV_LED2(int pin1, int pin2, int pin3) : Service::LightBulb() {    // tjeneste satt til lyspære

    power = new Characteristic::On();          
    level = new Characteristic::Brightness(50);  // Initialiserer lysstyrken til 50 %
    level->setRange(5, 100, 1);                  // Verdiområde satt fra 5-100% - stige med 1%
    
    this->ledPin1 = new LedPin(pin1);            // Setter første pinne fra parameter til ledPin1
    this->ledPin2 = new LedPin(pin2);            // Setter andre pinne fra parameter til ledPin2
    this->ledPin3 = new LedPin(pin3);            // Setter tredje pinne fra parameter til ledPin3
  } 

  boolean update() {                              // oppdaterer innlest verdi
    int brightness = power->getNewVal() * level->getNewVal(); // Beregner lysstyrke basert på av/på og lysstyrkenivå
    ledPin1->set(brightness);                    // Setter lysstyrken for første LED
    ledPin2->set(brightness);                    // Setter lysstyrken for andre LED
    ledPin3->set(brightness);                    // Setter lysstyrken for tredje LED
    return true;                               
  } 
};

/*
* Dimmebar lys Kjøkken
*/
struct DEV_LED3 : Service::LightBulb {         // Vanlig lyspære

  LedPin *ledPin1;                             // oppretter en ledpin-peker for første LED
  LedPin *ledPin2;                             // oppretter en ledpin-peker for andre LED
  SpanCharacteristic *power;                   // power-peker - refererer til egenskapen av/på
  SpanCharacteristic *level;                   // level-peker - refererer til dimme-funksjonalitet
  
  DEV_LED3(int pin1, int pin2) : Service::LightBulb() {    // tjeneste satt til lyspære

    power = new Characteristic::On();          
    level = new Characteristic::Brightness(50);  // Initialiserer lysstyrken til 50 %
    level->setRange(5, 100, 1);                  // Verdiområde satt fra 5-100% - stige med 1%
    
    this->ledPin1 = new LedPin(pin1);            // Setter første pinne fra parameter til ledPin1
    this->ledPin2 = new LedPin(pin2);            // Setter andre pinne fra parameter til ledPin2
  } 

  boolean update() {                              // oppdaterer innlest verdi
    int brightness = power->getNewVal() * level->getNewVal(); // Beregner lysstyrke basert på av/på og lysstyrkenivå
    ledPin1->set(brightness);                    // Setter lysstyrken for første LED
    ledPin2->set(brightness);                    // Setter lysstyrken for andre LED
    return true;                               
  } 
};

/*
* RGB-lys
*/
struct DEV_RgbLED : Service::LightBulb {      // Vanlig lyspære

  LedPin *redPin, *greenPin, *bluePin;        // oppretter pekerer til de ulike fargene
  
  SpanCharacteristic *power;                  
  SpanCharacteristic *H;                       // referer Hue egenskap
  SpanCharacteristic *S;                       // referer til Saturation egenskap
  SpanCharacteristic *V;                       // referer to the Brightness egenskap
  
  DEV_RgbLED(int red_pin, int green_pin, int blue_pin) : Service::LightBulb(){  // tjeneste satt til lyspære

    power=new Characteristic::On();        // har engenskapet av/på
    H=new Characteristic::Hue(0);          // Gir fargetonen til lyspære
    S=new Characteristic::Saturation(0);   // Metning på lyset
    V=new Characteristic::Brightness(100); // Lysstyrken - initialiserer med en verdi på 100%
    V->setRange(5,100,1);                  // Verdiområde for lysstyrken satt fra 5-100% - stige med 1%
    
    this->redPin=new LedPin(red_pin);        // Pinnen fått fra parameter satt til rød
    this->greenPin=new LedPin(green_pin);    // Pinnen fått fra parameter satt til grønn
    this->bluePin=new LedPin(blue_pin);      // Pinnen fått fra parameter satt til blå
 
    char cBuf[128];                         
    sprintf(cBuf,"Konfigurer RGB LED: Pinner=(%d,%d,%d)\n",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    Serial.print(cBuf);
  }
  
  /*
  * Oppdaterer innlest verdi
  */  
  boolean update(){                         

    boolean p;
    float v, h, s, r, g, b;                   // bruker float for bedre nøyaktighet

    h=H->getVal<float>();                      // Henter å setter verdier til fargetonen
    s=S->getVal<float>();                      // Henter å setter verdier til metning
    v=V->getVal<float>();                      // Henter å setter verdier til lysstyrken
    p=power->getVal();                         // Henter å setter verdier for strømtilstand

    char cBuf[128];
    sprintf(cBuf,"Oppdatterer RGB LED: Pinner=(%d,%d,%d): ",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    LOG1(cBuf);

    /*
    * Informasjon om oppdatert verdier for de ulike egenskaper
    */    
    if(power->updated()){          
      p=power->getNewVal();
      sprintf(cBuf,"Power=%s->%s, ",power->getVal()?"true":"false",p?"true":"false");
    } else {
      sprintf(cBuf,"Power=%s, ",p?"true":"false");
    }
    LOG1(cBuf);
      
    if(H->updated()){
      h=H->getNewVal<float>();
      sprintf(cBuf,"H=%.0f->%.0f, ",H->getVal<float>(),h);
    } else {
      sprintf(cBuf,"H=%.0f, ",h);
    }
    LOG1(cBuf);

    if(S->updated()){
      s=S->getNewVal<float>();
      sprintf(cBuf,"S=%.0f->%.0f, ",S->getVal<float>(),s);
    } else {
      sprintf(cBuf,"S=%.0f, ",s);
    }
    LOG1(cBuf);

    if(V->updated()){
      v=V->getNewVal<float>();
      sprintf(cBuf,"V=%.0f->%.0f  ",V->getVal<float>(),v);
    } else {
      sprintf(cBuf,"V=%.0f  ",v);
    }
    LOG1(cBuf);

   
    /*
    * Konverterer fra HSV til RGB
    */  

    LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);   // HomeKit gir S og V i prosent, må skalere ned med 100

    int R, G, B;

    R=p*r*100;                                      // RGB Led-pinne opererer med prosent - må skalarer opp igjen
    G=p*g*100;                                      // Multipliseres med P(strømtilstand - 0 eller 1/ på-av)
    B=p*b*100;

    sprintf(cBuf,"RGB=(%d,%d,%d)\n",R,G,B);         // Printer verdiene til RGB
    LOG1(cBuf);

    redPin->set(R);                      // oppdatere led pinner med nye verdier
    greenPin->set(G);    
    bluePin->set(B);    
      
    return(true);                               // returnerer true
  }
};