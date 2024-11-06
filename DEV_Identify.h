/*
* Informasjon om tilbehør
*/
struct DEV_Identify : Service::AccessoryInformation {

  int nBlinks;                    // antall ganger tilbehør skal blinke i identifiseringsfase
  SpanCharacteristic *identify;   // identify - peker
  
  DEV_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version, int nBlinks) : Service::AccessoryInformation(){
    
    new Characteristic::Name(name);                   // oppretter alle de nødvendige egenskapene med verdier satt basert på argumentene ovenfor
    new Characteristic::Manufacturer(manu);
    new Characteristic::SerialNumber(sn);    
    new Characteristic::Model(model);
    new Characteristic::FirmwareRevision(version);
    identify=new Characteristic::Identify();          // lagre en referanse til identifikasjonskjennetegn for bruk nedenfor

    this->nBlinks=nBlinks;                            // lagre antall blinking 

    pinMode(homeSpan.getStatusPin(),OUTPUT);          

  }

  boolean update(){
       
    for(int i=0;i<nBlinks;i++){
      digitalWrite(homeSpan.getStatusPin(),HIGH);
      delay(250);
      digitalWrite(homeSpan.getStatusPin(),LOW);
      delay(250);
    }
    return(true);                              
  }  
};

