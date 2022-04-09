#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 16

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);

}

void loop() {

  FastLED.clear();
  FastLED.show();
  // put your main code here, to run repeatedly:

}
