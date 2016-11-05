#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Servo.h>

// variables
String state = "alive!";
String oldState = state;
String command = "";
String message = "PLA-32310";

static int leftMotorDirPin = 12;
static int leftMotorBrakePin = 9;
static int leftMotorPowerPin = 3;
static int rightMotorDirPin = 13;
static int rightMotorBrakePin = 8;
static int rightMotorPowerPin = 11;

unsigned long int movementStopTime = 0;

static int lcdPowerPin = 40;


// variable for lcd refresh
unsigned long int lastLcdWrite;

// variables for sonic sensor
unsigned int distance = 0, lastDistance=0;
unsigned long lastSonic;

// variables for tilt & pan servos
int panPos = 90, tiltPos = 90; 

// variables for motors
int speed = 100; // speedvalue form 0-255


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(52, 53, 50, 51, 48, 49);

// intialize sonic sensor (trigger_pin, echo_pin, max_distance_cm)
NewPing sonar(31, 30, 200);

Servo panservo;  // create panservo;
Servo tiltservo; // create tilt servo;


void setup() {

  pinMode(lcdPowerPin, OUTPUT);
  
  // Begin serial connection
  Serial.begin(9600);
  
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  panservo.attach(33);  // attaches the panservo on pin  33 to the servo object,
  tiltservo.attach(32);  // attaches the tiltservo on pin 32 to the servo object

 // Intialize motors
  //Setup Channel A
  pinMode(leftMotorDirPin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(leftMotorBrakePin, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(rightMotorDirPin, OUTPUT); //Initiates Motor Channel B pin
  pinMode(rightMotorBrakePin, OUTPUT);  //Initiates Brake Channel B pin

  turnOnLcd();



}

void loop() {

   
    
    oldState = state;
        
    listenCommand(); // check if there is new command in serial connection
    readDistance();  // read sonic sensor
    writeLcd();      // write thingies on LCD dont care if it is on or off
    panAndTilt();    // move camera to right position

   checkMovements(); // check if there is movement that needs to be stopped

   writeLcd();      // write thingies on LCD dont care if it is on or off

 Serial.println(distance);
 
  


  


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


// *** return state via serial ***
void tellState() {
  Serial.println(state);
}

// *** evaluate command ***
void evaluateCommand(String c){
  
  // reject command if it is too short
  if (c.length()<6){
    command = "";
    return;
  } 
  
  // parse [xxxx...] xx:s from command to define action
  String action = "";
  action += c[1];
  action += c[2];
  action += c[3];
  action += c[4];
  
  Serial.println(action);
  
// move forward 
  if (action == "MOVE"){
   
    state = "MOVE";
    moveTime(3000);
    moveForward(150); // start moving forward
  }

 // reverse
  if (action == "RVRS"){
    state = "REVERSE";
    moveTime(3000);
    moveBackward(150);
  }

// turn right
  if (action == "TURI"){
    state = "TURN RIGHT";
    moveTime(3000);
    turnRight(150);
  }

// turn left
  if (action == "TULE"){
    state = "TURN LEFT";
    moveTime(3000);
    turnLeft(150);
  }

  // stop movement
  if (action == "STOP"){
    moveStop();
    state = "STOP";
  }

// tilt webcam up
  if (action == "TIUP"){
    if(tiltPos >= 60){
      tiltPos -= 10;
    }
  }

// tilt webcam down
  if (action == "TIDW"){
    if(tiltPos <= 170){
      tiltPos += 10;
    }
  }
 // Rotate webcam to left
  if (action == "PALE"){
    if(panPos >= 30){
      panPos -= 30;
    }
  }

  // Rotate webcam to right
  if (action == "PARI"){
    if(panPos <= 150){
      panPos += 30;
    }
  }

  // center webcam 
  if (action == "CEWE"){
    panPos = 90;
    tiltPos = 90;
    }
  

  
  if (action == "TS"){
    lcd.setCursor(0,1);
    lcd.print(c[0]);
    tellState();
  }  
  
  // clear command 
  command = "";
  
}

// *** WRITE LCD ***

void writeLcd(){

    if (millis()-lastLcdWrite > 500){ 

  

      lastLcdWrite = millis();
    
      lcd.clear();
      
      lcd.setCursor(0, 0);
      lcd.print(message);
  
      lcd.setCursor(0, 1);
      lcd.print("DIST:");
        if(distance > 200 ){
          lcd.print("--");
      } else {
          lcd.print(distance);
       }
    
  }
}

void readDistance(){

  if (millis()-lastSonic > 200){
    lastSonic = millis();
    distance = sonar.ping_cm();
  }
  lastDistance = distance;
}

void panAndTilt(){

   
      panservo.write(panPos);       // tell servo to go to position in variable 'pos'
      tiltservo.write(tiltPos);      // tell servo to go to position in variable 'pos
    
  }

void turnOnLcd(){

     digitalWrite(lcdPowerPin, HIGH); // power on lcd 
     LiquidCrystal lcd(52, 53, 50, 51, 48, 49);
     lcd.begin(16, 2);
  
  }

  void turnOffLcd(){

     digitalWrite(lcdPowerPin, LOW); // shut down lcd during movement  
    
    }


// leftMotorDirPin = 12;
// leftMotorBrakePin = 9;
// leftMotorPowerPin = 3;
// rightMotorDirPin = 13;
// rightMotorBrakePin = 8;
// rightMotorPowerPin = 11;

  void moveForward(int s){

     turnOffLcd();

     digitalWrite(leftMotorDirPin, HIGH);  //Establishes forward 
     digitalWrite(leftMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(leftMotorPowerPin, s);    //Spins motor 

     digitalWrite(rightMotorDirPin, LOW);  //Establishes forward 
     digitalWrite(rightMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(rightMotorPowerPin, s);    //Spins motor 
     
    }

   void moveBackward(int s){

     turnOffLcd();

     digitalWrite(leftMotorDirPin, LOW);  //Establishes backward 
     digitalWrite(leftMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(leftMotorPowerPin, s);    //Spins motor 

     digitalWrite(rightMotorDirPin, HIGH);  //Establishes backward;
     digitalWrite(rightMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(rightMotorPowerPin, s);    //Spins motor 

    
    }

void turnRight(int s){

     turnOffLcd();

     digitalWrite(leftMotorDirPin, HIGH);  //Establishes backward 
     digitalWrite(leftMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(leftMotorPowerPin, s);    //Spins motor 

     digitalWrite(rightMotorDirPin, HIGH);  //Establishes backward;
     digitalWrite(rightMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(rightMotorPowerPin, s);    //Spins motor 

    
    }

void turnLeft(int s){

     turnOffLcd();

     digitalWrite(leftMotorDirPin, LOW);  //Establishes backward 
     digitalWrite(leftMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(leftMotorPowerPin, s);    //Spins motor 

     digitalWrite(rightMotorDirPin, LOW);  //Establishes backward;
     digitalWrite(rightMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(rightMotorPowerPin, s);    //Spins motor 

    
    }    

    void moveStop(){

     turnOnLcd();

     digitalWrite(leftMotorDirPin, LOW);  //Establishes backward 
     digitalWrite(leftMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(leftMotorPowerPin, 0);    //Spins motor 

     digitalWrite(rightMotorDirPin, LOW);  //Establishes backward;
     digitalWrite(rightMotorBrakePin, LOW);   //Disengage the Brake
     analogWrite(rightMotorPowerPin, 0);    //Spins motor 

    digitalWrite(leftMotorBrakePin, LOW);  //DIS Engage the Brake for Channel A
    digitalWrite(rightMotorBrakePin, LOW);  //DIS Engage the Brake for Channel B

    
    }

    void moveTime(int movementDuration){

        movementStopTime =millis()+movementDuration;
      
   }

   void checkMovements() {
    if (distance < 10 && state == "MOVE"){
        moveStop();
        }

    if (movementStopTime <= millis() && movementStopTime != 0){
        moveStop();
        movementStopTime = 0;
    }
    
    }

      

    

