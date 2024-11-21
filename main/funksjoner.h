#ifndef FUNKSJONER_H
#define FUNKSJONER_H

#include <Adafruit_NeoPixel.h>

// Definer variablene her

struct timePlan {
  byte ukedag;              // Mandag = 1 --> fredag = 5
  String startTid;          // (tt:mm) f.eks. "08:10"
  byte varighet;            // (minutt)
  byte fag;                 // 0 = Ingenting 
                            // 1 = Friminutt 
                            // 2 = El- kretser og nettverk
                            // 3 = Energi og styresystemer 
                            // 4 = Norsk
                            // 5 = Engelsk
                            // 6 = Matte
                            // 7 = Naturfag
                            // 8 = Gym
};

extern timePlan plan[40]; // Deklarer plan som en ekstern variabel

// Funksjoner:

void fyllPlan() {
    // DAG          Klokke  Varighet  Fag
    // 1 = Mandag   "tt:mm" mm (int)  0 = Ingenting 
    // 2 = Tirsdag                    1 = Friminutt 
    // 3 = Onsdag                     2 = El- kretser og nettverk
    // 4 = Torsdag                    3 = Energi og styresystemer 
    // 4 = Fredag                     4 = Norsk
    //                                5 = Engelsk
    //                                6 = Matte
    //                                7 = Naturfag
    //                                8 = Gym

  // Mandag
  plan[0] = {1, "08:10", 45, 2}; 
  plan[1] = {1, "08:55", 45, 3}; 
  plan[2] = {1, "09:40", 20, 1}; 
  plan[3] = {1, "10:00", 90, 4}; 
  plan[4] = {1, "11:30", 30, 1}; 
  plan[5] = {1, "12:00", 90, 8}; 
  plan[6] = {1, "13:30", 10, 1}; 
  plan[7] = {1, "13:40", 90, 5};  

  // Tirsdag 
  plan[8] = {2, "08:10", 90, 6}; 
  plan[9] = {2, "09:40", 20, 1}; 
  plan[10] = {2, "10:00", 90, 3}; 
  plan[11] = {2, "11:30", 30, 1}; 
  plan[12] = {2, "12:00", 90, 3}; 
  plan[13] = {2, "13:30", 10, 1}; 
  plan[14] = {2, "13:40", 45, 3}; 

  // Onsdag 
  plan[15] = {3, "08:10", 90, 6}; 
  plan[16] = {3, "09:40", 20, 1}; 
  plan[17] = {3, "10:00", 45, 6}; 
  plan[18] = {3, "10:45", 90, 4};  

  // Torsdag 
  plan[19] = {4, "08:10", 90, 2};  
  plan[20] = {4, "09:40", 20, 1};  
  plan[21] = {4, "10:00", 90, 2};  
  plan[22] = {4, "11:30", 30, 1};  
  plan[23] = {4, "12:00", 45, 2};  
  plan[24] = {4, "12:45", 45, 5};  
  plan[25] = {4, "13:30", 10, 1};  
  plan[26] = {4, "13:40", 90, 5};   

  // Fredag 
  plan[27] = {5, "08:55", 45, 2}; 
  plan[28] = {5, "09:40", 20, 1}; 
  plan[29] = {5, "10:00", 90, 3}; 
  plan[30] = {5, "11:30", 30, 1}; 
  plan[31] = {5, "12:00", 90, 3}; 
  plan[32] = {5, "13:30", 10, 1}; 
  plan[33] = {5, "13:40", 90, 7}; 
}

//************************************************************
//************           KLOKKE           ********************
//************************************************************

void klokke(int tim, int min, int sek) {
  int minuttsiden = tim * 60 + min;  // Antall minutter siden kl. 12 eller 00
  int sekunditimen = min * 60 + sek; // Antall sekunder siden sist hele time
  
   // Sett timeviser
  int timePixel = map(minuttsiden, 0, 719, 0, NUMPIXELS - 1); //60min x 12tim = 720min
  strip.setPixelColor(timePixel, strip.Color(0, 255, 0)); // Grønn pixel som viser timehånden

  // Sett minuttviser
  int minuttPixel = map(sekunditimen, 0, 3599, 0, NUMPIXELS - 1);
  strip.setPixelColor(minuttPixel, strip.Color(255, 0, 0)); // Rød pixel som viser minutthånden
}


