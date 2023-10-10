#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

const char* ssid     = "T-Mobile_Swiatlowod_5GHz_E4D0";
const char* password = "********";
const char* host = "ADMIN-MAHOO2I1N";

#define POTENTIOMETER_PIN A0
#define PHOTORESISTOR_PIN A1
#define MOISTURE_PIN A2
#define BUTTON_PIN 2 
#define DHTPIN 3 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int moisture = 0;
int ledPin = 13;    
int button      = 0;    
int lastButton = 0;     
int ledState         = 0;     

void setup() 
{
  Serial.begin(9600);
  pinMode(PHOTORESISTOR_PIN, INPUT);
 

//---------------
   pinMode(ledPin, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT);
//------------------------
    dht.begin();

   Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
void loop()
{
  int data = analogRead(POTENTIOMETER_PIN);
  int potentiometer = map(data, 0, 1023, 0, 100);
  Serial.println("NEW");
  Serial.print("Potentiometer at ");
  Serial.print(potentiometer);
  Serial.println("%");
//------------------------
   button = digitalRead(BUTTON_PIN);


  if (button != lastButton) {
    
   
    if (button == 1) { 
      if(ledState==1) ledState=0;
      else            ledState=1;         
    }
    
  
    lastButton = button;

  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  //-------
  int photoresistor = analogRead(A1);
  Serial.print("Photoresistor: ");
  Serial.println(photoresistor);


  ------------

   moisture = analogRead(MOISTURE_PIN);
  Serial.print("{\"Hum\":");
 Serial.print(humidity);

 Serial.print(",\"Temp\":");
 Serial.print(temperature);

Serial.print(",\"Butt\":");
 Serial.print(button);

Serial.print(",\"Poten\":");
 Serial.print(potentiometer);

 Serial.print(",\"Photo\":");
 Serial.print(photoresistor);

 
 Serial.print(",\"Moist \":");
 Serial.print(moisture);
 Serial.println("}");


    Serial.print("connecting to ");
    Serial.println(host);

    WiFiClient client;
    const int httpPort = 49713;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

 
 client.print(String("GET http://ADMIN-MAHOO2I1N/arduino/connect.php?") + 
                          ("&temperature=") + temperature +
                          ("&humidity=") + humidity +
                          ("&photoresistor=") + photoresistor +
                          ("&potentiometer=") + temperature +
                          ("&button=") + button +
                          ("&moisture=") + moisture +
                          " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 1000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

 
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        
    }

    Serial.println();
    Serial.println("closing connection");




  delay(1000);
}


