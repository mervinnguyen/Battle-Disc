/*
  Project Name: Battle Disc 2.0: Miniaturized Ping Ping
  
  Resume Description:
    - Team of 2 to develop an embedded system game with Arduino Uno using PlatformIO and Fritzing, end goal of deploying 50+ system for after-school programs.
    - Programmed in C++ with Adafruit_GFX and Adafruit_SSD1306 libraries, introducing specific in-game movements/motion controls of the game.
    - Designed a circuit and breadboard schematic, incorporating I2C communication to display data on an I2C OLED display.

  Engineers: Ben Soo, Mervin Nguyen
  Date: 09/07/2023
*/

//Libraries for I2C Communication Protocol
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>           
#include <Adafruit_SSD1306.h>       

//Wire Push Buttons to Digital Input of microcontroller "2" and "3"
#define UP_BUTTON 2         
#define DOWN_BUTTON 3

//Initialize constant variables (units: pixels/frame)
const unsigned long paddleRate = 5;
const unsigned long ballRate = 5;
const uint8_t PADDLE_HEIGHT = 10;

//set resolution size in pixels 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

// Declaration for an SSD1306 display connected to I2C with reset pin 4
#define OLED_RESET     4 

//Initialize the OLED display (resolution, Addressing memory trasferred through the wire, allowing display to reset)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//call function to draw display
void drawCourt();

//initial position and direction 
uint8_t ball_x = 64, ball_y = 32;               //"uint8_t" declares variable with values with 8 bits (positive 0-255)
uint8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update;
unsigned long paddle_update;

//sets intial position of cpu paddle (can only move in y-direction)
const uint8_t CPU_X = 12;
uint8_t cpu_y = 16;

//sets intial position of player paddle (can only move in y-direction)
const uint8_t PLAYER_X = 115;
uint8_t player_y = 16;

//set variable types for scores
int cpuScore;       
int playerScore;  

void drawCourt() {
    display.drawRect(0, 0, 128, 64, WHITE);
} 
    
void setup() {
    
    //initialize local variables to start game at 0-0
    cpuScore = 0;       
    playerScore = 0;   
    
     //display "Adafruit" for 1 seconds
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);      //initialize i2c oled display, following i2c communciation protocol, loads into address "0x3C" i2c BUS
    display.display();
    display.clearDisplay();
   
    delay(500);       

    //display initial message "Battle Disc 2.0 by Mervin and Ben" for 5 seconds
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(24,20);
    display.println("Battle Disc 2.0");
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(15,30);
    display.println("by Mervin and Ben");
    display.display();

    delay(2300);             

    //Clear initial message and setup game
    display.clearDisplay();
    
    //Call function created "void drawCourt()" and record time in ms
    void drawCourt();
    display.display();
    unsigned long start = millis();

    //set inputs
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    digitalWrite(UP_BUTTON,1);
    digitalWrite(DOWN_BUTTON,1);
    drawCourt();

    while(millis() - start < 100); //wait .5 seconds before starting game
    display.display();

    ball_update = millis();
    paddle_update = ball_update;

    
}

void restartGame() {
    
    //initialize variables to start game at 0-0
    cpuScore = 0;       
    playerScore = 0;   
    
     //display "Adafruit" for .5 seconds
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);      //initialize i2c oled display, following i2c communciation protocol, loads into address "0x3C" i2c BUS
    display.display();
    display.clearDisplay();
    delay(500);       

    //display initial message "Try Again Noob!" for 5 seconds
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(15,30);
    display.println("Try Again Noob!");
    display.display();

    delay(1000);             

    //Clear initial message and setup game
    display.clearDisplay();
    
    //Call function created "void drawCourt()" and record time in ms
    drawCourt();
    display.display();
    unsigned long start = millis();

    //set inputs
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    digitalWrite(UP_BUTTON,1);
    digitalWrite(DOWN_BUTTON,1);
    drawCourt();

    while(millis() - start < 100);     //wait .5 seconds before starting
    display.display();

    ball_update = millis();
    paddle_update = ball_update;
}


