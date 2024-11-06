#include "HomeSpan.h" 
#include "DEV_LED.h" 
#include "FAN.h"
#include "tempHumid.h"
// #include "GARAGE.h"
#include "DEV_Identify.h"

// KODE: 466-37-726

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Bridges,"Smarthus  Kodeløypa");

  // Temperatursensor
  new SpanAccessory();
   new DEV_Identify("Temp", "HomeSpan", "11","DHT","0",0);
      new Service::HAPProtocolInformation();
        new Characteristic::Version("1.1.0");
          new DEV_TempSensor(); 

  // Luftfugtihetssensor
  new SpanAccessory();
    new DEV_Identify("Fuktighet", "HomeSpan", "11","DHT","0",0);
      new Service::HAPProtocolInformation();
        new Characteristic::Version("1.1.0");
          new DEV_HumSensor(); 

    // // Vanlig Lys
    // new SpanAccessory(); 
    //   new Service::AccessoryInformation(); 
    //     new Characteristic::Identify();
    //     new Characteristic::Name("Lys");                
    //   new DEV_LED(22);                // Av/På lys - pinne 2

    // Dimmebar LED lys
    new SpanAccessory(); 
      new Service::AccessoryInformation();    
        new Characteristic::Identify();
        new Characteristic::Name("LED Stue");                       
      new DEV_LED2(18, 19, 21);        //Ledpinne 4

      // Dimmebar LED lys
    new SpanAccessory(); 
      new Service::AccessoryInformation();    
        new Characteristic::Identify();
        new Characteristic::Name("LED Kjøkken");                       
      new DEV_LED3(22, 23);        //Ledpinne 4

    // RGB - LED lys
    new SpanAccessory();                                                          
    new Service::AccessoryInformation();    
      new Characteristic::Identify();               
      new Characteristic::Name("RGB-LED Stue"); 
    new DEV_RgbLED(25,26,27);  
    
    // Vifte 
    new SpanAccessory();
      new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name("Vifte");   
        new Service::HAPProtocolInformation();
        new Characteristic::Version("1.1.0");
      new DEV_FAN(32, 34); // Koplet til pinne 25, 35(ikke koplet til)
} 

void loop(){
  homeSpan.poll();
} 
