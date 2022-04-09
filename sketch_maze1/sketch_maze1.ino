//maze1

#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 16

CRGB leds[NUM_LEDS];

int wall[] = {240, 239, 207, 176, 175, 144, 143, 112, 111, 80, 79, 48, 47, 16, 15, 238, 206, 242, 205, 178, 146, 141, 109, 82, 77, 50, 18, 13,
              204, 179, 115, 76, 235, 180, 148, 75, 11, 234,  117, 53, 21, 182, 137, 105, 200, 168, 87, 55, 40, 8, 248, 199, 167, 88, 56,
              230, 166, 121, 57, 38, 165, 133, 122, 90, 58, 228, 187, 164, 132, 91, 36, 220, 131, 99, 92, 67, 35, 3, 226, 130, 66, 190, 161, 158,
              129, 65, 255, 224, 223, 192, 191, 160, 159, 128, 127, 96, 64, 63, 32, 31, 0, 241, 243, 244, 245, 246, 247, 249, 250, 251, 252, 253, 254, 
              14, 12, 10, 9, 7, 6, 5, 4, 2, 1, 201, 169};

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

  leds[95] = CRGB(0, 255, 0); //starting point
  leds[208] = CRGB(255, 255, 255); //finish

  FastLED.show();

  
 

}
