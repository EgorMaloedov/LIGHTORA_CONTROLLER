#pragma once
#include "Arduino.h"

void blink(byte pin, byte times, unsigned int mills = 300, boolean reversed = false){
    boolean H = 1;
    boolean L = 0;
    if (reversed){
    H = 0;
    L = 1;
    }
    for (byte i = 0; i < times; i++){
        digitalWrite(pin, H);
        delay(mills);
        digitalWrite(pin, L);
        delay(mills);
    };
}