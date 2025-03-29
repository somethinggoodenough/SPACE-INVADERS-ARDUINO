#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
//#include "pitches.h"


// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;


// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;


// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;
const int INVADER_DELAY = 3000;
const int BOSS_DELAY = 3000;
const int MAT_HEIGHT = 16;
const int CANNON_BALL_DELAY = 50;
const int MAT_WIDTH = 32;
const int strengths[4][16]
= {{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
  {1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2},
  {1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1},
  {5,4,5,4,5,4,5,4,2,3,2,3,2,3,2,3}};


// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);


// the following functions are for printing messages
void Welcome_Screen1();
void Welcome_Screen2();
void print_level(int level);
void print_lives(int lives);
void game_over();


class Color {
 public:
   int red;
   int green;
   int blue;
   Color() {
     red = 0;
     green = 0;
     blue = 0;
   }
   Color(int r, int g, int b) {
     red = r;
     green = g;
     blue = b;
   }
   uint16_t to_333() const {
     return matrix.Color333(red, green, blue);
   }
};


const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);


class Invader {
public:
  // Constructors
  Invader() {
    x = 0;
    y = 0;
    strength = 0;
    time = millis();
  }
  // sets values for private data members x and y
  Invader(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
    time = millis();
  }
  // sets values for private data members
  Invader(int x_arg, int y_arg, int strength_arg) {
    x = x_arg;
    y = y_arg;
    strength = strength_arg;
    time = millis();
  }
  // sets values for private data members
  void initialize(int x_arg, int y_arg, int strength_arg) {
    x = x_arg;
    y = y_arg;
    strength = strength_arg;
    time = millis();
  }


   // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  int get_strength() const {
    return strength;
  }

  void easy_move() {
   // Time records the time after the Invader moves. If millis() - time is larger than the time that the Invader stops, the Invader will move.
    if(strength > 0 && (millis() - time) > INVADER_DELAY){
       erase();
       y++;
       // Reach the bottom
       if(y >= (MAT_HEIGHT - 4)){ 
         reached_bottom = true;
       }
       // Doesn’t reach the bottom
       else{
         draw();
       }
       // Reset time
       time = millis();
     }
  }

  void luna_move() {
   // Time records the time after the Invader moves. If millis() - time is larger than the time that the Invader stops, the Invader will move.
    if(strength > 0 && (millis() - time) > INVADER_DELAY){
       erase();
       y++;
       x++;
       // Reach the bottom
       if(y >= (MAT_HEIGHT - 4)){ 
         reached_bottom = true;
       }
       else if(x>=MAT_WIDTH){
        x=0;
        draw();
       }
       // Doesn’t reach the bottom
       else{
         draw();
       }
       // Reset time
       time = millis();
     }
  }

  // Moves the Invader down the screen by one row
  // Modifies: y
  void move() {
   // Time records the time after the Invader moves. If millis() - time is larger than the time that the Invader stops, the Invader will move.
    if(strength > 0 && (millis() - time) > INVADER_DELAY){
       erase();
       y++;
       x++;
       // Reach the bottom
       if(y >= (MAT_HEIGHT - 4)){ 
         reached_bottom = true;
       }
       else if(x>=MAT_WIDTH){
        x=0;
        draw();
       }
       // Doesn’t reach the bottom
       else{
         draw();
       }
       // Reset time
       time = millis();
     }
  }


  bool has_been_hit(int x_arg, int y_arg) {
     if (get_strength() > 0) {
       bool _x = (x_arg == (x + 1) || x_arg == (x + 2) || x_arg == (x) || x_arg == (x + 3));
       bool _y = y_arg == (y + 1);
      
       if (_x && _y) {
         return true;
       }
     }
     return false;
   }


   bool has_touched_player(int x_arg, int y_arg){
     if (get_strength() > 0) {
       //bool _cannon = ((x <= x_arg) && (x_arg <= (x + 3)) && ((y_arg  == (y + 2))));
       //bool _overlap = false;
       //for(int i = x; i <= (x + 3); i++){
         //for(int j = (x_arg - 1); j <= (x_arg + 1); j++){
          // _overlap = (i == j);
         //}
       //}

      // bool _body = _overlap && (y_arg  == (y + 2));
       if ((x_arg == (x + 1)) || (x_arg == (x + 2))) {
          if (y_arg == y + 2) {
            return true;
          }
       }
       else if ((x_arg == x) || (x_arg == (x + 3))) {
         if (y_arg == y + 3) {
          return true;
         }
       }
     }
     return false;
   }


   bool has_reached_bottom(){
     return reached_bottom;
   }


   void clear_reached_bottom(){
     reached_bottom = false;
   }
   // setters
   void set_pos(int x_arg, int y_arg){
     x = x_arg;
     y = y_arg;
   }
   void set_strength(int s){
     strength = s;
     time = millis();
   }
  // draws the Invader if its strength is greater than 0
  // calls: draw_with_rgb
  void draw() {
      if (strength == 1) {
        draw_with_rgb(RED, BLUE);
      }




      if (strength == 2) {
        draw_with_rgb(ORANGE, BLUE);
      }




      if (strength == 3) {
        draw_with_rgb(YELLOW, BLUE);
      }




      if (strength == 4) {
        draw_with_rgb(GREEN, BLUE);
      }




      if (strength == 5) {
        draw_with_rgb(BLUE, BLUE);
      }




      if (strength == 6) {
        draw_with_rgb(PURPLE, BLUE);
      }




      if (strength == 7) {
        draw_with_rgb(WHITE, BLUE);
      }
  }
   // draws black where the Invader used to be
  // calls: draw_with_rgb
  void erase() {
    draw_with_rgb(BLACK, BLACK);
  }  
   // Invader is hit by a Cannonball.
  // Modifies: strength
  // calls: draw, erase
  void hit() {
     strength--;
     if(strength <= 0){
       erase();
     }
     else{
       draw();
     }
     //music
    // tone(13, NOTE_D8, 300);
  }




