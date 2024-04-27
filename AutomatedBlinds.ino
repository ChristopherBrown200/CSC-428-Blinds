#include "Adafruit_ThinkInk.h"
#include <stdlib.h>
#include <ESP32Servo.h>
#include <WiFi.h>

//WiFI Control ==========================================================================================
//Network Credintals
const char* ssid = "***************";
const char* password = "*****************";

WiFiServer server(80);

String header;

String blindsState = "Open";
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//Servo ======================================================================================================
//Constants for ServoMotor
#define ServoPin 12
#define ServoPulseHigh 2500
#define ServoPulseLow 500 

//create the motor object
Servo motor;

//varibles used in controling when the motor stops
bool motorOn = false;
long stopTime = 0;
// long maxLong = 0x7FFFFFFF;
bool checkTime = false;

//Display ====================================================================================================
//Constans for Display
#define epdDc 2
#define epdCs 15
#define sramCs 0
#define epdReset 4
#define epdBusy 16

//Declare the display Object
ThinkInk_154_Mono_D67 display(epdDc, epdReset, epdCs, sramCs, epdBusy);

//Used to set different status lines on the display
enum TextLines{
  Setup = -1,
  Title = 0,
  NetworkLable = 1,
  Network = 2,
};

//Stores the current data on the display
#define NumLines 6
String displayLines[NumLines] = {"Blinds", "Wifi:", "Searching..", ""};

//Encoder ================================================================================================
//Constanst for the encoder
#define CLK_PIN 27
#define DT_PIN  26
#define SW_PIN  25
#define DIRECTION_CW  0
#define DIRECTION_CCW 1

//varibles for the encoder 
int direction = DIRECTION_CW;
int CLK_state;
int prev_CLK_state;

//Limit Switch ================================================================================================
#define LSW_PIN 22

//Setup ========================================================================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  //Sets Up the Display
  display.begin(THINKINK_MONO);
  display.setTextSize(3);
  display.setTextColor(EPD_BLACK);
  setDisplay();

  //Sets up the encoder
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  pinMode(SW_PIN, INPUT);
  prev_CLK_state = digitalRead(CLK_PIN);

  //Setup Motor
  motor.attach(12, 500, 2500);

  //Set up limit switch
  pinMode(LSW_PIN, INPUT);

  //Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  setDisplay(WiFi.localIP().toString(), 2);
  server.begin();

  closeCommand();
}

//Loop =============================================================================================================
void loop() {
  if((checkTime && stopTime <= millis()) || (blindsState == "Open" && digitalRead(LSW_PIN) == 1)){
    stopTurning();
  }

  if(digitalRead(LSW_PIN) == 1){
    blindsState = "Closed";
  }
  else{
    blindsState = "Open";
  }

  if(digitalRead(SW_PIN) == 0){
    if (blindsState == "Closed"){
      openCommand();
    }
    else{
      closeCommand();
    }
  }

  CLK_state = digitalRead(CLK_PIN);
  WiFiClient client = server.available();

  if (client) { 
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = ""; 
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      if (client.available()) { 
        char c = client.read(); 
        header += c;
        if (c == '\n') { 
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /Blinds/Open") >= 0) {
              openCommand();
            } else if (header.indexOf("GET /Blinds/Close") >= 0) {
              closeCommand();
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #E3E329; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #000000;}</style></head>");
            
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<p>Blinds - State " + blindsState + "</p>");
            if (blindsState == "Closed") {
              client.println("<p><a href=\"/Blinds/Open\"><button class=\"button\">Open</button></a></p>");
            } else {
              client.println("<p><a href=\"/Blinds/Close\"><button class=\"button button2\">Close</button></a></p>");
            } 
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }

  CLK_state = digitalRead(CLK_PIN);
  if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
    //encoder is rotating counter-clockwise
    if (digitalRead(DT_PIN) == HIGH) {
      if(motorOn && direction == DIRECTION_CCW){
        stopTime += 500;
        checkTime = true;
      }
      else if(motorOn && direction == DIRECTION_CW){
        stopTurning();
      }
      else{
        turnCCW();
        stopTime = millis() + 500;
      }
      
      direction = DIRECTION_CCW;
    }
    //encoder is rotating clockwise
    else {
      if(motorOn && direction == DIRECTION_CW){
        stopTime += 500;
        checkTime = true;
      }
      else if(motorOn && direction == DIRECTION_CCW){
        stopTurning();
      }
      else{
        turnCW();
        stopTime = millis() + 500;
      }

      direction = DIRECTION_CW;
    }
  }
  prev_CLK_state = CLK_state;
}


void openCommand(){
  turnCCW();
  while(digitalRead(LSW_PIN) != 1){
    delay(10);
  }
  turnCW();
  delay(4000);
  stopTurning();
  blindsState = "Open";
}

void closeCommand(){
  turnCCW();
  while(digitalRead(LSW_PIN) != 1){
    delay(10);
  }
  stopTurning();
  blindsState = "Closed";
}

void setDisplay(){
  display.clearBuffer();
  display.setCursor(0,0);
  for (int i = 0; i < NumLines; i++){
    display.print(displayLines[i] + "\n");
  }
  display.display(); 
}

void setDisplay(String str, int lineNum){
  displayLines[lineNum] = str;
  setDisplay(); 
}

void turnCW(){
  motor.write(0);
  motorOn = true;
}

void turnCCW(){
  motor.write(180);
  motorOn = true;
}

void stopTurning(){
  motor.write(90);
  motorOn = false;
  delay(200);
  checkTime = false;
}