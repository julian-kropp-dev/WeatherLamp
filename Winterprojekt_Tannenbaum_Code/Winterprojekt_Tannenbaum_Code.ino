/*
  v1.0.8 - letzte Aenderung am 3. Februar 2021
*/

#include "FastLED.h"                      // Bibliothek einbinden, um LED ansteuern zu koennen

#include <SPI.h>                          // Bibliotheken einbinden, um das OLED Display ansteuern zu koennen
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ArduinoJson.h>                  // Bibliothek einbinden, um JSONs parsen zu koennen

#include <math.h>                         // Bibliothek einbinden, um Temperaturen runden zu koennen

#include <WiFiManager.h>                  // Bibliothek einbinden, um Uebergabe der WiFi Credentials ueber einen AP zu ermoeglichen
WiFiManager wifiManager;
WiFiClient client;

#define OLED_RESET 0                       // "0" fuer ESP8266
Adafruit_SSD1306 display(OLED_RESET);

#define LED_DATA_PIN D5                    // an welchem Pin liegt die LED an?

#define NUM_LEDS    8
//LED Array mit NUM_LEDS Elementen
CRGB leds[NUM_LEDS];
#define x 100 //delay Variable 
// ========================  hier deinen API-Key eintragen!!!  ============================================================================================================
const String city = "Oldenburg";
const String api_key = "xxx";    // dein Open-Weather-Map-API-Schluessel, kostenlos beziehbar ueber https://openweathermap.org/
// ========================================================================================================================================================================

int weatherID = 0;
int weatherID_shortened = 0;
String weatherforecast_shortened = " ";
int temperature_Celsius_Int = 0;
unsigned long lastcheck = 0;                               // Zeitpunkt des letzten Checks

void setup() {
  Serial.begin(115200);                               // fuer die Ausgabe des seriellen Monitors

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);        // das Display initialisieren, ...
  display.display();
  delay(2000);
  display.clearDisplay();                           // ... den Inhalt löschen ... und "Verbindungsversuch" anzeigen
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Verbinde");
  display.println("dich mit");
  display.println("der SSID ");
  display.println("Wemos_D1");
  display.setCursor(0, 0);
  display.display();

  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS); // LED instanziieren
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  leds[7] = CRGB::Red;                              // LED zu Beginn rot setzen, um sie zu testen
  FastLED.setBrightness(255);
  FastLED.show();

  // Falls es Probleme mit dem Portal gibt, kann man dieses auch über die IP 192.168.4.1 erreichen
  wifiManager.autoConnect("Wemos_D1");      // hier kann der Name des Hotspots deiner Lampe angepasst werden
  delay(2000);
  getCurrentWeatherConditions();                    // nach dem (Neu-)Start erstmalig das aktuelle Wetter laden
  updateDisplay();
}

void loop() {
  // folgende if-Bedingung sorgt dafuer, dass nur alle 30 Minuten (also 1.800.000 ms) das aktuelle Wetter abgefragt wird; dies spart Strom und Web-Traffic
  if (millis() - lastcheck >= 1800000) {            // millis() gibt die Zeit aus, die seit dem Start des Mikrocontrollers verstrichen ist
    getCurrentWeatherConditions();
    lastcheck = millis();                           // lastcheck auf aktuelle Systemzeit setzen
    updateDisplay();
  }

  // unten werden nun je nach Wetterbedingung (die in der Variablen "weatherID_shortened" steckt) eine Funktion aufgerufen, die die Lampe unterschiedlich leuchten laesst
  if (weatherID == 800) LED_effect_clearSky();                  // nur wenn die "weatherID" 800 ist, ist es klar/heiter/sonnig...
  else {
    switch (weatherID_shortened) {                              // sonst ist es je nach Hunderterbereich unterschiedlich: Werte hierfuer entstammen aus https://openweathermap.org/weather-conditions
      case 2: LED_effect_thunder(); break;    // "Gewitter"
      case 3: LED_effect_drizzle(); break;    // "Nieselregen"
      case 5: LED_effect_rain(); break;       // "Regen"
      case 6: LED_effect_snow(); break;       // "Schnee"
      case 7: LED_effect_fog(); break;        // "Nebel"
      case 8: LED_effect_cloudy(); break;     // "Wolken"
    }
  }

  // und die loop-Schleife beginnt nun wieder von vorne ... :)

}



// ========================================================================================================================================================================
/* es folgen Funktionen, die von der Lampe benoetigt werden*/