private:
  int x;
  int y;
  int strength;
  unsigned long time;
  bool reached_bottom;
   // draws the Invader
  void draw_with_rgb(Color body_color, Color eye_color) {
    // body color
    matrix.drawPixel(x + 1, y, body_color.to_333());
    matrix.drawPixel(x + 2, y, body_color.to_333());
    matrix.drawPixel(x, y + 1, body_color.to_333());
    matrix.drawPixel(x + 3, y + 1, body_color.to_333());
    matrix.drawPixel(x, y + 2, body_color.to_333());
    matrix.drawPixel(x + 1, y + 2, body_color.to_333());
    matrix.drawPixel(x + 2, y + 2, body_color.to_333());
    matrix.drawPixel(x + 3, y + 2, body_color.to_333());
    matrix.drawPixel(x, y + 3, body_color.to_333());
    matrix.drawPixel(x + 3, y + 3, body_color.to_333());




    // eye color
    matrix.drawPixel(x + 1, y + 1, eye_color.to_333());
    matrix.drawPixel(x + 2, y + 1, eye_color.to_333());
  }
};

class Boss {
public:

  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  // Constructors
  Boss() {
    // Initialize default values
    x = 0;
    y = 0;
    body_strength = 0;
    eye_strength = 0;
    wing_strength = 0;
    time = millis();
    reached_bottom = false;
  }

  // Constructors with parameters for customization
  Boss(int x_arg, int y_arg, int body_strength_arg, int eye_strength_arg, int wing_strength_arg) {
    x = x_arg;
    y = y_arg;
    body_strength = body_strength_arg;
    eye_strength = eye_strength_arg;
    wing_strength = wing_strength_arg;
    time = millis();
    reached_bottom = false;
  }

  // Getters and setters for position and strengths
  int get_x() const { 
    return x; 
  }
  int get_y() const { 
    return y; 
  }
  int get_body_strength() const { 
    return body_strength; 
  }
  int get_eye_strength() const { 
    return eye_strength; 
  }
  int get_wing_strength() const { 
    return wing_strength; 
  }

  void set_x(int x_arg) { 
    x = x_arg; 
  }
  void set_y(int y_arg) { 
    y = y_arg; 
  }
  void set_body_strength(int body_strength_arg) { 
    body_strength = body_strength_arg; 
  }
  void set_eye_strength(int eye_strength_arg) { 
    eye_strength = eye_strength_arg; 
  }
  void set_wing_strength(int wing_strength_arg) { 
    wing_strength = wing_strength_arg;
  }
  
  // Moves the Boss down the screen by one row
  void move() {
    // Check if the boss still has strength and time delay has passed
    if ((body_strength > 0 || wing_strength > 0 || eye_strength > 0 ) && (millis() - time) > BOSS_DELAY) {
        erase(); // Erase the boss from the current position

        // Move the boss down by incrementing the y coordinate
        y++;
        // Check if the boss has reached the bottom
        if (y >= (MAT_HEIGHT - 6)) {
            reached_bottom = true;
        }
        // If the boss hasn't reached the bottom, redraw it in the new position
        else {
            draw(); // Redraw the boss in the new position
        }

        // Reset the time for the next movement
        time = millis();
    }
  }


