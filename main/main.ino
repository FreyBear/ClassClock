#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"

#define PIN 25 // Pinnen vi bruker til Neopixlene
// Det er en feil i biblioteket her på WOKWI så vi må simulere med 69 pixler
#define NUMPIXELS 68 // Antall neopixler på en hel sirkel


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
RTC_DS1307 rtc;

#include "funksjoner.h" // Inkluder den nye header-filen
#include "animasjoner.h" // Inkluderer animasjonene

timePlan plan[40];

// Statusbits for animasjon:
bool erFriminutt = false; // Blir True når det blir friminutt
bool erTime = false;      // Blir True når det blir time 
bool erFerdig = false;    // Blir True når skoledagen er over
bool erHelg = false;      // Blir True når det er helg
byte gjeldende = 0;    // Gjeldende aktivitet, Aktiviteten som er akkurat nå
byte forrige = 0;      // Forrige aktivitet, Aktiviteten vi hadde for 1 millisekund siden



int igjen = 0;

void setup() {
  Serial.begin(115200);
  
  strip.begin();
  strip.show(); // Setter alle pixlene til 'AV'

  if (! rtc.begin()) {
    Serial.println("Kan ikke finne klokken, kanskje tom for batteri??");
    Serial.flush();
    abort();
  }
  if (!rtc.isrunning()) {
    Serial.println("Klokken er feil!");
  }
  fyllPlan(); // Fyller timeplanen inni funksjoner.h

}

void loop() {

  // STILL KLOKKEN! Skriv "tid" for å stille


  DateTime now = rtc.now();
  gjeldende = fagNummer(now, plan);

  if(gjeldende != forrige){
    Serial.print("NY AKTIVITET!!!");
    
    switch(gjeldende){
      case 0:
        if(now.dayOfTheWeek() == 5){
          erHelg = true;
        }
        else{
          erFerdig = true;
        }
        break;
      case 1:
        erFriminutt = true;
        break;
      default:
        erTime = true;
        break;
  }

    forrige = gjeldende;  // Gjør at vi bare kjører dette en gang hver gang vi 
  }

  igjen = tidIgjen(now.dayOfTheWeek(), now.hour(), now.minute(), now.second());
  
  Serial.print("Akkurat nå har vi: ");
  Serial.print(fag(now, plan));
  Serial.print(" (");
  Serial.print(fagNummer(now, plan));
  Serial.println(")");
  Serial.print("Klokken er: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());
  Serial.print("Dag i uken: ");
  Serial.println(now.dayOfTheWeek());
  if( igjen > 0){
    Serial.print("Det er ");
    Serial.print(igjen);
    Serial.println(" sek igjen av timen");
  }
  




//*************** ANIMASJONER ********************

  if (erFriminutt){
    friminuttAnimasjon();
    erFriminutt = false;
  }

  if (erTime){
    alarm(strip.Color(130, 0, 0), 25);
    blink(); 
    alarm(strip.Color(130, 0, 0), 25);
    skruAv();
    erTime = false;
  }
  
  if (erFerdig){
    ring(strip.Color(74, 129, 130), 30);
    runde(strip.Color(74, 129, 130), 50); 
    skruAv();
    erFerdig = false;
  }

  if (erHelg){
    HelgAnimasjon();
    erHelg = false;
  }

//************************************************
  

   
  // Oppdater LEDstrip mellom strip.clear() og strip.show()
  strip.clear();  
  
  nedTelling(igjen, fagFarge[gjeldende]); 

  strip.show();
  delay(100);
}
