/*
  The circuit:
  - LED attached from pin 13 - 11 to ground
  - pushbutton attached to pin 2 - 5 from +5V
  - 10K resistor attached to pin 2 -5 from ground
  - Speaker to pin 8 
  - Servo to pin 9  
*/

#include <Servo.h>
#include "pitches.h"

/*************************************************
 * Public Constants
 *************************************************/
 
// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = 2;     // the number of the pushbutton pin
const int buttonPin2 = 3; 
const int buttonPin3 = 4;
const int buttonPin4 = 5; 
const int ledPin1 =  13;      // the number of the LED pin
const int ledPin2 = 12; 
const int ledPin3 = 11; 
const int ledPin4 = 10; 
const int ENDGAMESIZE = 4;
const int tonePin = 8; 
const int servoPin = 9; 

const int numLEDs = 4; 
const int RED = NOTE_C4; 
const int BLUE = NOTE_D4;
const int YELLOW = NOTE_E4; 
const int GREEN = NOTE_F4; 

int win_melody[] = {
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_GS4, NOTE_AS4, NOTE_C5, 0, NOTE_AS4, NOTE_C5, -1
};

int win_noteDurations[] = {
  8, 8, 8, 4, 4, 4, 8, 8, 8, 1.5
};

int lose_melody[] = {
  NOTE_F3, -1
}; 

int lose_noteDurations[] = {
  1
}; 

