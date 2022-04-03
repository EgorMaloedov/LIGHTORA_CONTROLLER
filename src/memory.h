#pragma once
#include "Arduino.h"
#include "ESP_EEPROM.h"

struct STRIPDATA
{
    unsigned int LENGTH;
    boolean POWER;
    struct RGB{
        byte R = 255;
        byte G = 255;
        byte B = 255;
    } COLOR;
    String  MODE = "SOLID";
    boolean ACTIVATED = false;
} LN;

struct STADATA
{
    String SSID;
    String PASS;
} ST;


class MEMORY{
    public:
    MEMORY(){EEPROM.begin(2048);EEPROM.get(0,MFDATA);};
    void Save(){
        EEPROM.put(0,MFDATA); EEPROM.commit();};
    void SetSTA(STADATA STAD){
        MFDATA.STA.SSID = STAD.SSID; 
        MFDATA.STA.PASS = STAD.PASS;
        };
    void SetSTRIP(STRIPDATA STRIPD){
        MFDATA.STRIP.POWER = STRIPD.POWER;
        MFDATA.STRIP.LENGTH = STRIPD.LENGTH;
        MFDATA.STRIP.COLOR = STRIPD.COLOR;
        MFDATA.STRIP.MODE = STRIPD.MODE;
        };
    STADATA getSTA(){return MFDATA.STA;};
    STRIPDATA getSTRIP(){return MFDATA.STRIP;};
    void Clear(){for (unsigned int i = 0; i < EEPROM.length(); i++) EEPROM.put(i, 0); EEPROM.commit();};

    private:
    struct MEMORYDATA
    {
        STADATA      STA;
        STRIPDATA   STRIP;
    }MFDATA;
    
};