  // Check if the Boss has been hit by a projectile
  bool has_been_hit(int x_arg, int y_arg) {
    // Check if the boss parts have strength left
    bool hit_body = (((x_arg == x + 3) || (x_arg == x + 4) || (x_arg == x + 7) || (x_arg == x + 8)) && (y_arg == y + 5)) || (((x_arg == x + 5) || (x_arg == x + 6)) && (y_arg == y + 4));
    bool hit_eye = ((x_arg >= (x + 5) && x_arg <= (x + 6)) && (y_arg == y));
    bool hit_wing =   (((x_arg >= (x) && x_arg <= (x + 2)) || (x_arg >= (x + 9) && 
                      x_arg <= (x + 11))) && (y_arg >= (y) && y_arg <= (y + 2)));
    
    if (body_strength > 0) {
      num1 = 0;
      if (hit_body) {
          num1 = num1 + 1;
          return true;
      }
    }

    if (eye_strength > 0) {
      num2 = 0;
      if (hit_eye) {
        num2 = num2 + 1;
        return true;
      }
    }

    if (wing_strength > 0) {
      num3 = 0;
      if (hit_wing) {
        num3 = num3 + 1;
        return true;
      }
    }
       
    if (!hit_eye && !hit_body && !hit_wing) {
      return false;
    }
      
  }


  // Check if the Boss has touched the player
 bool has_touched_player(int x_arg, int y_arg) {
    if (get_body_strength() > 0 || get_eye_strength() > 0 || get_wing_strength() > 0) {
        bool body_hit = (((x_arg == x + 3) || (x_arg == x + 4) || (x_arg == x + 7) || (x_arg == x + 8)) && (y_arg == y + 5)) || (((x_arg == x + 5) || (x_arg == x + 6)) && (y_arg == y + 4));
        bool eye_hit = (x_arg >= (x + 5) && x_arg <= (x + 6)) && (y_arg == y);
        bool wing_hit = ((x_arg >= x && x_arg <= (x + 2)) || (x_arg >= (x + 9) && x_arg <= (x + 11))) &&
                        (y_arg >= y && y_arg <= (y + 2));

        // Return true if any part of the boss has touched the player's cannon
        if (body_hit && body_strength > 0) {
            return true;
        }
        else if ((eye_hit || wing_hit) && (wing_strength > 0) && (body_strength <= 0)) {
          return true;
        }
        else if (eye_hit && (eye_strength > 0 )&& (wing_strength <= 0 )&& (body_strength <= 0)) {
          return true;
        }
    }
    return false;
}


  // Check if the Boss has reached the bottom
  bool has_reached_bottom() {
    bool hit_body = (y + 5 == 15);
    bool hit_eye = (y == 15);
    bool hit_wing = (y + 2 == 15);

    if (body_strength > 0 && hit_body) {
      reached_bottom = true;
    }

    else if (body_strength <= 0 && hit_wing && wing_strength > 0) {
      reached_bottom = true;
    }

    else if (body_strength <= 0 && wing_strength <= 0 && eye_strength > 0 && hit_eye ) {
      reached_bottom = true;
    }
    else {
      reached_bottom = false;
    }
    return reached_bottom;
    
  }

  // Set position and strengths of the Boss
  void initialize(int x_arg, int y_arg, int body_strength_arg, int eye_strength_arg, int wing_strength_arg) {
    x = x_arg;
    y = y_arg;
    body_strength = body_strength_arg;
    eye_strength = eye_strength_arg;
    wing_strength = wing_strength_arg;
    time = millis();
    reached_bottom = false;
  }

  // Draw the Boss based on its different parts and strengths
  void draw() {
    if (body_strength == 5) {
      draw_with_rgb1(AQUA);
    }

    if (body_strength == 4) {
      draw_with_rgb1(YELLOW);
    }

    if (body_strength == 3) {
      draw_with_rgb1(BLUE);
    }

    if (body_strength == 2) {
      draw_with_rgb1(RED);
    }

    if (body_strength == 1) {
      draw_with_rgb1(ORANGE);
    }

    if (wing_strength == 3) {
      draw_with_rgb3(BLUE);
    }

    if (wing_strength == 2) {
      draw_with_rgb3(YELLOW);
    }

    if (wing_strength == 1) {
      draw_with_rgb3(RED);
    }

    if (eye_strength == 2) {
      draw_with_rgb2(ORANGE);
    }

    if (eye_strength == 1) {
      draw_with_rgb2(AQUA);
    }
  }

  // Erase the Boss from the screen
  void erase() {
    //if (body_strength <= 0) {
      draw_with_rgb1(BLACK);
    //}
    

    //if (wing_strength <= 0) {
      draw_with_rgb2(BLACK);
    //}

    //if (eye_strength <= 0) {
      draw_with_rgb3(BLACK);
    //} 
  }

