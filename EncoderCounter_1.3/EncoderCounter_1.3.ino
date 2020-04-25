/*
Shurjo Maitra
Highlight Industries
HiLife Encoder Project Phase 1
Started on 4/1/2020

TODO
PPR - integer
Read protect arduino file
Circumference - distance per pulse calculation
display 1 decimal place
Feet and meters switch button
*/

/** NOT FOR USE WITH THE TOUCH SHIELD, ONLY FOR THE BREAKOUT! **/

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"

// These are the four touchscreen analog pins
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 7   // can be any digital pin
#define XP 6   // can be any digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 100
#define PENRADIUS 3
int oldcolor, currentcolor;


// this constant won't change:
const int encoderPin = 0; // the pin that the pushbutton is attached to
const int ledPin = 13; // the pin that the LED is attached to
const int resetPin = 2;
const int onOffPin = 3;
// 
// Variables will change:
int encoderPulseCounter = 0; // counter for the number of button presses
int encoderState = 0; // current state of the button
int lastencoderState = 0; // previous state of the button
int resetState = 0; // current state of reset button
int onOffState = 0; // current state of on/off button


void setup()
{
	// initialize the button pin as a input:
	pinMode(encoderPin, INPUT);
	// initialize the LED as an output:
	pinMode(ledPin, OUTPUT);
	// initialize reset button as an input
	pinMode(resetPin, INPUT);
	//initialize on/off button as an input
	pinMode(onOffPin, INPUT);
	// initialize serial communication:
	Serial.begin(9600);

  Serial.println(F("HiLife Touchscreen!"));

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);

  // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
  currentcolor = ILI9341_RED;
  
}

void loop()
{
	// read the encoder pulses through input pin:
	encoderState = digitalRead(encoderPin);
	// read reset signal
	resetState = digitalRead(resetPin);
	// read on/off signal
	onOffState = digitalRead(onOffPin);
	// compare the encoderState to its previous state
	if (encoderState != lastencoderState)
	{
		// if the state has changed, increment the counter
		if (encoderState == HIGH)
		{
			// if the current state is HIGH then the button
			// wend from off to on:
			encoderPulseCounter++;
			if (encoderPulseCounter % 10 == 0)
			{
				Serial.println("on");
				Serial.print("number of revolutions:  ");
				Serial.println(encoderPulseCounter /10);
			}
		}
		else
		{
			// if the current state is LOW then the button
			// wend from on to off:
			Serial.println("off");
		}
	}
	if (resetState == HIGH)
	{
		encoderPulseCounter = 0;
		Serial.println("Counter reset");
	}
	if (onOffState == HIGH)
	{
		Serial.println("Encoder on");
	}
	// save the current state as the last state,
	// for next time through the loop
	lastencoderState = encoderState;
	// turns on the LED every 10 pulse by
	// checking the modulo of the button pulse counter.
	// the modulo function gives you the remainder of
	// the division of two numbers:
	if (encoderPulseCounter % 10 == 0)
	{
		digitalWrite(ledPin, HIGH);
	}
	else
	{
		digitalWrite(ledPin, LOW);
	}
}
