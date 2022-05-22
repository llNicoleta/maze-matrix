#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 16

int button_state = 0;
int button_pin = 2;
int led_pin = 8;

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:

  pinMode(button_pin, INPUT);
  pinMode(led_pin, OUTPUT);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);

}

void loop() {



   FastLED.clear(true);
   FastLED.show();

}
