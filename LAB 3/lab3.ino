int controllerValue;
float voltage;
unsigned long TIME_RED;
unsigned long TIME_GREEN;
unsigned long TIME_YELLOW;
unsigned long TIME_NOW;
unsigned long TIME_STATE_DURATION;
unsigned long TIME_LEFT_GREEN;
unsigned long TIME_OBSTACLE_APPEARED;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long intervalBlink = 1000;           // interval at which to blink (milliseconds)
int yellowBlinkState = LOW;             // ledState used to set the LED
boolean obstaclePresentBefore = false;

int state; //State is a variable for having knowledge of the current state
           //Assignment can be GREEN,YELLOW,RED variables          
//Pin setup 
int GREEN = 3;
int YELLOW = 5;
int RED = 6;

//Time setup 
int GREEN_INTERVAL = 20000;
int RED_INTERVAL = 15000;
int YELLOW_INTERVAL = 5000;

void setup() {
  Serial.begin(9600);
  pinMode(GREEN,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(RED,OUTPUT);
  greenOn();
  TIME_NOW = millis();
}

void loop() {
  controllerValue = analogRead(A3);
  voltage = controllerValue*(5.0/1023);
  //Serial.println(voltage);
  
  
  //Obstacle present
  if(voltage < 0.5){
    if(obstaclePresentBefore == false){
      TIME_OBSTACLE_APPEARED = millis();
      obstaclePresentBefore = true;
    }

    TIME_NOW = millis();
    if(state==GREEN){
      if(TIME_LEFT_GREEN > 3000){
         //We have enough time (more than 3 seconds)
         Serial.println(TIME_NOW-TIME_OBSTACLE_APPEARED);
         if(TIME_NOW - TIME_OBSTACLE_APPEARED >= 3000){
             digitalWrite(GREEN, LOW);
             Serial.println(TIME_NOW - TIME_OBSTACLE_APPEARED);
             if(TIME_NOW - TIME_OBSTACLE_APPEARED <= 11000){   //less than 8 sec have passed. Blink yellow light for 8 sec per 1 sec   
                unsigned long currentMillis = millis();
                blinkYellow();
             }else{
                 Serial.println("Red on.....");
                 digitalWrite(YELLOW, LOW);  
                 redOn();
             }
         }
    
      }else{
        //We have less than 3 seconds
        TIME_NOW = millis();
        if(TIME_NOW - TIME_OBSTACLE_APPEARED >= TIME_LEFT_GREEN){
          digitalWrite(GREEN, LOW);
          if((TIME_NOW - TIME_OBSTACLE_APPEARED) <= (TIME_LEFT_GREEN + 8000)){
            blinkYellow();         
          }else{
            digitalWrite(YELLOW, LOW);
            redOn();
          }
        } 
      }
      
    }else if(state==YELLOW){
      normalCycle();
    }else if(state==RED){
      TIME_STATE_DURATION = millis();
      normalCycle();
    }
    
          
  }else{
    obstaclePresentBefore = false;
    normalCycle();    
  }
}

void greenOn(){
    TIME_STATE_DURATION = millis();
    state=GREEN;
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
  }

  void yellowOn(){
    TIME_STATE_DURATION = millis();
    state=YELLOW;
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, HIGH);
  }

  void redOn(){
    TIME_STATE_DURATION = millis();
    state=RED;
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);
  }

  void normalCycle(){
    TIME_NOW = millis();
    if(state==GREEN){
      TIME_LEFT_GREEN = GREEN_INTERVAL - (TIME_NOW - TIME_STATE_DURATION);
      if(TIME_NOW - TIME_STATE_DURATION > GREEN_INTERVAL){ //Switch to yellow state     
         yellowOn();
      } 
    }else if(state==YELLOW){
      if(TIME_NOW - TIME_STATE_DURATION > YELLOW_INTERVAL){ //Switch to red state     
         redOn();
      }     
    }else if(state==RED){
      if(TIME_NOW - TIME_STATE_DURATION > RED_INTERVAL){ //Switch to green state     
         greenOn();
      }     
    }
  }

  void blinkYellow(){
     unsigned long currentMillis = millis();
    
      if (currentMillis - previousMillis >= intervalBlink) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
  
        // if the LED is off turn it on and vice-versa:
        if (yellowBlinkState == LOW) {
           yellowBlinkState = HIGH;
        } else {
           yellowBlinkState = LOW;
        }
  
        // set the LED with the ledState of the variable:
       digitalWrite(YELLOW, yellowBlinkState);
      }
  }
