#ifndef ANIMASJONER_H
#define ANIMASJONER_H

#include <Adafruit_NeoPixel.h>

//************************************************************
//************          TIMEANIMASJON    ********************
//************************************************************

void blink() {
   for (int blink = 0; blink < 5; blink++) {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (blink % 3 == 0) {
      strip.setPixelColor(i, 130, 0, 0); 
    } else if (blink % 3 == 1) {
      strip.setPixelColor(i, 130, 0, 0); 
    } else {
      strip.setPixelColor(i, 130, 0, 0); 
    }
  }
  strip.show();
  delay(50);
  strip.clear();
  strip.show();
  delay(50);
 }
}

void alarm(uint32_t c, uint8_t wait) {
  for (int j=0; j<6; j++) {  
    for (int q=0; q < 7; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i+=7) {
        strip.setPixelColor(i+q, c);    
      }
      strip.show();
      delay(wait);
      for (uint16_t i=0; i < strip.numPixels(); i+=7) {
        strip.setPixelColor(i+q, 0);    
      }
    }
  }
}

//************************************************************
//************   ANIMASJON FOR FRIMINUTT  ********************
//************************************************************




//************************************************************
//************ ANIMASJON FERDIG FOR DAGEN ********************
//************************************************************

void ring(uint32_t farge, uint8_t delayTid) {
  for(int i=0; i<NUMPIXELS; i++) { 
    strip.setPixelColor(i, farge);
    strip.show(); 
    delay(delayTid); 
  } 
}

void runde(uint32_t c, uint8_t wait) {
  for (int j=0; j<6; j++) {  
    for (int q=0; q < 11; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i+=11) {
        strip.setPixelColor(i+q, c);    
      }
      strip.show();
      delay(wait);
      for (uint16_t i=0; i < strip.numPixels(); i+=11) {
        strip.setPixelColor(i+q, 0);    
      }
    }
  }
}

void skruAv() {
  for(int i=0; i<NUMPIXELS; i++) {
    strip.setPixelColor(i, 0); 
  }
  strip.show(); 
}

//************************************************************
//************          HELGEANIMASJON    ********************
//************************************************************


#endif