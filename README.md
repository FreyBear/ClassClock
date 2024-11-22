# ClassClock
En adresserbar LED-ramme til en analog klokke som viser hvor lenge det er igjen av friminutt og timer.
Koden er utviklet for en ESP32 med følgende bibliotek:
1. [WiFi.h]
2. [time.h]
3. [Adafruit_NeoPixel.h]
4. [Wire.h]
5. [RTClib]

Alle timene fylles inn i en liste med følgende struct.

```cpp
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
```

## Credits:

Klasse 1ELER på Godalen VGS

 - [Ingve (Kontaktlærer)]
 - [Hans Kristian (hardware til prosjektet)]
  
  
```cpp
// EKSEMPEL
```