//************************************************************
//************            FAG             ********************
//************************************************************

String fag(DateTime now, timePlan plan[])
{
  const String fagList[] = {"Ingenting", "Friminutt", "El. kretser og nettverk",
                            "Energi og styresystemer", "Norsk", "Engelsk", "Matte",
                            "Naturfag", "Gym"};

  byte dag = now.dayOfTheWeek();
  if (dag == 0 || dag == 6) {
    return "Det er helg i dag.";
  }

  for (int i = 0; i < 40; i++) {
    if (dag == plan[i].ukedag) {
      // Calculate the end time of the lesson
      String startTid = plan[i].startTid;
      byte varighet = plan[i].varighet;
      
      // Convert start time to minutes
      int startHour = startTid.substring(0, 2).toInt();
      int startMinute = startTid.substring(3, 5).toInt();
      int startTotalMinutes = startHour * 60 + startMinute;

      // Convert current time to minutes
      int currentTotalMinutes = now.hour() * 60 + now.minute();

      // Check if current time falls within the lesson time frame
      if (currentTotalMinutes >= startTotalMinutes && currentTotalMinutes < startTotalMinutes + varighet) {
        byte fagIndex = plan[i].fag;
        return fagList[fagIndex];
      }
    }
  }

  return fagList[0];
}

//************************************************************
//************        FAGNUMMER           ********************
//************************************************************

byte fagNummer(DateTime now, timePlan plan[]){
  byte dag = now.dayOfTheWeek();
  if (dag == 0 || dag == 6) {
    return 0;
  }

  for (int i = 0; i < 40; i++) {
    if (dag == plan[i].ukedag) {
      // Calculate the end time of the lesson
      String startTid = plan[i].startTid;
      byte varighet = plan[i].varighet;
      
      // Convert start time to minutes
      int startHour = startTid.substring(0, 2).toInt();
      int startMinute = startTid.substring(3, 5).toInt();
      int startTotalMinutes = startHour * 60 + startMinute;

      // Convert current time to minutes
      int currentTotalMinutes = now.hour() * 60 + now.minute();

      // Check if current time falls within the lesson time frame
      if (currentTotalMinutes >= startTotalMinutes && currentTotalMinutes < startTotalMinutes + varighet) {
        return plan[i].fag;
      }
    }
  }
  return 0;
}

//************************************************************
//************          STILL KLOKKE      ********************
//************************************************************


//************************************************************
//************ Sekunder igjen av timen    ********************
//************************************************************

int tidIgjen(int currentDay, int currentHour, int currentMinute, int currentSecond){
    int currentfag = -1;
    for (int i = 0; i < 40; i++) {
      if (plan[i].ukedag == currentDay) {
        int fagStartMinutes = plan[i].startTid.substring(0, 2).toInt() * 60 + plan[i].startTid.substring(3, 5).toInt();
        int fagEndMinutes = fagStartMinutes + plan[i].varighet;
        int currentMinutes = currentHour * 60 + currentMinute;

        if (currentMinutes >= fagStartMinutes && currentMinutes < fagEndMinutes) {
          currentfag = i;
          break;
        }
      }
    }

    if (currentfag != -1) {
      int fagStartMinutes = plan[currentfag].startTid.substring(0, 2).toInt() * 60 + plan[currentfag].startTid.substring(3, 5).toInt();
      int fagEndMinutes = fagStartMinutes + plan[currentfag].varighet;
      int currentMinutes = currentHour * 60 + currentMinute;
      int currentTotalSeconds = currentMinutes * 60 + currentSecond;
      int fagEndTotalSeconds = fagEndMinutes * 60;

      int remainingTime = fagEndTotalSeconds - currentTotalSeconds;

      return remainingTime;
      //Serial.print(remainingTime);
      //Serial.println(" sekunder igjen");
    } else {
      return 0;
      //Serial.println("Ingen fag");
    }
  }
  

//************************************************************
//************        NEDTELLING          ********************
//************************************************************



#endif