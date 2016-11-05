

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define ultrasonic pins
const int trigPin = 30;
const int echoPin = 31;

String state = "alive!";
String oldState = state;
String command = "";

// define ultrasonic variables
long duration;
long distance;


void setup() {
  // Begin serial connection
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  
 
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
 // lcd.print("hello, world!");
}

void loop() {
  
    oldState = state;    

    listenCommand();
    
    
    
   
    
    // clear lcd if state changes
    if (oldState != state) {
      lcd.clear();
    }
    
    lcd.setCursor(0, 0);
    lcd.print(state);
  
  /*  if (command != "no command"){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(command);
      
      Serial.print(command);
    } */

    
     sonic();
     delay(500);

}

// test code for ultrasonic sensor
void sonic(){
  
  // Clears the trigPin
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  
  
  // print values to lcd
  lcd.setCursor(0, 1);
  lcd.print("           ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
}


// command listener method
void listenCommand() {

  if (Serial.available() > 0) {
    char c = Serial.read();
    
    // find the beginning of new command
    if(c=='['){
      command = "[";  
    } else command += c;
    
    // find the end of command
    if(c==']'){
      evaluateCommand(command);
    }
    
  } 
  
}


// return state via serial
void tellState() {
  Serial.println(state);
}

// evaluate command
void evaluateCommand(String c){
  
  // reject command if it is too short
  if (c.length()<4){
    command = "";
    return;
  } 
  
  // parse [××.....] xx:s from command to define action
  String action = "";
  action += c[1];
  action += c[2];
  
  Serial.println(action);
  
  if (action == "MO"){
    state = "liikutaan";
  }
  
  if (action == "RE"){
    state = "pakitetaan";
  }
  
  if (action == "TR"){
    state = "kaannytaan oikealle";
  }
  
  if (action == "SP"){
    state = "pysahdytty";
  }
  
  if (action == "TS"){
    lcd.setCursor(0,1);
    lcd.print(c[0]);
    tellState();
  }  
  
  // clear command 
  command = "";
  
}
  
  
 

  



