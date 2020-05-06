
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

//Touchscreen X+ X- Y+ Y- pins
#define YP A0  // must be an analog pin, use "An" notation!
#define XM A1  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

boolean RecordOn = false;

#define FRAME_X 0
#define FRAME_Y 50
#define FRAME_W 75
#define FRAME_H 75

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H


#define FRAME_X2 0
#define FRAME_Y2 150
#define FRAME_W2 75
#define FRAME_H2 75

#define REDBUTTON_X2 FRAME_X2
#define REDBUTTON_Y2 FRAME_Y2
#define REDBUTTON_W2 (FRAME_W2/2)
#define REDBUTTON_H2 FRAME_H2

#define GREENBUTTON_X2 (REDBUTTON_X2 + REDBUTTON_W2)
#define GREENBUTTON_Y2 FRAME_Y2
#define GREENBUTTON_W2 (FRAME_W2/2)
#define GREENBUTTON_H2 FRAME_H2

void drawFrame()
{
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
  tft.drawRect(FRAME_X2, FRAME_Y2, FRAME_W2, FRAME_H2, ILI9341_BLACK);
}

void redBtn()
{ 
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("ON");
  RecordOn = false;
}

void redBtn2()
{ 
  tft.fillRect(REDBUTTON_X2, REDBUTTON_Y2, REDBUTTON_W2, REDBUTTON_H2, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X2, GREENBUTTON_Y2, GREENBUTTON_W2, GREENBUTTON_H2, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(GREENBUTTON_X2 + 6 , GREENBUTTON_Y2 + (GREENBUTTON_H2/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Ft");
  RecordOn = false;
}
void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("OFF");
  RecordOn = true;
}

void greenBtn2()
{
  tft.fillRect(GREENBUTTON_X2, GREENBUTTON_Y2, GREENBUTTON_W2, GREENBUTTON_H2, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X2, REDBUTTON_Y2, REDBUTTON_W2, REDBUTTON_H2, ILI9341_BLUE);
  drawFrame();
  tft.setCursor(REDBUTTON_X2 + 6 , REDBUTTON_Y2 + (REDBUTTON_H2/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("M");
  RecordOn = true;
}

void setup(void)
{
  Serial.begin(9600);
  tft.begin();
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("HiLife Prototype");
  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(0); 
  redBtn();
  redBtn2();
  
}

void loop()
{
   // Retrieve a point  
  TSPoint p = ts.getPoint();

  // See if there's any  touch data for us
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {   
    // Scale using the calibration #'s
    // and rotate coordinate system
    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
    p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
    int y = p.y;
    int x = p.x;

    if (RecordOn)
    {
      if((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Red btn hit"); 
          redBtn();
        }
      }
            if((x > REDBUTTON_X2) && (x < (REDBUTTON_X2 + REDBUTTON_W2))) {
        if ((y > REDBUTTON_Y2) && (y <= (REDBUTTON_Y2 + REDBUTTON_H2))) {
          Serial.println("Red btn2 hit"); 
          redBtn2();
        }
      }
    }
    else //Record is off (RecordOn == false)
    {
      if((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Green btn hit"); 
          greenBtn();
        }
      }
            if((x > GREENBUTTON_X2) && (x < (GREENBUTTON_X2 + GREENBUTTON_W2))) {
        if ((y > GREENBUTTON_Y2) && (y <= (GREENBUTTON_Y2 + GREENBUTTON_H2))) {
          Serial.println("Green btn2 hit"); 
          greenBtn2();
        }
      }
    }
    Serial.println(RecordOn);
  }  
}
