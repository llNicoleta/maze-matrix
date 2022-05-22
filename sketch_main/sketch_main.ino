#include <FastLED.h>
#include "Maps.h"

#define LED_PIN 13
#define NUM_LEDS 256
#define BRIGHTNESS 10
// #define WALL wall_3
// #define WALL_SIZE wall_3_size
#define BUTTON_1 8    //go up
#define BUTTON_2 9    //go down
#define BUTTON_3 10   //go right
#define BUTTON_4 11   //go left

byte pc_pos;
byte pc_can_move = 0;

byte button_state = 0;

byte a[16][16];
int crt_pos = -1;
boolean generate = false;
boolean win = false;
boolean lost = false;

byte selected = 0;

CRGB leds[NUM_LEDS];
const byte debounce_delay = 90; //milliseconds to wait until stable

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

void generate_maze(const byte wall[], const byte wall_size){

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

boolean is_wall(byte led, const byte wall[], const byte wall_size){

  for(int i = 0; i < wall_size; i++){
    if(wall[i] == led){
      return true;
    }
  }

  return false;
}

boolean is_finish(byte led, const byte wall[], const byte wall_size){

  if(led == wall[wall_size+1]){
    return true;
  }

  return false;

}

boolean is_pc_pos(byte led){  //next move

  if(led == pc_pos){
    return true;
  }

  return false;
}

boolean is_pc_over_crt(byte led){ //next move

  if(led == crt_pos){
    return true;
  }

  return false;
}

void advance_up(const byte wall[], const byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(is_pc_pos(a[i-1][j])){ //pc_pos can't be a wall
    lost = true;
    return;
  }

  if(!is_wall(a[i-1][j], wall, wall_size + 1))  //starting point included
  {     
        // debugging session:
        // Serial.print(F("in up: "));
        // Serial.println(a[i-1][j]);
        

        leds[crt_pos] = CRGB::Black;
        leds[a[i-1][j]] = CRGB(0, 255, 0);
        crt_pos = a[i-1][j];

        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
            leds[wall[wall_size]] = CRGB(255, 0, 0);

        FastLED.show();

        if(is_finish(crt_pos, wall, wall_size)){
            //Serial.println("win");
            win = true;
            return;
        }

  }

}

void advance_down(const byte wall[], const byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(is_pc_pos(a[i+1][j])){
    lost = true;
    return;
  }

  if(!is_wall(a[i+1][j], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print(F("in down: "));
        // Serial.println(a[i+1][j]);
        

        leds[crt_pos] = CRGB::Black;
        leds[a[i+1][j]] = CRGB(0, 255, 0);
        crt_pos = a[i+1][j];
        
        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
          leds[wall[wall_size]] = CRGB(255, 0, 0);
        
        FastLED.show();

        if(is_finish(crt_pos, wall, wall_size)){
            //Serial.println("win");
            win = true;
            return; 
        }

  }

}

void advance_left(const byte wall[], const byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(is_pc_pos(a[i][j-1])){
    lost = true;
    return;
  }

  if(!is_wall(a[i][j-1], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print(F("in left: "));
        // Serial.println(a[i][j-1]);
        
        leds[crt_pos] = CRGB::Black;
        leds[a[i][j-1]] = CRGB(0, 255, 0);
        crt_pos = a[i][j-1];

        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
          leds[wall[wall_size]] = CRGB(255, 0, 0);

        FastLED.show();

        if(is_finish(crt_pos, wall, wall_size)){
            //Serial.println("win");
            win = true;
            return;
        }

  }

}

void advance_right(const byte wall[], const byte wall_size){

  byte i = found_i(crt_pos);
  byte j = found_j(crt_pos);

  if(is_pc_pos(a[i][j+1])){
    lost = true;
    return;
  }

  if(!is_wall(a[i][j+1], wall, wall_size + 1))
  {     
        // debugging session:
        // Serial.print(F("in right: "));
        // Serial.println(a[i][j+1]);
        

        leds[crt_pos] = CRGB::Black;
        leds[a[i][j+1]] = CRGB(0, 255, 0);
        crt_pos = a[i][j+1];

        if(leds[wall[wall_size]] != CRGB(255, 0, 0))
          leds[wall[wall_size]] = CRGB(255, 0, 0);

        FastLED.show();

        if(is_finish(crt_pos, wall, wall_size)){
            //Serial.println("win");
            win = true;
            return;
        }

  }

}

void display_win(){

  FastLED.clear();

  for(int i = 240; i <= 255; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 218; i <= 220; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 176; i <= 191; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 146; i <= 159; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 112; i <= 127; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 96; i <= 111; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 16; i <= 31; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  for(int i = 0; i <= 15; i++){
    leds[i] = CRGB(255, 0, 0);
  }

  leds[83] = CRGB(255, 0, 0);
  leds[84] = CRGB(255, 0, 0);
  leds[74] = CRGB(255, 0, 0);
  leds[75] = CRGB(255, 0, 0);
  leds[54] = CRGB(255, 0, 0);
  leds[53] = CRGB(255, 0, 0);
  leds[40] = CRGB(255, 0, 0);
  leds[41] = CRGB(255, 0, 0);

  leds[225] = CRGB(255, 0, 0);
  leds[226] = CRGB(255, 0, 0);

  leds[193] = CRGB(255, 0, 0);
  leds[194] = CRGB(255, 0, 0);

  leds[144] = CRGB(255, 0, 0);

  FastLED.show();

}

void load_map(byte selected){

   switch(selected){

      case 1:
        generate_maze(wall_1, wall_1_size);
        break;

      case 2:
        generate_maze(wall_2, wall_2_size);
        break;
      
      case 3:
        generate_maze(wall_3, wall_3_size);
        break;
      
    }
}

void advance_select(byte button, byte selected){

  if(button == 1){  //1 for up

    switch(selected){

      case 1:
        advance_up(wall_1, wall_1_size);
        break;

      case 2:
        advance_up(wall_2, wall_2_size);
        break;
      
      case 3:
        advance_up(wall_3, wall_3_size);
        break;
    }

      
  }else
  if(button == 2){  //2 for down

    switch(selected){

      case 1:
        advance_down(wall_1, wall_1_size);
        break;

      case 2:
        advance_down(wall_2, wall_2_size);
        break;
      
      case 3:
        advance_down(wall_3, wall_3_size);
        break;
      }
      
    }else
    if(button == 3){  //3 for right

      switch(selected){

      case 1:
        advance_right(wall_1, wall_1_size);
        break;

      case 2:
        advance_right(wall_2, wall_2_size);
        break;
      
      case 3:
        advance_right(wall_3, wall_3_size);
        break;
      
      }
    
    }else
    if(button == 4){ //4 for left

      switch(selected){

      case 1:
        advance_left(wall_1, wall_1_size);
        break;

      case 2:
        advance_left(wall_2, wall_2_size);
        break;
      
      case 3:
        advance_left(wall_3, wall_3_size);
        break;
      
      }

    }

}

void pc_init(byte selected){

  randomSeed(millis()); //1193 hours available

  for(;;){

    pc_pos = random(256);

    // debugging session:
    // Serial.println(F("iteration:"));
    // Serial.println(pc_pos);
    // Serial.println(is_wall(pc_pos, wall_1, wall_1_size));
    // Serial.println(is_wall(pc_pos, wall_2, wall_2_size));
    // Serial.println(is_wall(pc_pos, wall_3, wall_3_size));
    // Serial.println("-----------\n");

    if(selected == 1){
      
      if(is_wall(pc_pos, wall_1, wall_1_size + 2)){
        continue;               //starting + finish point included
      }else{
        break;
      }
    }
    else
    if(selected == 2){
      
      if(is_wall(pc_pos, wall_2, wall_2_size + 2)){
        continue;
      }else{
        break;
      }
    }
    else
    if(selected == 3){
      
      if(is_wall(pc_pos, wall_3, wall_3_size + 2)){
        continue;
      }else{
        break;
      }
    }


  }

  leds[pc_pos] = CRGB(0, 255, 255);

  Serial.print(F("pc_pos: "));
  Serial.println(pc_pos);
}

void pc_advance_up(const byte wall[], const byte wall_size){

  byte i = found_i(pc_pos);
  byte j = found_j(pc_pos);

  Serial.print(F("selected: "));
  Serial.println(selected);
  Serial.print(F("wall_size: "));
  Serial.println(wall_size);

  Serial.println(F("pc advance_up to: "));
  Serial.print(a[i-1][j]);
  Serial.print(F("-"));
  Serial.println(is_wall(a[i-1][j], wall, wall_size + 2));
  Serial.println(F("--------------\n"));

  if(is_pc_over_crt(a[i-1][j])){  //crt_pos can't be a wall
    lost = true;
    return;
  }


  if(!is_wall(a[i-1][j], wall, wall_size + 2))  
  {                                 //starting + finish point included
        // debugging session:
        // Serial.print("in up: ");
        // Serial.println(a[i-1][j]);
        

        leds[pc_pos] = CRGB::Black;
        leds[a[i-1][j]] = CRGB(0, 255, 255);
        pc_pos = a[i-1][j];

        FastLED.show();

        pc_can_move = 1;
  }


}

void pc_advance_down(const byte wall[], const byte wall_size){

  byte i = found_i(pc_pos);
  byte j = found_j(pc_pos);

  Serial.print(F("selected: "));
  Serial.println(selected);
  Serial.print(F("wall_size: "));
  Serial.println(wall_size);

  Serial.println(F("pc advance_down to:"));
  Serial.print(a[i+1][j]);
  Serial.print(F("-"));
  Serial.println(is_wall(a[i+1][j], wall, wall_size + 2));
  Serial.println(F("--------------\n"));

  if(is_pc_over_crt(a[i+1][j])){
    lost = true;
    return;
  }

  if(!is_wall(a[i+1][j], wall, wall_size + 2))  
  {                                 //starting + finish point included
        // debugging session:
        // Serial.print("in up: ");
        // Serial.println(a[i-1][j]);
        

        leds[pc_pos] = CRGB::Black;
        leds[a[i+1][j]] = CRGB(0, 255, 255);
        pc_pos = a[i+1][j];

        FastLED.show();

        pc_can_move = 1;

  }

}

void pc_advance_left(const byte wall[], const byte wall_size){

  byte i = found_i(pc_pos);
  byte j = found_j(pc_pos);

  Serial.print(F("selected: "));
  Serial.println(selected);
  Serial.print(F("wall_size: "));
  Serial.println(wall_size);

  Serial.println(F("pc advance_left to: "));
  Serial.print(a[i][j-1]);
  Serial.print(F("-"));
  Serial.println(is_wall(a[i][j-1], wall, wall_size + 2));
  Serial.println(F("--------------\n"));

  if(is_pc_over_crt(a[i][j-1])){
    lost = true;
    return;
  }

  if(!is_wall(a[i][j-1], wall, wall_size + 2))  
  {                                 //starting + finish point included
        // debugging session:
        // Serial.print("in up: ");
        // Serial.println(a[i-1][j]);
        

        leds[pc_pos] = CRGB::Black;
        leds[a[i][j-1]] = CRGB(0, 255, 255);
        pc_pos = a[i][j-1];

        FastLED.show();

        pc_can_move = 1;
        
  }


}

void pc_advance_right(const byte wall[], const byte wall_size){

  byte i = found_i(pc_pos);
  byte j = found_j(pc_pos);

  Serial.print(F("selected: "));
  Serial.println(selected);
  Serial.print(F("wall_size: "));
  Serial.println(wall_size);

  Serial.println(F("pc advance_right to: "));
  Serial.print(a[i][j+1]);
  Serial.print(F("-"));
  Serial.println(is_wall(a[i][j+1], wall, wall_size + 2));
  Serial.println(F("--------------\n"));

  if(is_pc_over_crt(a[i][j+1])){
    lost = true;
    return;
  }

  if(!is_wall(a[i][j+1], wall, wall_size + 2))  
  {                                 //starting + finish point included
        // debugging session:
        // Serial.print("in up: ");
        // Serial.println(a[i-1][j]);
        

        leds[pc_pos] = CRGB::Black;
        leds[a[i][j+1]] = CRGB(0, 255, 255);
        pc_pos = a[i][j+1];

        FastLED.show();

        pc_can_move = 1;

  }

}

void pc_advance(byte selected){

  //usage of: 
  //char pc_moves[4][6] = {"up", "down", "right", "left"};

  randomSeed(millis()); //1193 hours available

  for(;;){    //one advance per iteration

  byte move = random(4); 
  //0 for up, 1 for down, 2 for right, 3 for left

  // Serial.print("move: ");
  // Serial.println(move);
  // Serial.println("----------\n");

  if(move == 0){  //up


    switch(selected){

      case 1:
        pc_advance_up(wall_1, wall_1_size);
        break;

      case 2:
        pc_advance_up(wall_2, wall_2_size);
        break;

      case 3:
        pc_advance_up(wall_3, wall_3_size);
        break;

    }

  }else
  if(move == 1){  //down

      switch(selected){

      case 1:
        pc_advance_down(wall_1, wall_1_size);
        break;

      case 2:
        pc_advance_down(wall_2, wall_2_size);
        break;

      case 3:
        pc_advance_down(wall_3, wall_3_size);
        break;

    }

  }else
  if(move == 2){ //right

      switch(selected){

      case 1:
        pc_advance_right(wall_1, wall_1_size);
        break;

      case 2:
        pc_advance_right(wall_2, wall_2_size);
        break;

      case 3:
        pc_advance_right(wall_3, wall_3_size);
        break;
    }

  }else
  if(move == 3){  //left

      switch(selected){

      case 1:
        pc_advance_left(wall_1, wall_1_size);
        break;

      case 2:
        pc_advance_left(wall_2, wall_2_size);
        break;

      case 3:
        pc_advance_left(wall_3, wall_3_size);
        break;
    }


  }

      if(pc_can_move == 1){
      
        pc_can_move = 0;
        Serial.println(F("pc can move\n\n\n"));
        break;
      }else{
        continue;
      }

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

  FastLED.clear();
  FastLED.show();

  Serial.println(F("welcome"));
  Serial.println(wall_1_size);
  Serial.println(wall_2_size);
  Serial.println(wall_3_size);
  Serial.println(F("--------------\n\n\n"));

}

void loop() {
  
  //display_win();

  if(!selected){

      if(debounce(BUTTON_4)){

        selected = 1;
        //Serial.println("button 4 - first map");
      }

      if(debounce(BUTTON_3)){

        selected = 2;
        //Serial.println("button 3 - second map");
      }

      if(debounce(BUTTON_2)){

        selected = 3;
        //Serial.println("button 2 - third map");
      }

  }

  if(!generate && selected){

    //Serial.println("welcome"); //serial port test

    load_map(selected); //generate-maze - no FastLED.show()   
    generate = true;
    
    pc_init(selected);


    FastLED.show();

  }


  if(!win && generate && selected){

      if(debounce(BUTTON_1)){
        
        //Serial.println("up button");
        advance_select(1, selected);
        pc_advance(selected);

      }

      if(debounce(BUTTON_2)){
        
        //Serial.println("down button");
        advance_select(2, selected);
        pc_advance(selected);

      }

      if(debounce(BUTTON_3)){
        
        //Serial.println("right button");
        advance_select(3, selected);
        pc_advance(selected);

      }

      if(debounce(BUTTON_4)){
        
        //Serial.println("left button");
        advance_select(4, selected);
        pc_advance(selected);
      }
  
  }

  if(win && generate && selected){
    

    selected = false;
    generate = false;
    win = false;

    display_win();
    delay(4000);

    FastLED.clear();
    FastLED.show();
  }

  if(lost){

    generate = false;
    selected = false;
    lost = false;

    delay(500);

    FastLED.clear();
    FastLED.show();
    Serial.println(F("you lost"));
  }

}
