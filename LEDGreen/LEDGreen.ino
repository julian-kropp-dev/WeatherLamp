//OLED Display
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_RESET 0
#define OLED_WIDTH 64
#define OLED_HEIGHT 48
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(OLED_RESET);
//LED
#include <FastLED.h>
#define PAUSE 1000
#define DATA_PIN D5
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

const int x = 100;
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

  //Display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  display.setTextSize(0.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Wetter fur Oldenburg");

  display.setTextSize(0.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 24);
  display.println("Temperatur");
  display.println("-3 Grad");
  

  display.display();
  
}

//Wird unendlich oft abgespielt
void loop() {
  greenfading();
 
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

void singleflashlight()
{
  for (int i = 0; i < 8; i++)
  {
    leds[i] = CRGB(0, 255, 0);
    FastLED.show();
    delay(50);
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(50);
  }
}
