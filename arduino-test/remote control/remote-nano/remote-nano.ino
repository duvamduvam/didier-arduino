#include <Keypad.h>
#include "U8glib.h"
#include "ArduinoLog.h"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // I2C / TWI

const int MIC_pin = A0;
const int JOY_X = A6; // analog pin connected to X output
const int JOY_Y = A7; // analog pin connected to Y output

const int AXIS_X = 1;                  // x-axis of the accelerometer
const int AXIS_Y = 2;                  // y-axis
const int AXIS_Z = 3;              // z-axis (only on 3-axis models)

const int VIBRATION = 13;

const byte Joy_margin = 10;

const byte ROWS = 3;
const byte COLS = 8;
char keys[ROWS][COLS] = {
  {'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X'},
  {'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'},
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},
};
byte rowPins[ROWS] = {12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6, 5, 4, 3, 2};

char key;
char mod = 'X';
//char radioMsg[4];

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

static char current_key;

void setup() {
  Serial.begin(9600);
  Log.begin   (LOG_LEVEL, &Serial);
  pinMode(VIBRATION, OUTPUT);
}

void printButton() {
  Serial.print('<');
  Serial.print(mod);
  Serial.print(key);
  Serial.print('>');
}

void updateMod() {
  if (key >= 85) {
    mod = key;
  }
}

void vibration() {
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5)
  {
    // sets the value (range from 0 to 255):
    analogWrite(VIBRATION, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5)
  {
    // sets the value (range from 0 to 255):
    analogWrite(VIBRATION, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
  }
}

void joystick() {
  byte x = 255 - (analogRead(JOY_X) / 4);
  byte y = analogRead(JOY_Y) / 4;

  char m = Joy_margin;
  if (x > 128 + m || x < 128 - m || y > 128 + m || y < 128 - m) {
    Serial.print('<');
    Serial.print('J');
    Serial.print((char)x);
    Serial.print((char)y);
    Serial.print('>');
    Log.notice("Joystick x:%d y:%d\n", x, y);
  }

}

void axis() {
  int x = analogRead(AXIS_X);
  int y = analogRead(AXIS_Y);
  int z = analogRead(AXIS_Z) / 4;
  //Log.notice("3 axis x:%d y:%d z:%d\n", x, y, z);

  Serial.print('X');
  Serial.print(x);
  Serial.print('Y');
  Serial.print(y);
  Serial.print('Z');
  Serial.print(z);

  char m = Joy_margin;
  if (x > 128 + m || x < 128 - m || y > 128 + m || y < 128 - m)
  {
    // printJoystick(x, y);
  }
}

void micro() {
  byte mic = analogRead(MIC_pin) / 4;
  Log.notice("micro level : %d\n", mic);
}

void screen() {
  // picture loop
  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );
}

void drawColorBox(void)
{
  u8g_uint_t w, h;
  u8g_uint_t r, g, b;

  w = u8g.getWidth() / 32;
  h = u8g.getHeight() / 8;
  for ( b = 0; b < 4; b++ )
    for ( g = 0; g < 8; g++ )
      for ( r = 0; r < 8; r++ )
      {
        u8g.setColorIndex((r << 5) |  (g << 2) | b );
        u8g.drawBox(g * w + b * w * 8, r * h, w, h);
      }
}

void drawLogo(uint8_t d)
{
#ifdef MINI_LOGO
  u8g.setFont(u8g_font_gdr17r);
  u8g.drawStr(0 + d, 22 + d, "U");
  u8g.setFont(u8g_font_gdr20n);
  u8g.drawStr90(17 + d, 8 + d, "8");
  u8g.setFont(u8g_font_gdr17r);
  u8g.drawStr(39 + d, 22 + d, "g");

  u8g.drawHLine(2 + d, 25 + d, 34);
  u8g.drawVLine(32 + d, 22 + d, 12);
#else
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(0 + d, 30 + d, "U");
  u8g.setFont(u8g_font_gdr30n);
  u8g.drawStr90(23 + d, 10 + d, "8");
  u8g.setFont(u8g_font_gdr25r);
  u8g.drawStr(53 + d, 30 + d, "g");

  u8g.drawHLine(2 + d, 35 + d, 47);
  u8g.drawVLine(45 + d, 32 + d, 12);
#endif
}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g.setFont(u8g_font_4x6);
  if ( u8g.getHeight() < 59 )
  {
    u8g.drawStr(53, 9, "code.google.com");
    u8g.drawStr(77, 18, "/p/u8glib");
  }
  else
  {
    u8g.drawStr(1, 54, "code.google.com/p/u8glib");
  }
#endif
}


void draw(void) {
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    drawColorBox();
  }
  u8g.setColorIndex(1);
  if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g.getMode()) > 1 ) {
    drawLogo(2);
    u8g.setColorIndex(2);
    drawLogo(1);
    u8g.setColorIndex(3);
  }
  drawLogo(0);
  drawURL();

}

void loop() {
  char in = keypad.getKey();

  if (in != NO_KEY) {
    key = in;
  }

  if (keypad.getState() > 0) {
    updateMod();
    printButton();
  }

  joystick();
  //axis();
  //vibration();
  screen();

  delay(5);
}
