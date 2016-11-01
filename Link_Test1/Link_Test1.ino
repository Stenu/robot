

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String state = "alive!";
String oldState = state;


void setup() {
  // Begin serial connection
  Serial.begin(9600);
  
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
 // lcd.print("hello, world!");
}

void loop() {
  
    oldState = state;    

    String command = listenCommand();
    
    
    
    evaluateCommand(command);
    
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


}


// command listener method
String listenCommand() {
  if (Serial.available() > 0) {
    String c = Serial.readString();
    return c;
  } else return "no command";
  
}


// return state via serial
void tellState() {
  Serial.println(state);
}

// evaluate command
void evaluateCommand(String c){
  
  if (c == "liiku"){
    state = "liikutaan";
  }
  
  if (c == "pakita"){
    state = "pakitetaan";
  }
  
  if (c == "kaanny"){
    state = "kaannytaan";
  }
  
  if (c == "pysahdy"){
    state = "pysahdytty";
  }
  
  if (c == "tila"){
    tellState();
  }  
  
}
  
  
 

  



