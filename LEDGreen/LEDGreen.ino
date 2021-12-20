#include "FastLED.h"
#define x 100

#define DATA_PIN    D5
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    8

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          80

void setup() {
  Serial.begin(9600);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  //max 400mA ziehen
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 400);


}

//Wird unendlich oft abgespielt
void loop() {
  int y = random(0,3);
  if (y == 0) greenfading();
  if (y == 1) greenflashlight();
  else singleflashlight();
}

void greenfading() {
  //LEDs nacheinander grün schalten
 for (int i=0; i<8; i++){
    leds[i] = CRGB( 0, 255, 0);
    FastLED.show(); 
    delay(x);
  }

  // wenn ganzes LED Band grün, dann 2 Sekunden warten
 delay(2000);

 //LEDs nacheinander ausschalten
 for (int i=0; i<8; i++){
   leds[i] = CRGB( 0, 0, 0);
   FastLED.show(); 
   delay(x);
  }
  
}

void greenflashlight() {
  for (int i=0; i<8; i++){
    leds[i] = CRGB( 0, 255, 0);
    FastLED.show(); 
  }
  
  // wenn ganzes LED Band grün, dann 2 Sekunden warten
 delay(100);

 //LEDs nacheinander ausschalten
 for (int i=0; i<8; i++){
   leds[i] = CRGB( 0, 0, 0);
   FastLED.show(); 
  }
}

void singleflashlight() {
  for (int i=0; i<8; i++){
    leds[i] = CRGB( 0, 255, 0);
    FastLED.show(); 
    delay(50);
    leds[i] = CRGB( 0, 0, 0);
    FastLED.show();
    delay(50); 
  }
  
  
}
