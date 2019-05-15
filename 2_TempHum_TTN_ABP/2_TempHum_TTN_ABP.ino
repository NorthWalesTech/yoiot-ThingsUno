#include <TheThingsNetwork.h>
#include <CayenneLPP.h>
#include "DHT.h"

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";


#define freqPlan TTN_FP_EU868
#define loraSerial Serial1

TheThingsNetwork ttn(loraSerial, Serial, freqPlan);
CayenneLPP lpp(51);



#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  loraSerial.begin(57600);
  Serial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!Serial && millis() < 10000);

  // set keys
  ttn.personalize(devAddr, nwkSKey, appSKey);

  // print device details
  ttn.showStatus();
}

void loop() 
{
    Serial.println("-- Begin Loop");
    
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
        Serial.println("Sending...");

        // make the payload
        lpp.reset();
        lpp.addTemperature(1, t);
        lpp.addRelativeHumidity(1, h);
     
        // Send it off
        ttn.sendBytes(lpp.getBuffer(), lpp.getSize());

        Serial.println("... Sent");
    }

    Serial.println("-- End Loop");
    // wait
    delay(10000);  
}
