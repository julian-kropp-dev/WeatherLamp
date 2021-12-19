#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FastLED.h"

#define FASTLED_ALLOW_INTERRUPTS 0
#define x 100



// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define DATA_PIN    D5
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          80
#define FRAMES_PER_SECOND  120



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