void updateDisplay() {                                                                    // Funktion zum Aktualisieren des Inhalts auf dem Display
  display.clearDisplay();
  display.stopscroll();
  // obere Zeile
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);                 // bei TextSize(1) funktioniert setCursor(32, 9) ganz gut
  display.println(weatherforecast_shortened);

  Serial.println(weatherforecast_shortened);
  Serial.println(weatherforecast_shortened.length());

  // untere Zeile
  if (weatherforecast_shortened.length() != 0) {                          // nur, wenn weatherforecast_shortened nicht leer ist (dann naemlich keine Server-Antwort)

    Serial.println(temperature_Celsius_Int);
    Serial.println(String(temperature_Celsius_Int, DEC));
    int digitsTemperature = String(temperature_Celsius_Int, DEC).length(); // wie lang (wie viele Ziffern) ist die Anzeige der Temperatur?
    display.setCursor(24 - 12 * digitsTemperature, 28);                   // bei textsize(2) ist eine Ziffer 12 Pixel breit; rechtsbuendig anzeigen, deswegen wird die x-Koord. des Cursors abhaengig davon gesetzt
    display.setTextSize(2);
    display.println(temperature_Celsius_Int);


    // Grad Celsius: C
    display.setCursor(30, 28);
    display.setTextSize(2);
    display.println("C");

    // Grad Celsius: Kreis
    display.drawCircle(28, 26, 2, WHITE);
  } else {
    display.stopscroll();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 16);
    display.println("keine");
    display.println("Server-");
    display.println("antwort...");
  }
  display.display();
}

void getCurrentWeatherConditions() {
  Serial.print("connecting to "); Serial.println("api.openweathermap.org");
  if (client.connect("api.openweathermap.org", 80)) {
    client.println("GET /data/2.5/weather?q=" + city + ",DE&units=metric&lang=de&APPID=" + api_key);
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("connection failed");
    display.stopscroll();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(10, 16);
    display.println("keine");
    display.setCursor(10, 28);
    display.println("Server-");
    display.setCursor(10, 40);
    display.println("antwort...");
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(14) + 360;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  client.stop();

  weatherID = doc["weather"][0]["id"];
  int temperature_Celsius = doc["main"]["temp"];
  temperature_Celsius_Int = (int)temperature_Celsius;

  weatherID_shortened = weatherID / 100;
  switch (weatherID_shortened) {                                                 // Werte hierfuer stammen aus https://openweathermap.org/weather-conditions
    case 2: weatherforecast_shortened = "Gewitter"; break;
    case 3: weatherforecast_shortened = "Nieselreg."; break;
    case 5: weatherforecast_shortened = "Regen"; break;
    case 6: weatherforecast_shortened = "Schnee"; break;
    case 7: weatherforecast_shortened = "Nebel"; break;
    case 8: weatherforecast_shortened = "Wolken"; break;
    default: weatherforecast_shortened = ""; break;                              // wenn kein anderer Wert passt (z.B. weil Server nicht antwortet), ist die weatherlage ungewiss
  } if (weatherID == 800) weatherforecast_shortened = "klar";                    // nur fuer den Fall, dass die weather-ID genau 800 ist, ist es "klar"
}


// ========================================================================================================================================================================
/*
  ###### #    # #####  ######
  #      #    # #    # #
  #####  #    # #    # #####
  #      #    # #####  #
  #      #    # #   #  #
  ######  ####  #    # ######
  #######
  #       ###### ###### ###### #    # ##### ######
  #       #      #      #      #   #    #   #
  #####   #####  #####  #####  ####     #   #####
  #       #      #      #      #  #     #   #
  #       #      #      #      #   #    #   #
  ####### #      #      ###### #    #   #   ######
  ###### # #    # ###### #    # ######  ####  ###### #    #
  #      # ##   # #      #    # #      #    # #      ##   #
  #####  # # #  # #####  #    # #####  #      #####  # #  #
  #      # #  # # #      #    # #      #  ### #      #  # #
  #      # #   ## #      #    # #      #    # #      #   ##
  ###### # #    # #       ####  ######  ####  ###### #    #
          ########
          ########
          ########
          ########
          ########
          ########
          ########
          ########
          ########
          ########
     ##################
       ##############
         ##########
           ######
             ##
*/

void LED_effect_clearSky() { // Effekt, der angezeigt wird, wenn der Himmel klar ist
  FastLED.setBrightness(255);
  leds[7] = CRGB::Yellow;
  FastLED.show();
  delay(500);
  leds[7] = CRGB::Black;
  FastLED.show();
  delay(500);
}


void LED_effect_thunder() {
  //LEDs nacheinander gelb schalten
 for (int i=0; i<8; i++){
    leds[i] = CRGB( 255, 255, 0);
    FastLED.show(); 
    delay(x);
  }

  

 //LEDs nacheinander ausschalten
 for (int i=0; i<8; i++){
   leds[i] = CRGB( 0, 0, 0);
   FastLED.show(); 
   delay(x);
  }

}

void LED_effect_drizzle() {

}

void LED_effect_rain() {

}

void LED_effect_snow() {

}

void LED_effect_fog() {

}

void LED_effect_cloudy() {

}
