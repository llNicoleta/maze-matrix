#include <FastLED.h>
#include "Maps.h"

#define LED_PIN 13
#define NUM_LEDS 256
#define BRIGHTNESS 10
#define WALL wall_1
#define WALL_SIZE wall_1_size
#define BUTTON_1 8    //go up
#define BUTTON_2 9    //go down
#define BUTTON_3 10   //go left
#define BUTTON_4 11   //go right

byte button_state = 0;

byte a[16][16];
int crt_pos = -1;
boolean generate = false;

CRGB leds[NUM_LEDS];
const byte debounce_delay = 100; //milliseconds to wait until stable

boolean debounce(int pin){

  boolean state;
  boolean previous_state;

  previous_state = digitalRead(pin);

  for(int cnt = 0; cnt < debounce_delay; cnt++){

    delay(1);
    state = digitalRead(pin);

    if(state != previous_state){

      cnt = 0;
      previous_state = state;
    }
  }

  return state;
}

void matrix_init(){

      byte cnt = 0;

      for(int j = 15; j >= 0; j--){

        if(j % 2 == 1){
            for(int i = 15; i >= 0; i--){
                a[i][j] = cnt;
                cnt++;
            }
        }else{

            for(int i = 0; i <= 15; i++){
                a[i][j] = cnt;
                cnt++;
            }
        }

    }

}

void generate_maze(const byte wall[], byte wall_size){

   for(int i = 0; i < wall_size; i++){
    leds[wall[i]] = CRGB(255, 0, 0);   
  }

  leds[wall[wall_size]] = CRGB(0, 255, 0);  //starting point
  leds[wall[wall_size+1]] = CRGB(255, 255, 255);  //finish

  crt_pos = wall[wall_size]; //current position = starting point
}

byte found_j(int pos){

  for(int i = 0; i < 16; i++)
    for(int j = 0; j < 16; j++){

      if(a[i][j] == pos){
        return j;
      }
    }
}

byte found_i(int pos){

  for(int i = 0; i < 16; i++)
    for(int j = 0; j < 16; j++){

      if(a[i][j] == pos){
        return i;
      }
    }
}

boolean is_wall(byte led, const byte wall[], byte wall_size){

  for(int i = 0; i < wall_size; i++){
    if(wall[i] == led){
      return true;
    }
  }

  return false;
}

void advance_up(const byte wall[], byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(!is_wall(a[i-1][j], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print("in up: ");
        // Serial.println(a[i-1][j]);
        

        leds[crt_pos] = CRGB::Black;
        leds[a[i-1][j]] = CRGB(0, 255, 0);
        crt_pos = a[i-1][j];

        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
            leds[wall[wall_size]] = CRGB(255, 0, 0);

        FastLED.show();

  }

}

void advance_down(const byte wall[], byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(!is_wall(a[i+1][j], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print("in down: ");
        // Serial.println(a[i+1][j]);
        

        leds[crt_pos] = CRGB::Black;
        leds[a[i+1][j]] = CRGB(0, 255, 0);
        crt_pos = a[i+1][j];
        
        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
          leds[wall[wall_size]] = CRGB(255, 0, 0);
        
        FastLED.show();

  }

}

void advance_left(const byte wall[], byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(!is_wall(a[i][j-1], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print("in left: ");
        // Serial.println(a[i][j-1]);
        
        leds[crt_pos] = CRGB::Black;
        leds[a[i][j-1]] = CRGB(0, 255, 0);
        crt_pos = a[i][j-1];

        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
          leds[wall[wall_size]] = CRGB(255, 0, 0);

        FastLED.show();

  }

}

void advance_right(const byte wall[], byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(!is_wall(a[i][j+1], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print("in right: ");
        // Serial.println(a[i][j+1]);
        

        leds[crt_pos] = CRGB::Black;
        leds[a[i][j+1]] = CRGB(0, 255, 0);
        crt_pos = a[i][j+1];

        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
          leds[wall[wall_size]] = CRGB(255, 0, 0);

        FastLED.show();

  }

}

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  pinMode(BUTTON_4, INPUT);

  
  matrix_init();

  Serial.begin(9600);
}

void loop() {
  
  if(!generate){
    generate_maze(WALL, WALL_SIZE);
    generate = true;
    FastLED.show();
    
    //Serial.println(a[0][0]);
    //Serial.println(is_wall(29, wall_3, wall_3_size));
  }

  if(debounce(BUTTON_1)){

    advance_up(WALL, WALL_SIZE);
  }

  if(debounce(BUTTON_2)){

    advance_down(WALL, WALL_SIZE);
  }

  if(debounce(BUTTON_3)){

    advance_right(WALL, WALL_SIZE);
  }

  if(debounce(BUTTON_4)){

    advance_left(WALL, WALL_SIZE);
  }

}
