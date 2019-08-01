#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "./asyncHTTPrequest.h"


#define REFRESH 100

WiFiManager wifiManager;
asyncHTTPrequest request;


#include <FastLED.h>

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define URL "http://192.168.254.7:8000/receive"
#define DATA_PIN 6
#define NUM_LEDS 61
#define VAR 100
#define SPEED 1
#define FPS 30
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending = LINEARBLEND;

int solidColour[3] = {0,0,255};
String disp = "palette";

void process(String data) {

  if (data[0] == 'X') {
    //Serial.println("Nothing new");
  }
  else {
    String type = data.substring(0, data.indexOf("."));
    String cmd = data.substring(data.indexOf(".") + 1);
    if (type == "rgb") {
      disp = "palette";
      processRgb(cmd, &setPallet);

    }
    else if(type == "solid"){
      disp = "solid";
      processRgb(cmd, &storeSolid);
      
    }
    else if(type == "beat"){
      disp = "beat";
      processRgb(cmd, &storeSolid);
    }
    else {
      Serial.print("Unrecognised type: ");
      Serial.println(type);
    }
  }
}

void sendRequest(){
    if(request.readyState() == 0 || request.readyState() == 4){
        request.open("GET",URL);
        request.send();
    }
}

void requestCB(void* optParm, asyncHTTPrequest* request, int readyState){
    if(readyState == 4){
      String res = request->responseText();
        process(res);
        Serial.println(res);
        Serial.println();
        request->setDebug(false);
    }
}
    

void setup(){
    Serial.begin(115200);
    randomSeed(micros());
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    //setPallet(210, 210, 210);
    leds[0] = CRGB(200,0,0);
    FastLED.show();
    
    WiFi.setAutoConnect(true);
    WiFi.begin();
    while(WiFi.status() != WL_CONNECTED){
        wifiManager.setDebugOutput(true);
        wifiManager.setConfigPortalTimeout(180);
        Serial.println("Connecting with WiFiManager");
        wifiManager.autoConnect("ESP8266", "ESP8266");
        yield();
    }
    request.setDebug(false);
    request.onReadyStateChange(requestCB);
    //ticker.attach(5, sendRequest);

    leds[0] = CRGB(0,200,0);
    FastLED.show();
    setPallet(0,0,200);
}

void loop() {
    if(millis() % REFRESH == 0){
      sendRequest();
    }
    static uint8_t pos = 0;
    static uint8_t pos2 = 0;
    int m = millis();
    if(m % FPS == 0){
      pos = pos + SPEED;
      pos = pos % 255;
      pos2++;
      pos = pos % 1050;
      //Serial.println(pos);
      if(disp == "palette"){
          fill(pos);
      }
      else if(disp == "beat"){
          beat(pos);
      }
      else if(disp == "solid"){
          light(solidColour[0], solidColour[1], solidColour[2]);
      }
      
      FastLED.show();
    }

}
