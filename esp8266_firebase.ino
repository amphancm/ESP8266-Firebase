#include <ESP8266WiFi.h>
#include <WiFiConnector.h>
#include <FirebaseArduino.h>

WiFiConnector *wifi;
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <ESP_Adafruit_SSD1306.h>

#include "declare.h"
#include "init_wifi.h"


void init_hardware(){
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");
}

void showTemp(float temp,float hud) {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("   Temp  ");
  
  display.setTextSize(3);                                 // Size4 = 5 digit , size3 = 7 digits
  //display.setTextColor(BLACK, WHITE);                   // 'inverted' text
  display.setTextColor(WHITE);
  display.setCursor(0,24);
 
  display.print(temp);
  display.println("C"); 
  display.display();
}

void showHud(float temp,float hud) {

  display.clearDisplay();  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(" Humidity ");
   
  display.setTextSize(3);                               // Size4 = 5 digit , size3 = 7 digits
  //display.setTextColor(BLACK, WHITE);                 // 'inverted' text
  display.setTextColor(WHITE);
  display.setCursor(0,24);
 
  display.print(hud);
  display.println("%");  
  display.display();
}


//==================================================
void setup()   {                
//==================================================
  
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  
  dht.begin();            // dht begin

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x78>>1);
  // init done
  

  // Clear the buffer.
  display.clearDisplay();
    
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,0);
  display.println("  Weather  ");
  
  display.setTextSize(3);               // Size4 = 5 digit , size3 = 7 digits
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextColor(WHITE);
  display.setCursor(0,18);
  display.println("Station");
    
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,52);
  display.println("Version 0.4");
 
  display.display();
  delay(2000);
  
  
  // WiFi Connect
  Serial.print("CONNECTING TO ");
  Serial.println(wifi->SSID() + ", " + wifi->psk());
  
  display.clearDisplay();  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,0);
  display.println("Connecting");
   

  wifi->on_connecting([&](const void* message){
    char buffer[70];
    //sprintf(buffer, "[%d] connecting -> %s ", wifi->counter, (char*) message);
    //Serial.println(buffer);
        
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
    //lp++;
    //if(lp>=20) break;
  });

  
  
  wifi->on_smartconfig_waiting([&](const void* message){
    display.clearDisplay();    
    display.setTextSize(2);       display.setTextColor(WHITE);  
    display.setCursor(0,0);       display.println("Smart");  
    display.setCursor(0,18);      display.println("CONFIG");
    //display.setCursor(0,36);      display.println(WiFi.localIP()); 
    display.display();      
  });
  
  

  wifi->on_connected([&](const void* message){
    // Print the IP address
    Serial.print("WIFI CONNECTED ");
    Serial.println(WiFi.localIP());
  });

  wifi->connect();
  
  
  //WiFi.begin(ssid, password);
  Serial.println("");                 // Clear the buffer.
  
  display.clearDisplay();
    
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,0);
  display.println("Connecting");
  

  
  while (WiFi.status() != WL_CONNECTED) {    // Wait for connection
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
    lp++;
    if(lp>=20) break;
  }
  
  
  Serial.println("");
  Serial.print("Connected to ");
  //Serial.println(ssid);
  Serial.print("IP address: ");  Serial.println(WiFi.localIP());
  
  display.clearDisplay();    
  display.setTextSize(2);       display.setTextColor(WHITE);  
  display.setCursor(0,0);       display.println(wifi->SSID());  
  display.setCursor(0,18);      display.println(WiFi.localIP());
  //display.setCursor(0,36);      display.println(WiFi.localIP());
  
  display.display();
  delay(2000);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  
}//setup



//===============================================================
void loop() {
 
 
  unsigned long currentMillis = millis();
    
  if(currentMillis - previousMillis >= interval) {     
      previousMillis = currentMillis;
      digitalWrite(BUILTIN_LED,LOW);
    
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius
      float t = dht.readTemperature();
    
       
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,22);
        display.println("Sensor");
        display.println("Error !!!");
        display.display();

        h = 0; t = 0;
        //delay(2000);
        //return;
      }
      
      // show in serial monitor
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temp: "); 
        Serial.print(t);
        Serial.print(" *C \n");
      
      if (showState == LOW){
        showState = HIGH; 
        showHud(t,h);        
      }
      else{    
         showState = LOW; 
         showTemp(t,h);                      // show temp
      }
      
      
    
     lp++;      
      if(lp >= lp_time){
        lp=0;

        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["temperature"] = t;
        root["humidity"] = h;
        
        // append a new value to /logDHT
        String name = Firebase.push("/sensor/dht", root);
        // handle error
        if (Firebase.failed()) {
            Serial.print("Firebase Pushing /sensor/dht failed:");
            Serial.println(Firebase.error()); 
            display.clearDisplay();  
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.println(" Firebase ");
             
            display.setTextSize(3);                               // Size4 = 5 digit , size3 = 7 digits
            //display.setTextColor(BLACK, WHITE);                 // 'inverted' text
            display.setTextColor(WHITE);
            display.setCursor(0,24);
                   
            display.println("ERROR");  
            display.display();
            
            digitalWrite(BUILTIN_LED,HIGH); 
            delay(2000);             
            lp = 10;                              // repeat to sent again 
            return;
        }else{
            Serial.print("Firebase Pushed /sensor/dht ");
            Serial.println(name);
         
            display.clearDisplay();  
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(0,0);
            display.println(" Firebase ");
             
            display.setTextSize(2);                               // Size4 = 5 digit , size3 = 7 digits
            //display.setTextColor(BLACK, WHITE);                 // 'inverted' text
            display.setTextColor(WHITE);
            display.setCursor(0,24);
                   
            display.println("Update OK");  
            display.display();
            digitalWrite(BUILTIN_LED,HIGH); 
            delay(2000); 
        }
      }
      digitalWrite(BUILTIN_LED,HIGH);                   
  }
  
 
}//Loop




