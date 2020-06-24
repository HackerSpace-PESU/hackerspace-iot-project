#define BLYNK_PRINT Serial 
#include <SPI.h>;
#include <ESP8266WiFi.h>;
#include <BlynkSimpleEsp8266.h>;
#include <SimpleTimer.h>;
#include <DHT.h>;
char auth[] = "auth_name";
char ssid[] = "ssid";
char pass[] = "password";

#define DHTPIN 4 
#define pirPin 6                // Input for HC-S501
int pirValue;                   // Place to store read PIR Value
int pinValue;                   //Variable to read virtual pin
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void sendSensor()
{
 float h = dht.readHumidity();
 float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

 if (isnan(h) || isnan(t)) {
 Serial.println("Failed to read from DHT sensor!");
 return;
 }
 Blynk.virtualWrite(V1, h); // Humidity for gauge
 Blynk.virtualWrite(V0, t); // Temperature for gauge
}
BLYNK_WRITE(V2)
{
 pinValue = param.asInt();    
} 

void setup()
{
 Serial.begin(115200);
 delay(10);// See the connection status in Serial Monitor
 Blynk.begin(auth, ssid, pass);
 pinMode(pirPin, INPUT);
 dht.begin();

 // Setup a function to be called every second
 timer.setInterval(1000L, sendSensor);
}

void loop()
{
  if (pinValue == HIGH)    
      {
        getPirValue();
      }
 Blynk.run(); // Initiates Blynk
 timer.run(); // Initiates SimpleTimer
}
void getPirValue(void)        //Get PIR Data
  {
   pirValue = digitalRead(pirPin);
    if (pirValue) 
     { 
       Serial.println("Motion detected");
       Blynk.notify("Motion detected");  
     }
  }
