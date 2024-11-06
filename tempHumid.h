#include "DHT.h"
#define DHTPIN 33
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); 

/*
* Temperatursensor
*/
struct DEV_TempSensor : Service::TemperatureSensor {    

  SpanCharacteristic *temp;                                // temp-peker  

  DEV_TempSensor() : Service::TemperatureSensor() {        // kontruktor - tjeneste satt til temperatursensor

    dht.begin();                                    
    temp = new Characteristic::CurrentTemperature(-10.0);  // initialiserer nåværende-verdi 
    temp->setRange(-50, 100);                              // Setter forventet temperatur mellom -50 og 100 grader celsius.       
    Serial.print("Starter temperatur sensor...");       
    Serial.print("\n");
  }

  void loop() {
    if (temp->timeVal() > 5000) {                        // Oppdaterer hver 5 sekunder
      float temperature = dht.readTemperature(); // leser temperatur fra dht11 sensor       
      temp->setVal(temperature);        // setter temperatur til lest temperatur                   

      LOG1("Temperaturen: ");
      LOG1(temperature);
      LOG1(" ; ");
    }
  } 
};

/*
* Luftfuktighetssensor
*/
struct DEV_HumSensor : Service::HumiditySensor {     

  SpanCharacteristic *hum;                        // temp-peker  

  DEV_HumSensor() : Service::HumiditySensor() {  // kontruktor - tjeneste satt til luftfuktighetssensor

    dht.begin();                                // starter dhtt                         
    hum = new Characteristic::CurrentRelativeHumidity(50); // initialiserer nåværende-verdi til 50%
    hum->setRange(0, 100);                                 // setter forventet verdi fra 0% til 100%

    Serial.print("Starter luftfuktighetssensor...");          // start medling
    Serial.print("\n");
  } 

  void loop() {

    if (hum->timeVal() > 5000) {                // Oppdaterer hver 5 sekunder
      float humidity = dht.readHumidity();      // leser temperatur fra dht11 sensor        
      hum->setVal(humidity);                   // setter fuktighet til lest verdi       

      LOG1("Luftfuktighet: ");
      LOG1(humidity);
      LOG1(" ; ");
    }
  } 
};