#include <FastLED.h>

#define DATA_PIN D5
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

const int x = 80;
const int y = 150;
const int BRIGHTNESS = 80;
    
void setup()
{
  Serial.begin(9600);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // max 400mA ziehen
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 400);
}

//Wird unendlich oft abgespielt
void loop() {
  greenfading();
  greenflashlight();

}

void greenfading()
{
  // LEDs nacheinander grün schalten
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 255, 0);
    FastLED.show();
    delay(x);
  }

  // wenn ganzes LED Band grün, dann 2 Sekunden warten
  delay(2000);

  // LEDs nacheinander ausschalten
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(x);
    
  }
}

void greenflashlight()
{
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 255, 0);
    FastLED.show();
  }

  // wenn ganzes LED Band grün, dann 2 Sekunden warten
  delay(100);

  // LEDs nacheinander ausschalten
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
  }
}

void bluefading()
{
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 0, 128);
    FastLED.show();
    delay(x);
  }
  delay(y);
  for (int i = 0; i > 8; i--)
  {
    leds[i] = CRGB(0, 0, 255);
    FastLED.show();
    delay(x);
  }
  delay(y);
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 191, 255);
    FastLED.show();
    delay(x);
  }
  delay(y);
  for (int i = 0; i > 8; i--)
  {
    leds[i] = CRGB(255, 182, 193);
    FastLED.show();
    delay(x);
  }
  delay(y);
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(255, 105, 180);
    FastLED.show();
    delay(x);
  }
  delay(y);
}