  // When the Boss is hit, update its strengths accordingly
  void hit() { 
    if ((eye_strength > 0) && (num2 == 1) ) {
        eye_strength--;
    } 
    else if ((wing_strength > 0) && (num3 == 1)) {
        wing_strength--;
    } 
    else if ((body_strength > 0) && (num1 == 1)) {
        body_strength--;
    }
    // Check if any part's strength is reduced to zero, if so, erase that part
    if (body_strength <= 0) {
        draw_with_rgb1(BLACK);
    }
    if (eye_strength <= 0) {
        draw_with_rgb2(BLACK);
    }
    if (wing_strength <= 0) {
        draw_with_rgb3(BLACK);
    } 
  }



private:
  int x;
  int y;
  int body_strength;
  int eye_strength;
  int wing_strength;
  unsigned long time;
  bool reached_bottom;

  // Method to draw the Boss with larger pixels and varying strengths for different parts
  void draw_with_rgb1(Color body_color) {
    // body color
    matrix.drawPixel(x + 3, y + 1, body_color.to_333());
    matrix.drawPixel(x + 3, y + 2, body_color.to_333());
    matrix.drawPixel(x + 3, y + 3, body_color.to_333());
    matrix.drawPixel(x + 3, y + 4, body_color.to_333());
    matrix.drawPixel(x + 3, y + 5, body_color.to_333());
    matrix.drawPixel(x + 4, y + 1, body_color.to_333());
    matrix.drawPixel(x + 4, y + 2, body_color.to_333());
    matrix.drawPixel(x + 4, y + 3, body_color.to_333());
    matrix.drawPixel(x + 4, y + 4, body_color.to_333());
    matrix.drawPixel(x + 4, y + 5, body_color.to_333());
    matrix.drawPixel(x + 5, y + 1, body_color.to_333());
    matrix.drawPixel(x + 5, y + 2, body_color.to_333());
    matrix.drawPixel(x + 5, y + 3, body_color.to_333());
    matrix.drawPixel(x + 5, y + 4, body_color.to_333());
    matrix.drawPixel(x + 6, y + 1, body_color.to_333());
    matrix.drawPixel(x + 6, y + 2, body_color.to_333());
    matrix.drawPixel(x + 6, y + 3, body_color.to_333());
    matrix.drawPixel(x + 6, y + 4, body_color.to_333());
    matrix.drawPixel(x + 7, y + 1, body_color.to_333());
    matrix.drawPixel(x + 7, y + 2, body_color.to_333());
    matrix.drawPixel(x + 7, y + 3, body_color.to_333());
    matrix.drawPixel(x + 7, y + 4, body_color.to_333());
    matrix.drawPixel(x + 7, y + 5, body_color.to_333());
    matrix.drawPixel(x + 8, y + 1, body_color.to_333());
    matrix.drawPixel(x + 8, y + 2, body_color.to_333());
    matrix.drawPixel(x + 8, y + 3, body_color.to_333());
    matrix.drawPixel(x + 8, y + 4, body_color.to_333());
    matrix.drawPixel(x + 8, y + 5, body_color.to_333());

  }

  void draw_with_rgb2(Color eye_color) {
    // Implement drawing of different parts of the Boss with different colors and sizes
    // eye color
    matrix.drawPixel(x + 5, y, eye_color.to_333());
    matrix.drawPixel(x + 6, y, eye_color.to_333());
  }

  void draw_with_rgb3(Color wing_color) {
    // Implement drawing of different parts of the Boss with different colors and sizes
    // wing color
    matrix.drawPixel(x, y, wing_color.to_333());
    matrix.drawPixel(x + 1, y, wing_color.to_333());
    matrix.drawPixel(x + 2, y, wing_color.to_333());
    matrix.drawPixel(x, y + 1, wing_color.to_333());
    matrix.drawPixel(x + 1, y + 1, wing_color.to_333());
    matrix.drawPixel(x + 2, y + 1, wing_color.to_333());
    matrix.drawPixel(x, y + 2, wing_color.to_333());
    matrix.drawPixel(x + 1, y + 2, wing_color.to_333());
    matrix.drawPixel(x + 2, y + 2, wing_color.to_333());
    matrix.drawPixel(x + 9, y, wing_color.to_333());
    matrix.drawPixel(x + 10, y, wing_color.to_333());
    matrix.drawPixel(x + 11, y, wing_color.to_333());
    matrix.drawPixel(x + 9, y + 1, wing_color.to_333());
    matrix.drawPixel(x + 10, y + 1, wing_color.to_333());
    matrix.drawPixel(x + 11, y + 1, wing_color.to_333());
    matrix.drawPixel(x + 9, y + 2, wing_color.to_333());
    matrix.drawPixel(x + 10, y + 2, wing_color.to_333());
    matrix.drawPixel(x + 11, y + 2, wing_color.to_333());
  }
};


