/*
  State change detection (edge detection)
 	
 Often, you don't need to know the state of a digital input all the time,
 but you just need to know when the input changes from one state to another.
 For example, you want to know when a button goes from OFF to ON.  This is called
 state change detection, or edge detection.
 
 This example shows how to detect when a button or button changes from off to on
 and on to off.
 
 This example code is in the public domain.
 	
 */

// this constant won't change:
const int  encoderPin = 0;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to
const int resetPin = 2;

// Variables will change:
int encoderPulseCounter = 0;   // counter for the number of button presses
int encoderState = 0;         // current state of the button
int lastencoderState = 0;     // previous state of the button
int resetState = 0;

void setup() {
  // initialize the button pin as a input:
  pinMode(encoderPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(resetPin, INPUT);
  // initialize serial communication:
  Serial.begin(9600);

}

void loop() {
  // read the pushbutton input pin:
  encoderState = digitalRead(encoderPin);
  resetState = digitalRead(resetPin);
  // compare the encoderState to its previous state
  if (encoderState != lastencoderState) {
    // if the state has changed, increment the counter
    if (encoderState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      encoderPulseCounter++;
      if (encoderPulseCounter % 10 == 0) {
        Serial.println("on");
        Serial.print("number of revolutions:  ");
        Serial.println(encoderPulseCounter /10);
      } 
    }
    else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off"); 
    }
  }
  if (resetState == HIGH) {
    encoderPulseCounter = 0;
  }
  // save the current state as the last state, 
  //for next time through the loop
  lastencoderState = encoderState;


  // turns on the LED every 10 pulse by 
  // checking the modulo of the button pulse counter.
  // the modulo function gives you the remainder of 
  // the division of two numbers:
  if (encoderPulseCounter % 10 == 0) {
    digitalWrite(ledPin, HIGH);
  } 
  else {
    digitalWrite(ledPin, LOW);
  }

}

//PPR - integer 
//Read protect arduino file
// Circumference - distance per pulse calculation
//display 1 decimal place
//Feet and meters switch button









