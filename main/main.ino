#include <WiFi.h>
#include <time.h>

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"

#define PIN 25 // Pinnen vi bruker til Neopixlene
// Det er en feil i biblioteket her på WOKWI så vi må simulere med 69 pixler
#define NUMPIXELS 77 // Antall neopixler på en hel sirkel

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

// Wifi
const char* ssid = "1ELER";
const char* wifipw = "klokkeprosjekt";

uint32_t fagFarge[10] = {
  strip.Color(0, 0, 0), 
  strip.Color(0,255,0),
  strip.Color(102,205,170),
  strip.Color(0,128,128),
  strip.Color(205,92,92),
  strip.Color(255,0,0),
  strip.Color(60,179,113),
  strip.Color(0,100,0),
  strip.Color(188,143,143)
};

int igjen = 0;

void setup() {
  Serial.begin(115200);
  
  strip.begin();
  strip.show(); // Setter alle pixlene til 'AV'

  // Starter opp trådløst nett
  WiFi.begin(ssid, wifipw);
  Serial.println("Kobler til WiFi");
  
  delay(5000); //Gi den litt tid til å koble seg til
  if(WiFi.status() == 3){
    alarm(strip.Color(0, 130, 0), 25); //GRØNN ALARM
    Serial.println("TILKOBLET");
  }
  else {
    alarm(strip.Color(130, 0, 0), 25); //RØD ALARM
    Serial.println("IKKE TILKOBLET");
  }
  Serial.print("Wifi RSSI=");
  Serial.println(WiFi.RSSI());

  //Starter opp RTC
  if (! rtc.begin()) {
    Serial.println("Kan ikke finne klokken, kanskje tom for batteri??");
    Serial.flush();
    abort();
  }
  if (!rtc.isrunning()) {
    Serial.println("Klokken er feil!");
  }
  fyllPlan(); // Fyller timeplanen inni funksjoner.h
  
  // Oppdatere RTC med ny tid fra internett
  if(rtc.isrunning() && WiFi.status() == 3){
    initTime("CET-1CEST,M3.5.0,M10.5.0/3");
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Fikk ikke ny tid fra NTC :-( ");
      alarm(strip.Color(130, 130, 0), 25); //GUL ALARM
      return;
    }
    else{
      //    **********   Overføre NTC tid til RTC:    ***************
      DateTime nyTid = DateTime(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_wday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      rtc.adjust(nyTid);
      alarm(strip.Color(0, 0, 130), 25); // BLÅ ALARM
    }
  }
}

void loop() {

  // STILL KLOKKEN! Skriv "tid" for å stille
  if (Serial.available()) {
    String kommando = Serial.readStringUntil('\n');
    if (kommando == "tid") {
      stillKlokke();
    }
  }

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