class Cannonball {
public:
  Cannonball() {
    x = 0;
    y = 0;
    fired = false;
    time = millis();
  }
   // resets private data members to initial values
  void reset() {
    x = 0;
    y = 0;
    fired = false;
    time = millis();
  }
   // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  bool has_been_fired() const {
    return fired;
  }
   // sets private data members
  void fire(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
    fired = true;
    //music
   // tone(13, NOTE_C3, 500);
  }
   // moves the Cannonball and detects if it goes off the screen
  // Modifies: y, fired




  void move() {
    if(fired && (millis() - time) > CANNON_BALL_DELAY){
       erase();
       y--;
       if(y < 0) {
         reset();
       }
       else {
         draw();
       }
       time = millis();
     }
    }
   // resets private data members to initial values
  void hit() {
    erase();
    reset();
  }




  // draws the Cannonball, if it is fired
  void draw() {
    if (fired == true){
      matrix.drawPixel(x, y, ORANGE.to_333());
      matrix.drawPixel(x, y - 1, ORANGE.to_333());
    }
    return;
  }
 
   // draws black where the Cannonball used to be
  void erase() {
  matrix.drawPixel(x, y, BLACK.to_333());
  matrix.drawPixel(x, y - 1, BLACK.to_333());
  return;
 }






private:
  int x;
  int y;
  bool fired;
  unsigned long time;
};




class Player {
public:
  Player() {
    x = 1;
    y = 14;
    lives = 3;
  }
   // getters
  int get_x() const {
    return x;
  }
  int get_y() const {
    return y;
  }
  int get_lives() const {
    return lives;
  }
   // setter
  void set_x(int x_arg) {
    x = x_arg;
  }
   // Modifies: lives
  void die() {
    lives--;
  }
  void respawn(){
    lives = 4;
  }
   // draws the Player
  // calls: draw_with_rgb
  void draw() {
    draw_with_rgb(AQUA);
  }
   // draws black where the Player used to be
  // calls: draw_with_rgb
  void erase() {
    draw_with_rgb(BLACK);
  }





private:
  int x;
  int y;
  int lives;


  // sets private data members x and y to initial values
  void initialize(int x_arg, int y_arg) {
    x = x_arg;
    y = y_arg;
    lives = 3;
  }
   // draws the player
  void draw_with_rgb(Color color) {
    matrix.drawPixel(x, y+1, color.to_333());
    matrix.drawPixel(x-1, y+1, color.to_333());
    matrix.drawPixel(x+1, y+1, color.to_333());
     // player's cannon
    matrix.drawPixel(x, y, color.to_333());
  }
};


class Game {
 public:
  Game() {
     level = 0;
     time = 0;
   }
   // sets up a new game of Space Invaders
   // Modifies: global variable matrix
  void setupGame() {
    //setupMusic();

    //initialize invaders and boss
    Invader enemies[NUM_ENEMIES / 2];
    for (int i = 0; i < NUM_ENEMIES / 2; ++i) {
      enemies[i].initialize(4 * i, 0, 1);
      enemies[i].draw();
    }

    boss1.initialize(0, 5, 5, 2, 3);
    boss2.initialize(20, 5, 5, 2, 3);
    
    //initialize player
    Player player;
    player = Player();
    player.draw();
    
    //set up level
    setupLevel();

    //setup opening and level and stuff
    setupInitial();
  }
  
   // advances the game simulation one step and renders the graphics
   // see spec for details of game
   // Modifies: global variable matrix
  void update(int potentiometer_value, bool button_pressed) {
    if (level == 0){
      menu(potentiometer_value, button_pressed);
    }
    else if (level != 0)
      if (gamemode == 1){
        easy_gamemode(potentiometer_value, button_pressed);
      }
      else if (gamemode == 2){
        luna_gamemode(potentiometer_value, button_pressed);
      }
  } // This should be the closing brace for the else if block

  
 private:
   int level;
   unsigned long time;
   Player player;
   Cannonball ball;
   Invader enemies[NUM_ENEMIES];
   int balltime;
   int invadertime;
   int num_invaders;
   short gamemode;
   int score = 0;
   int higher_score = 0;
   bool is_game_over = false;
   int potentiometer_value2 = 0;
   Boss boss1;
   Boss boss2;

  //  void setupMusic() {
  //   int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
  //   int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};
  //   for (int thisNote = 0; thisNote < 8; thisNote++) {
  //     int noteDuration = 1000 / noteDurations[thisNote];
  //     tone(13, melody[thisNote], noteDuration);
  //     int pauseBetweenNotes = noteDuration * 1.30;
  //     delay(pauseBetweenNotes);
  //     noTone(13);
  //   }
  //  }