void loop() {

    //display point system
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(90, 0);
    display.print("YOU: ");
    display.print(playerScore);
    display.setCursor(0, 0);
    display.print("CPU: ");
    display.print(cpuScore);
    display.display();

    bool update = false;
    unsigned long time = millis();

    static bool up_state = false;
    static bool down_state = false;
    
    up_state |= (digitalRead(UP_BUTTON) == LOW);
    down_state |= (digitalRead(DOWN_BUTTON) == LOW);

    if(time > ball_update) {
        uint8_t new_x = ball_x + ball_dir_x;
        uint8_t new_y = ball_y + ball_dir_y;

        // Increment player's or cpu's point if we hit a horizontal wall and set back to center of display. Display message depending whoever reaches 2 points
        if(new_x == 0 || new_x == 127) {
            if(new_x == 0) {
                playerScore++;
                new_x = 64;     
            
                if(playerScore == 2){
                    delay(500);
                   
                    display.clearDisplay();
                    display.setTextSize(2);
                    display.setTextColor(WHITE);
                    display.setCursor(25,20);
                    display.println("WINNER!");
                    display.display();
                    delay(1000);     

                    //restart the game
                    restartGame();
                }
            
        
            } else if (new_x == 127){
                  cpuScore++;
                  new_x = 64;    
                
                  if (cpuScore == 2){
                      delay(500);
                    
                      display.clearDisplay();
                      display.setTextSize(2);
                      display.setTextColor(WHITE);
                      display.setCursor(1,30);
                      display.println("SORE LOSER");
                      display.display();
                      delay(1000);                       

                      //restart the game
                      restartGame();
                    }
                }
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }


//UPDATE COMMENTS FOR BELOW TO SHOW THE TRAJECTORY OF THE BALL OFF CONTACT



        // Check if we hit the vertical walls, traject ball at the opposite y direction
        if(new_y == 0 || new_y == 63) {
            ball_dir_y = -ball_dir_y;
            new_y += ball_dir_y + ball_dir_y;
        }

        // Check if we hit the CPU paddle, traject ball at the opposite x direction 
        if(new_x == CPU_X && new_y >= cpu_y && new_y <= cpu_y + PADDLE_HEIGHT) {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }

        // Check if we hit the player paddle
        if(new_x == PLAYER_X && new_y >= player_y && new_y <= player_y + PADDLE_HEIGHT) {
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
        }
        
        display.drawPixel(ball_x, ball_y, BLACK);
        display.drawPixel(new_x, new_y, WHITE);
        ball_x = new_x;
        ball_y = new_y;

        ball_update += ballRate;

        update = true;
    }

    if(time > paddle_update) {
        paddle_update += paddleRate;

        // CPU paddle
        display.drawFastVLine(CPU_X, cpu_y, PADDLE_HEIGHT, BLACK);
        const uint8_t half_paddle = PADDLE_HEIGHT >> 1;
        if(cpu_y + half_paddle > ball_y) {
            cpu_y -= 1;
        }
        if(cpu_y + half_paddle < ball_y) {
            cpu_y += 1;
        }
        if(cpu_y < 1) cpu_y = 1;
        if(cpu_y + PADDLE_HEIGHT > 63) cpu_y = 63 - PADDLE_HEIGHT;
        display.drawFastVLine(CPU_X, cpu_y, PADDLE_HEIGHT, WHITE);

        // Player paddle
        display.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, BLACK);
        if(up_state) {
            player_y -= 1;
        }
        if(down_state) {
            player_y += 1;
        }
        up_state = down_state = false;
        if(player_y < 1) player_y = 1;
        if(player_y + PADDLE_HEIGHT > 63) player_y = 63 - PADDLE_HEIGHT;
        display.drawFastVLine(PLAYER_X, player_y, PADDLE_HEIGHT, WHITE);

        update = true;

        display.fillRect(0,0,128,10, BLACK);    
    }

    if(update)
        display.display();
}