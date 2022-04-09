//maze3

#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 16

CRGB leds[NUM_LEDS];

int wall[] = {240,239,208,207,176,175,144,143,112,111,121,80,79,48,47,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,23,29,45,44,42,40,38,36,34,32,51,53,55,57,59,61,63,
77,76,74,72,70,68,66,64,82,83,85,87,89,91,95,109,106,100,99,98,96,113,114,117,118,119,120,127,134,133,132,129,128,145,146,147,148,149,150,151,153,154,155,156,157,
158,159,168,166,160,177,178,179,180,181,182,183,185,186,187,188,189,190,191,200,198,193,192,210,211,213,218,220,222,223,234,231,227,225,224,242,243,244,245,246,
247,248,249,250,251,252,253,254,255};

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
}

void loop() {

  int wall_size = sizeof(wall)/sizeof(wall[0]);

  Serial.println(wall_size);
  
  for(int i = 0; i < wall_size; i++){
    leds[wall[i]] = CRGB(255, 0, 0);   
  }

  leds[31] = CRGB(0, 255, 0); //starting point
  leds[241] = CRGB(255, 255, 255);  //finish

  FastLED.show();

  
 

}