  void setupLevel(){
    if (level == 1) {
      num_invaders = 8; 
    }
    else if (level%6 == 0) {
      num_invaders = 8;
    }
    else{
      num_invaders = 16;
    }
    if (level%6 == 0) {
      for (int i = 0; i < NUM_ENEMIES / 2; ++i) {
       enemies[i].initialize(4 * i, 10, 3);
       enemies[i].draw();
      }
    }
  }

  void setupInitial(){
    reset_invaders();
    print_level(level);
    delay(2000);
    print_lives(player.get_lives());
    delay(2000);
    matrix.fillScreen(BLACK.to_333());
  }

  void menu(int potentiometer_value, bool button_pressed){
    int menu_choose = (((MAT_WIDTH) * potentiometer_value) / 1024);
    if (menu_choose < 16){
      easy();
      if (button_pressed == true){
        level = 1;
        gamemode = 1;
        setupGame();
      }
    }
    else if (menu_choose >= 16){
      luna();
      if (button_pressed == true){
        level = 1;
        gamemode = 2;
        setupGame();
      }
    }
  }

  void if_game_over(){
    if(is_game_over == true) {
       game_over();
       delay(2000);
       if (score > higher_score){
        higher_score = score;
        new_score();
        delay(2000);
       }
       print_score();
       delay(10000);
       level = 0;
       player.respawn();
       is_game_over = false;
       return;
     }
  }

  void player_move(int potentiometer_value, bool button_pressed){
    if (abs(potentiometer_value - potentiometer_value2) > 30) {
      potentiometer_value2 = potentiometer_value;
      int player_x = (((MAT_WIDTH) * potentiometer_value) / 1024);

      if (player_x == -1){//not tested, may cause error
        player.set_x(MAT_WIDTH - 2);
      }

      else if (player_x == MAT_WIDTH - 1){//number should be changed probably?
        player.set_x(0);
      }

      else{
      player.set_x(player_x);//potentiometer_value2?
      }
    }
  }

  void cannonball_fire(int potentiometer_value, bool button_pressed){
    if (button_pressed && !ball.has_been_fired()) {
      time = millis();
      ball.fire(player.get_x(), 14);
    }

    if (ball.has_been_fired() && (millis() - time) > CANNON_BALL_DELAY) {
      ball.move();
      }
  }
  
  void easy_invader_move(){
    for (int i = 0; i < num_invaders; ++i) {
      enemies[i].draw();
      // Move first row first and do not move second row until the first row all die
      if ((i / 8 == 0) || enemies_killed(8)) {
        enemies[i].easy_move();
      }
      // Detect if the cannonball is colliding with any of the invaders;
      // if so, both the cannonball and the invader disappear
      if (ball.has_been_fired()) {
        if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {
          score += 10;
          // redraw the enemy
          enemies[i].hit(); // erase the ball
          ball.hit();       // Detect if all the invaders are killed; if so, start the next level
          if (level_cleared()) {
            //Serial.println("Level Cleared!");
            next_level();
            score += 50;
          }
        }
      }
      // Detect whether the player is colliding with any of the invaders; if so, the player loses a life and the level restarts
      if (enemies[i].has_touched_player(player.get_x(), player.get_y()) || enemies[i].has_reached_bottom()) {
        player.die();
        if (player.get_lives() < 1) {
          is_game_over = true;
          game_over();
        } 
        else {
          reset_level();
        }
      }
    }
  }

  void luna_invader_move(){
    for (int i = 0; i < num_invaders; ++i) {
      enemies[i].draw();
      // Move first row first and do not move second row until the first row all die
      if ((i / 8 == 0) || enemies_killed(8)) {
        enemies[i].luna_move();
      }
      // Detect if the cannonball is colliding with any of the invaders;
      // if so, both the cannonball and the invader disappear
      if (ball.has_been_fired()) {
        if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {
          score += 20;
        // redraw the enemy
        enemies[i].hit(); // erase the ball
        ball.hit();       // Detect if all the invaders are killed; if so, start the next level
          if (level_cleared()) {
            //Serial.println("Level Cleared!");
            next_level();
            score += 70;
          }
        }
      }
      // Detect whether the player is colliding with any of the invaders; if so, the player loses a life and the level restarts
      if (enemies[i].has_touched_player(player.get_x(), player.get_y()) || enemies[i].has_reached_bottom()) {
        player.die();
        if (player.get_lives() < 1) {
          is_game_over = true;
          game_over();
        } 
        else {
          reset_level();
        }
      }
    }
  }

