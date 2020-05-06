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

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

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

//Touchscreen pressure range
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

boolean RecordOn = false;


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define FRAME_X 210
#define FRAME_Y 180
#define FRAME_W 100
#define FRAME_H 50

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}

void redBtn()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("ON");
  RecordOn = false;
}

void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H / 2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("OFF");
  RecordOn = true;
}



// this constant won't change:
const int encoderPin = 0; // the pin that the pushbutton is attached to
const int ledPin = 13; // the pin that the LED is attached to
const int resetPin = 2;
const int onOffPin = 3;

//Calculation stuff
const int PPR = 10;
const int Circumference = 10;
const float pi = 3.14;

// Variables will change:
int encoderPulseCounter = 0; // counter for the number of button presses
int encoderState = 0; // current state of the button
int lastencoderState = 0; // previous state of the button
int resetState = 0; // current state of reset button
int onOffState = 0; // current state of on/off button

int distanceMeters = 0;
int dstianceFeet = 0;
bool FeetMeters = false;



void setup()
{
  pinMode(encoderPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(resetPin, INPUT);
  pinMode(onOffPin, INPUT);

  Serial.begin(9600);

  Serial.println(F("HiLife Touchscreen!"));
  tft.fillScreen(ILI9341_BLACK);
  tft.begin();
  tft.setRotation(1);
  redBtn();

  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  // tft.println("HiLife Film Test V1");

}

void loop()
{
  // Retrieve a point
  TSPoint p = ts.getPoint();

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
    // Scale using the calibration #'s
    // and rotate coordinate system
    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
    p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
    int y = tft.height() - p.x;
    int x = p.y;

    if (RecordOn)
    {
      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red btn hit");
          redBtn();
        }
      }
    }
    else //Record is off (RecordOn == false)
    {
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green btn hit");
          greenBtn();
        }
      }
    }
  }

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
        Serial.println(encoderPulseCounter / 10);

        tft.println(encoderPulseCounter / 10);
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
