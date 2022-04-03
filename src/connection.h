#pragma once
#include "WiFiClient.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "WiFiServer.h"
#include "memory.h"
#include "strip.h"

void Split(char separator, String str, String* array){
    String st = "";
    int data_len = 0;
    for (unsigned int i = 0; str[i] != '\0'; i++){
        if (str[i] == separator){array[data_len] = st; data_len++; st = "";}
        else st+=str[i];
}
};
int lenForSplit(char separator, String str){
    int j = 0;
    for (unsigned int i = 0; str[i] != '\0'; i++)
        if (str[i] == separator) j++;
    return j + 1;    
}

class WIFI_cl{ //class WiFi
    public:
    void ANCP(String ssid, const String password = "", char channel = 1, boolean ssid_hidden = 0, byte max_users = 8){
        ANCP_SSID = ssid; ANCP_PASS = password; ANCP_CHNL = channel; ANCP_HIDDEN = ssid_hidden; ANCP_MUSERS = max_users; 
    }
    void STA(STADATA STA){
        STA_SSID = STA.SSID; STA_PASS = STA.PASS;
    }
    
    String  getANCP_SSID(){return ANCP_SSID;};
    String  getANCP_PASS(){return ANCP_PASS;};
    byte    getANCP_CHNL(){return ANCP_CHNL;};
    boolean getANCP_HIDDEN(){return ANCP_HIDDEN;};
    byte    getANCP_MUSERS(){return ANCP_MUSERS;};
    String  getSTA_SSID(){return STA_SSID;};
    String  getSTA_PASS(){return STA_PASS;};
    byte    getSTATUS(){STA_STATUS = WiFi.status(); return STA_STATUS;}; 
    void    setMODE(WiFiMode_t mode){WiFi.mode(mode);};


    private:
    String  ANCP_SSID;
    String  ANCP_PASS;
    byte    ANCP_CHNL;
    boolean ANCP_HIDDEN;
    byte    ANCP_MUSERS;
    String  STA_SSID;
    String  STA_PASS;
    byte    STA_STATUS;
    byte    MODE;
};

class BLTH_cl{ //class Bluetooth

};

class TCP{
    public:
    TCP(unsigned int port = 2307){SERVER = WiFiServer(port);};
    void begin(){
        SERVER.begin();
    }
    boolean checkClient(){
        CLIENT = SERVER.available();
        if(CLIENT) {
            AVCLIENT = CLIENT;
            return true;
        }
        else return false;
    }
    WiFiClient getAVCLIENT(){
        if (!AVCLIENT.connected())
        AVCLIENT = SERVER.available();
        return AVCLIENT;
    }
    String getTCPDATA(){
        DATA = "";
        while (AVCLIENT.available() > 0)
        DATA += char(AVCLIENT.read());
        return DATA;
    }

    String readDATA(){
        String data = DATA;
        DATA = "";
        return data;
    }
    

    private:
    String DATA = "";
    WiFiServer SERVER{2307};
    WiFiClient CLIENT;
    WiFiClient AVCLIENT;
};

class WirelessConnection{
    public:
    void setWIFI(WIFI_cl wifi){WIFI = wifi;};
    void createAP(){WiFi.mode(WIFI_AP);WiFi.softAP(WIFI.getANCP_SSID(), WIFI.getANCP_PASS(), WIFI.getANCP_CHNL(), WIFI.getANCP_HIDDEN(), WIFI.getANCP_MUSERS()); APIP = WiFi.localIP();
    while(WiFi.softAPgetStationNum() < 1) blink(2,1,100,true);};
    boolean connectToAP(){
        WiFi.mode(WIFI_STA);
        byte timesToConnect = 0;
        while (WIFI.getSTATUS() != WL_CONNECTED && timesToConnect < 3){
            WiFi.begin(WIFI.getSTA_SSID(), WIFI.getSTA_PASS());
            timesToConnect ++;
            delay(10000);
        }
        if (WIFI.getSTATUS() == WL_CONNECTED) {STAIP = WiFi.localIP() ;return 1;}
        else return 0;
    }
    STADATA getSTA_TCP(){
        createAP();
        TCP STA_TCP;
        STA_TCP.begin();
        while (!STA_TCP.checkClient()) delay(1);
        while (STA_TCP.getTCPDATA() == "")  blink(2,1,500,true);
        String TCPDATA = STA_TCP.readDATA();
        String data[lenForSplit(':', TCPDATA)];
        Split(':', TCPDATA,data);
        STADATA STA;
        STA.SSID = data[0];
        STA.PASS = data[1];
        return STA;
    }

    private:
    WIFI_cl WIFI;
    BLTH_cl BLTH;
    IPAddress APIP;
    IPAddress STAIP;
};