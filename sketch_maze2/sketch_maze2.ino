//maze2

#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 16

CRGB leds[NUM_LEDS];

int wall[] = {240,241,239,208,207,175,144,143, 112, 111,80, 79, 48, 47, 16, 15,14,12,11,10,9,8,7,6,5,4,3,1,0,2,17,20,30,31,46,45,43,40,39,38,37,35,33,
32,49,50,54,58,62,63,78,77,75,73,71,69,64,66,81,82,84,86,88,90,92,95,107,103,101,96,98,114,115,116,117,118,119,122,126,127,141,140,139,138,137,136,133,
132,128,147,151,153,154,155,159,174,172,170,168,165,162,160,164,206,176,177,179,181,184,187,190,191,201,192,210,213,216,218,219,222,223,236,227,225,224,242,243,
244,245,246,247,248,249,250,251,252,254,255};

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

  leds[253] = CRGB(0, 255, 0); //starting point
  leds[13] = CRGB(255, 255, 255);  //finish

  FastLED.show();

  
 

}