  void easy_invader_Boss_move(){
    for (int i = 0; i < num_invaders; ++i) {
      enemies[i].draw();

        // Move first row first and do not move second row until the first row all die
      if ((i / 8 == 0) || enemies_killed(8)) {
        enemies[i].easy_move();
      }
      // Detect if the cannonball is colliding with any of the invaders;
      // if so, both the cannonball and the invader disappear

      if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {
        score += 10;
        // redraw the enemy
        enemies[i].hit(); // erase the ball
        ball.hit();       // Detect if all the invaders are killed; if so, start the next level

        if (level_cleared()) {
          //Serial.println("Level Cleared!");
          next_level();
          score += 80;
        }
      }

      if (ball.has_been_fired()) {

        if (boss1.has_been_hit(ball.get_x(), ball.get_y())) {
          score += 10;
          boss1.hit(); // Reduce boss strength
          ball.hit(); // Remove the cannonball
        }

        if (boss2.has_been_hit(ball.get_x(), ball.get_y())) {
          score += 10;
          boss2.hit(); // Reduce boss strength
          ball.hit(); // Remove the cannonball
        }
      }

      // Detect whether the player is colliding with any of the invaders; if so, the player loses a life and the level restarts
      if (enemies[i].has_touched_player(player.get_x(), player.get_y()) || enemies[i].has_reached_bottom()) {
        player.die();
        if (player.get_lives() < 1) {
          is_game_over = true;
          game_over();
        }

        else {
          reset_level();
        }
      }
    }
  }

  void luna_invader_Boss_move(){
    for (int i = 0; i < num_invaders; ++i) {
      enemies[i].draw();

        // Move first row first and do not move second row until the first row all die
      if ((i / 8 == 0) || enemies_killed(8)) {
        enemies[i].luna_move();
      }
      // Detect if the cannonball is colliding with any of the invaders;
      // if so, both the cannonball and the invader disappear

      if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {
        score += 20;
        // redraw the enemy
        enemies[i].hit(); // erase the ball
        ball.hit();       // Detect if all the invaders are killed; if so, start the next level

        if (level_cleared()) {
          //Serial.println("Level Cleared!");
          next_level();
          score += 100;
        }
      }

      if (ball.has_been_fired()) {

        if (boss1.has_been_hit(ball.get_x(), ball.get_y())) {
          score += 10;
          boss1.hit(); // Reduce boss strength
          ball.hit(); // Remove the cannonball
        }

        if (boss2.has_been_hit(ball.get_x(), ball.get_y())) {
          score += 10;
          boss2.hit(); // Reduce boss strength
          ball.hit(); // Remove the cannonball
        }
      }

      // Detect whether the player is colliding with any of the invaders; if so, the player loses a life and the level restarts
      if (enemies[i].has_touched_player(player.get_x(), player.get_y()) || enemies[i].has_reached_bottom()) {
        player.die();
        if (player.get_lives() < 1) {
          is_game_over = true;
          game_over();
        }

        else {
          reset_level();
        }
      }
    }
  }

  void boss_touch_player(){
    if ((boss1.has_touched_player(player.get_x(), player.get_y())) || (boss2.has_touched_player(player.get_x(), player.get_y()))) {
       player.die(); // Player loses a life
       if (player.get_lives() < 1) {
         is_game_over = true;
         game_over();
       }
       else {
         reset_level(); // Reset the level if the player has lives left
       }
    }
  }

  void boss_reach_bottom(){
    if ((boss1.has_reached_bottom()) || (boss2.has_reached_bottom())) {
      player.die(); // Player loses a life
      if (player.get_lives() < 1) {
        is_game_over = true;
        game_over();
      }
      else {
        reset_level(); // Reset the level if the player has lives left
      }
    }
  }

  void easy_boss_level(int potentiometer_value, bool button_pressed){
    boss1.draw();
    boss1.move();
    boss2.draw();
    boss2.move();
    // Check if the cannonball is fired and handle its movement
    cannonball_fire(potentiometer_value, button_pressed);

    easy_invader_Boss_move();

    // Check if the boss collides with the player
    boss_touch_player();
    // Check if the boss reaches the bottom
    boss_reach_bottom();
  }

  void luna_boss_level(int potentiometer_value, bool button_pressed){
    boss1.draw();
    boss1.move();
    boss2.draw();
    boss2.move();
    // Check if the cannonball is fired and handle its movement
    cannonball_fire(potentiometer_value, button_pressed);

    luna_invader_Boss_move();

    // Check if the boss collides with the player
    boss_touch_player();
    // Check if the boss reaches the bottom
    boss_reach_bottom();
  }

