#include "ArduinoOTA.h"
#include "connection.h"
#include "Arduino.h"
#include "memory.h"
#include "ESP_EEPROM.h"
#include "strip.h"
#include "config.h"
#include "FastLED.h"

#define NUM_LEDS 166
#define STRIP_PIN 5

CRGB leds[NUM_LEDS];
String APSSID = "Lightora";
String APPASS = "23072004";

STADATA STA;
STRIPDATA STRIP;
MEMORY MMEMORY = MEMORY();
TCP TCPS[NUM_OF_PORTS];

WIFI_cl WIFI;
WirelessConnection WC;

void OPEN_TCP_PORTS(){
  for (unsigned short int i = 0; i < NUM_OF_PORTS; i++){
    TCPS[i] = TCP(OPEN_PORTS[i]);
    TCPS[i].begin();
  }
}

void setup() {
  FastLED.addLeds<WS2813, STRIP_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  digitalWrite(2,1);
  STA = MMEMORY.getSTA();
  STRIP = MMEMORY.getSTRIP();
  WIFI.STA(STA);
  WIFI.ANCP(APSSID, APPASS, (char) 10, false, 1);
  WC.setWIFI(WIFI);
}

void loop() {
  FastLED.show();
  if (WIFI.getSTATUS() != WL_CONNECTED){
      while (!WC.connectToAP()){
        STA = WC.getSTA_TCP();
        WIFI.STA(STA);
        WC.setWIFI(WIFI);
      }
      ArduinoOTA.begin();
      OPEN_TCP_PORTS();
      if (MMEMORY.getSTA().SSID != STA.SSID && STA.SSID != ""){
        MMEMORY.SetSTA(STA);
        MMEMORY.Save();
      }
    }
  else{
  ArduinoOTA.handle();
  String TCPDATA = "";
  for (unsigned short int i = 0; i < NUM_OF_PORTS; i++){
  TCPS[i].checkClient();
  if (TCPS[i].getAVCLIENT()){ 
  TCPS[i].getTCPDATA();
  TCPDATA = TCPS[i].readDATA();
  }
  }
  if (TCPDATA == "On") for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(255,255,255);
  

  if (TCPDATA == "Off") for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(0,0,0);
}
}