void playMelody( int melody[], int notes[] ) {
  for (int thisNote = 0; melody[thisNote] != -1; thisNote++) {
    int noteDuration = 1000 / notes[thisNote];
    tone(tonePin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

Servo myservo;
int pos = 0;

// variables will change:
int randNumber; 
int gameOrder[ENDGAMESIZE+1]; 
int count = 0;
int current = 1; 

int button1State = 0;         // variable for reading the pushbutton status
int button2State = 0; 
int button3State = 0; 
int button4State = 0; 
bool stateChanged1 = false; 
bool stateChanged2 = false; 
bool stateChanged3 = false; 
bool stateChanged4 = false; 
long button1Timer = 0; 
long button2Timer = 0; 
long button3Timer = 0; 
long button4Timer = 0; 
long pressTime = 20; 
bool pressActive = false; 

void generate() {
  Serial.println("new game");
  //flashAll();
  startSequence(); 
  delay (1000); 
  for (int i = 0; i < (ENDGAMESIZE+1); i++) {
    gameOrder[i] = random(1, numLEDs+1); 
    Serial.println(gameOrder[i]); 
  }
//  flash(gameOrder[0]); 
  play(gameOrder[0]);
}

void setup() {
  delay (2000); 

  myservo.attach(servoPin);
  myservo.write(90);
  
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT); 
  pinMode(ledPin3, OUTPUT); 
  pinMode(ledPin4, OUTPUT); 
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT); 
  pinMode(buttonPin3, INPUT); 
  pinMode(buttonPin4, INPUT); 
  
  Serial.begin(9600);

  randomSeed(analogRead(0));  
  generate();
}

void loop() {
  
  // read the state of the pushbutton value:
  button1State = digitalRead(buttonPin1);
  button2State = digitalRead(buttonPin2); 
  button3State = digitalRead(buttonPin3); 
  button4State = digitalRead(buttonPin4); 

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (digitalRead(buttonPin1) == HIGH) {
    // turn LED on:
    if (!stateChanged1) {
      button1Timer = millis();
      stateChanged1 = true; 
    } 
  } 
  else if (digitalRead(buttonPin2) == HIGH) {
    if (!stateChanged2) {
      button2Timer = millis();
      stateChanged2 = true; 
    } 
  } 
  else if (digitalRead(buttonPin3) == HIGH) {
    if (!stateChanged3) {
      button3Timer = millis();
      stateChanged3 = true; 
    } 
  } 
  else if (digitalRead(buttonPin4) == HIGH) {
    if (!stateChanged4) {
      button4Timer = millis();
      stateChanged4 = true; 
    } 
  } 
  else {
    if (stateChanged1) {  
      Serial.println("1 released"); 
      stateChanged1 = false; 
      if (millis() - button1Timer > pressTime && !pressActive) {
        pressActive = true; 
        turnOn1(); 
     } 
    } 
    if (stateChanged2) {
      Serial.println("2 released"); 
      stateChanged2 = false; 
      if (millis() - button2Timer > pressTime && !pressActive) {
        pressActive = true; 
        turnOn2(); 
     } 
    }
    if (stateChanged3) {
      Serial.println("3 released"); 
      stateChanged3 = false; 
      if (millis() - button3Timer > pressTime && !pressActive) {
        pressActive = true; 
        turnOn3(); 
     } 
    }
    if (stateChanged4) {
      Serial.println("4 released"); 
      stateChanged4 = false; 
      if (millis() - button4Timer > pressTime && !pressActive) {
        pressActive = true; 
        turnOn4(); 
     } 
    }
  }
}

//void flash(int input) {
//  play(input); fl
////  if (input == 1) {
////    play(1);
////  }
////  if (input == 2) {
////    play(2);
////  }
////  if (input == 3) {
////    play(3);
////  }
////  if (input == 4) {
////    play(4);
////  }
//}

void startSequence() {
  Serial.println("start");
  play(1); 
  play(2);
  play(3);
  play(4);
  delay(1200); 
}

//signals start and victory of game
void flashAll() {
  Serial.println("flash all"); 
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
//  delay(3000); 
  playMelody(win_melody, win_noteDurations);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

}

//signals wrong input and loss of game
void endSequence() {
  Serial.println("end");
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
//  delay(3000); 
  playMelody(lose_melody, lose_noteDurations); 
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
}

void turnOn1() {
  Serial.println("1 pressed"); 
  play1(); 
  nextInput(1);
  stateChanged1 = false; 
  pressActive = false; 
}

void turnOn2() {
  Serial.println("2 pressed"); 
  play2();
  nextInput(2);
  stateChanged2 = false; 
  pressActive = false; 
}

void turnOn3() {
  Serial.println("3 pressed"); 
  play3();
  nextInput(3);
  stateChanged3 = false; 
  pressActive = false; 
}

void turnOn4() {
  Serial.println("4 pressed"); 
  play4();
  nextInput(4);
  stateChanged4 = false; 
  pressActive = false; 
}

void play1() {
  Serial.println("1 play"); 
  tone(tonePin, RED, 1000);
  digitalWrite(ledPin1, HIGH);
  delay(250); 
  noTone(tonePin);  
  digitalWrite(ledPin1, LOW);
}

void play2() {
  Serial.println("2 play");
  tone(tonePin, BLUE, 1000); 
  digitalWrite(ledPin2, HIGH);
  delay(250); 
  noTone(tonePin);
  digitalWrite(ledPin2, LOW);
}

void play3() {
  Serial.println("3 play");
  tone(tonePin, YELLOW, 1000); 
  digitalWrite(ledPin3, HIGH);
  delay(250); 
  noTone(tonePin);
  digitalWrite(ledPin3, LOW);
}

void play4() {
  Serial.println("4 play");
  tone(tonePin, GREEN, 1000); 
  digitalWrite(ledPin4, HIGH);
  delay(250); 
  noTone(tonePin);
  digitalWrite(ledPin4, LOW);
}



void play(int i) {
  Serial.println("play called"); 
  Serial.println(i); 
  if (i == 1) {
    play1();
  } 
  if (i == 2) {
    play2();
  }
  if (i == 3) {
    play3();
  }
  if (i == 4) {
    play4();
  }
}

//something something something 
void nextInput(int input) {
  // if ended 
  if (count > ENDGAMESIZE) {
    Serial.println("ended");
    //myservo.write(180); 
      //delay(1500);
      myservo.write(90); 
//    if (pos < 90) {
//      Serial.println("close");
//    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//        // in steps of 1 degree
//        myservo.write(pos);              // tell servo to go to position in variable 'pos'
//        delay(15);                       // waits 15ms for the servo to reach the position
//      }
//    }else {
//      myservo.write(90);
//    }

      
    count = 0; 
    current = 1; 
    generate(); 
    return;
  }
  if (input == gameOrder[count] && count <= current) {

    // else: valid input
    Serial.println("valid"); 
//    Serial.println(count); 
//    Serial.println(current); 
    count++;  
    if (count == current) {
      if (current-1 == ENDGAMESIZE) {
      Serial.println("win");
      //end state
      delay(500); 
      flashAll();
      //need better end game signal
      count = ENDGAMESIZE + 1;

      Serial.println("open");
//      for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//        myservo.write(pos);              // tell servo to go to position in variable 'pos'
//        delay(15);                       // waits 15ms for the servo to reach the position
//      }
      //myservo.write(0); 
      //delay(150);
      myservo.write(180); 
      
      return;
    } 
      Serial.println("print pattern");
      delay(1000); 
      current++; 
      count = 0; 
      for (int i = 0; i < (current); i++) {
//        flash(gameOrder[i]);  
        play(gameOrder[i]);  
        delay(1000);
      }
    }
    return;
  }
  else {
    //END GAME
    count = ENDGAMESIZE + 1; 
    Serial.println("lose");
    endSequence();
    return;
    }
}