  void easy_gamemode(int potentiometer_value, bool button_pressed){
    if_game_over();
    // Update the position of the player based on the value of the potentiometer
    player.erase();
    player_move(potentiometer_value, button_pressed);
    player.draw();

    if (level%6 == 0) {
      // Update the boss position and behavior
      easy_boss_level(potentiometer_value, button_pressed);
    }
    else if (level %6 != 0) {
      // Update the position of the cannonball, including detecting if a new cannonball is being fired
      cannonball_fire(potentiometer_value, button_pressed);
      // Update the position of each invader needed to be done
      easy_invader_move();
    }
  }

  void luna_gamemode(int potentiometer_value, bool button_pressed){
    if_game_over();
    // Update the position of the player based on the value of the potentiometer
    player.erase();
    player_move(potentiometer_value, button_pressed);
    player.draw();

    if (level%6 == 0) {
      // Update the boss position and behavior
      luna_boss_level(potentiometer_value, button_pressed);
    }
    else if (level %6 != 0) {
      // Update the position of the cannonball, including detecting if a new cannonball is being fired
      cannonball_fire(potentiometer_value, button_pressed);
      // Update the position of each invader needed to be done
      luna_invader_move();
    }
  }

  bool enemies_killed(int num_enemies){
     for (int i = 0; i < num_enemies; ++i){
       if (enemies[i].get_strength() != 0){
         return false;
       }
     }
     return true;
   }

   // check if Player defeated all Invaders in current level
   bool level_cleared() {
     return enemies_killed(NUM_ENEMIES);
   }


   // set up a level
   void reset_level() {
     level = level;
     setupGame();
   }

   void next_level(){
     level++;
     setupGame();
   }
   void reset_invaders(){
     int strength;
     for (int i = 0; i < num_invaders; ++i) {         
         if(level >= 1 && level <= 4){
           strength = strengths[level-1][i];
         }
         else{
           strength = random(1, 8);
         }
        
         enemies[i].set_strength(strength);
         enemies[i].set_pos(((i % 8) * 4), (num_invaders/16) * (1-(i/8)) * 3);
         enemies[i].clear_reached_bottom();
      }
   }
   void easy() {
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(1,0);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(7,0,0));
    matrix.print('E');
    matrix.print('A');
    matrix.print('S');
    matrix.print('Y');
  }

  void luna() {
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(1,0);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(0,0,7));
    matrix.print('L');
    matrix.print('U');
    matrix.print('N');
    matrix.print('A');
  }

  void print_score() {
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(1,0);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(7,0,0));
    matrix.print('S');
    matrix.print('C');
    matrix.print('O');
    matrix.print('R');
    matrix.print('E');
    matrix.print(higher_score);
  }

  void new_score(){
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(1,0);
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.Color333(0,0,7));
    matrix.print('R');
    matrix.print('E');
    matrix.print('C');
    matrix.print('O');
    matrix.print('R');
    matrix.print('D');
  }
};


// a global variable that represents the game Space Invaders
Game game;


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  Welcome_Screen();
  delay(2000);
}


void loop() {
 int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
 bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
 game.update(potentiometer_value, button_pressed);
 delay(5);
}

// display welcome screen
void Welcome_Screen(){
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1,0);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(0,7,0));
 
  matrix.print('S');
  matrix.print('T');
  matrix.print('A');
  matrix.print('R');
  matrix.print('T');
  matrix.print('G');
  matrix.print('A');
  matrix.print('M');
  matrix.print('E');
}

// displays Level
void print_level(int level) {
 matrix.fillScreen(BLACK.to_333());
 matrix.setCursor(1,0);
 matrix.setTextSize(1);
 matrix.setTextColor(matrix.Color333(0,7,0));
 matrix.print('L');
 matrix.print('E');
 matrix.print('V');
 matrix.print('E');
 matrix.print('L');
 matrix.print(' ');
 matrix.print(level);
}


// displays number of lives
void print_lives(int lives) {
 matrix.fillScreen(BLACK.to_333());
 matrix.setCursor(1,0);
 matrix.setTextSize(1);
 matrix.setTextColor(matrix.Color333(0,7,0));
 matrix.print('L');
 matrix.print('I');
 matrix.print('V');
 matrix.print('E');
 matrix.print('S');
 matrix.print(' ');
 matrix.print(lives);
}


// displays "game over"
void game_over() {
 matrix.fillScreen(BLACK.to_333());
 matrix.setCursor(1,0);
 matrix.setTextSize(1);
 matrix.setTextColor(matrix.Color333(0,7,0));
 matrix.print('G');
 matrix.print('A');
 matrix.print('M');
 matrix.print('E');
 matrix.print(' ');
 matrix.print('O');
 matrix.print('V');
 matrix.print('E');
 matrix.print('R');
}
