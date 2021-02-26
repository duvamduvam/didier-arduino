// Adafruit_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "face.h"

// Choose your prefered pixmap
//#include "heart24.h"
//#include "yellowsmiley24.h"
//#include "bluesmiley24.h"

//#include "smileytongue24.h"
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN 5
#define EARTH_PIN 6

// ESP8266 has an I2S neopixel library which can only use pin RX
// so it's recommended to use the same pin with Neopixel to avoid
// rewiring when changing libs
#ifdef ESP8266
#define PIN RX
#endif

//#define P32BY8X4
#define P16BY16X4
#if defined(P32BY8X4) || defined(P16BY16X4)
#define BM32
#endif

#ifdef BM32
//#include "google32.h"
// Anything with black does not look so good with the naked eye (better on pictures)
#include "linux32.h"
#endif

// Max is 255, 32 is a conservative value to not overload
// a USB power supply (500mA) for 12x12 pixels.
#define BRIGHTNESS 5


#define mw 24
#define mh 16
Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(8, 8, 3 , 2, PIN,
    NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
    NEO_GRB            + NEO_KHZ800);

Adafruit_NeoMatrix *hearth = new Adafruit_NeoMatrix(8, 8, EARTH_PIN,
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
    NEO_GRB            + NEO_KHZ800);

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK		0

#define LED_RED_VERYLOW 	(3 <<  11)
#define LED_RED_LOW 		(7 <<  11)
#define LED_RED_MEDIUM 		(15 << 11)
#define LED_RED_HIGH 		(31 << 11)

#define LED_GREEN_VERYLOW	(1 <<  5)
#define LED_GREEN_LOW 		(15 << 5)
#define LED_GREEN_MEDIUM 	(31 << 5)
#define LED_GREEN_HIGH 		(63 << 5)

#define LED_BLUE_VERYLOW	3
#define LED_BLUE_LOW 		7
#define LED_BLUE_MEDIUM 	15
#define LED_BLUE_HIGH 		31

#define LED_ORANGE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW		(LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW	(LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW		(LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM	(LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH		(LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW	(LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW		(LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM		(LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH		(LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW		(LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)


// Convert a BGR 4/4/4 bitmap to RGB 5/6/5 used by Adafruit_GFX
void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
  // work around "a15 cannot be used in asm here" compiler bug when using an array on ESP8266
  // uint16_t RGB_bmp_fixed[w * h];
  static uint16_t *RGB_bmp_fixed = (uint16_t *) malloc( w * h * 2);
  for (uint16_t pixel = 0; pixel < w * h; pixel++) {
    uint8_t r, g, b;
    uint16_t color = pgm_read_word(bitmap + pixel);

    //Serial.print(color, HEX);
    b = (color & 0xF00) >> 8;
    g = (color & 0x0F0) >> 4;
    r = color & 0x00F;
    //Serial.print(" ");
    //Serial.print(b);
    //Serial.print("/");
    //Serial.print(g);
    //Serial.print("/");
    //Serial.print(r);
    //Serial.print(" -> ");
    // expand from 4/4/4 bits per color to 5/6/5
    b = map(b, 0, 15, 0, 31);
    g = map(g, 0, 15, 0, 63);
    r = map(r, 0, 15, 0, 31);
    //Serial.print(r);
    //Serial.print("/");
    //Serial.print(g);
    //Serial.print("/");
    //Serial.print(b);
    RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
    //Serial.print(" -> ");
    //Serial.println(RGB_bmp_fixed[pixel], HEX);
  }
  matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}

// In a case of a tile of neomatrices, this test is helpful to make sure that the
// pixels are all in sequence (to check your wiring order and the tile options you
// gave to the constructor).
void count_pixels() {
  matrix->clear();
  for (uint16_t i = 0; i < mh; i++) {
    for (uint16_t j = 0; j < mw; j++) {
      matrix->drawPixel(j, i, i % 3 == 0 ? LED_BLUE_HIGH : i % 3 == 1 ? LED_RED_HIGH : LED_GREEN_HIGH);
      // depending on the matrix size, it's too slow to display each pixel, so
      // make the scan init faster. This will however be too fast on a small matrix.
      if (!(j % 7)) matrix->show();
      yield();
    }
  }
}

// Fill the screen with multiple levels of white to gauge the quality
void display_four_white() {
  matrix->clear();
  matrix->fillRect(0, 0, mw, mh, LED_WHITE_HIGH);
  matrix->drawRect(1, 1, mw - 2, mh - 2, LED_WHITE_MEDIUM);
  matrix->drawRect(2, 2, mw - 4, mh - 4, LED_WHITE_LOW);
  matrix->drawRect(3, 3, mw - 6, mh - 6, LED_WHITE_VERYLOW);
  matrix->show();
}

/*void display_bitmap(uint8_t bmp_num, uint16_t color) {
  static uint16_t bmx, bmy;

  // Clear the space under the bitmap that will be drawn as
  // drawing a single color pixmap does not write over pixels
  // that are nul, and leaves the data that was underneath
  matrix->fillRect(bmx, bmy, bmx + 8, bmy + 8, LED_BLACK);
  matrix->drawBitmap(bmx, bmy, mono_bmp[bmp_num], 8, 8, color);
  bmx += 8;
  if (bmx >= mw) bmx = 0;
  if (!bmx) bmy += 8;
  if (bmy >= mh) bmy = 0;
  matrix->show();
}*/

/*void display_rgbBitmap(uint8_t bmp_num) {
  static uint16_t bmx, bmy;

  fixdrawRGBBitmap(bmx, bmy, RGB_bmp[bmp_num], 8, 8);
  bmx += 8;
  if (bmx >= mw) bmx = 0;
  if (!bmx) bmy += 8;
  if (bmy >= mh) bmy = 0;
  matrix->show();
}*/


// Scroll within big bitmap so that all if it becomes visible or bounce a small one.
// If the bitmap is bigger in one dimension and smaller in the other one, it will
// be both panned and bounced in the appropriate dimensions.
void display_panOrBounceBitmap (uint8_t bitmapSize) {
  // keep integer math, deal with values 16 times too big
  // start by showing upper left of big bitmap or centering if the display is big
  int16_t xf = max(0, (mw - bitmapSize) / 2) << 4;
  int16_t yf = max(0, (mh - bitmapSize) / 2) << 4;
  // scroll speed in 1/16th
  int16_t xfc = 6;
  int16_t yfc = 3;
  // scroll down and right by moving upper left corner off screen
  // more up and left (which means negative numbers)
  int16_t xfdir = -1;
  int16_t yfdir = -1;

  for (uint16_t i = 1; i < 200; i++) {
    bool updDir = false;

    // Get actual x/y by dividing by 16.
    int16_t x = xf >> 4;
    int16_t y = yf >> 4;

    matrix->clear();
    // bounce 8x8 tri color smiley face around the screen
    // if (bitmapSize == 8) fixdrawRGBBitmap(x, y, RGB_bmp[10], 8, 8);
    // pan 24x24 pixmap
    if (bitmapSize == 24) matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap24, bitmapSize, bitmapSize);
#ifdef BM32
    if (bitmapSize == 32) matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap32, bitmapSize, bitmapSize);
#endif
    matrix->show();

    // Only pan if the display size is smaller than the pixmap
    // but not if the difference is too small or it'll look bad.
    if (bitmapSize - mw > 2) {
      xf += xfc * xfdir;
      if (xf >= 0)                      {
        xfdir = -1;
        updDir = true ;
      };
      // we don't go negative past right corner, go back positive
      if (xf <= ((mw - bitmapSize) << 4)) {
        xfdir = 1;
        updDir = true ;
      };
    }
    if (bitmapSize - mh > 2) {
      yf += yfc * yfdir;
      // we shouldn't display past left corner, reverse direction.
      if (yf >= 0)                      {
        yfdir = -1;
        updDir = true ;
      };
      if (yf <= ((mh - bitmapSize) << 4)) {
        yfdir = 1;
        updDir = true ;
      };
    }
    // only bounce a pixmap if it's smaller than the display size
    if (mw > bitmapSize) {
      xf += xfc * xfdir;
      // Deal with bouncing off the 'walls'
      if (xf >= (mw - bitmapSize) << 4) {
        xfdir = -1;
        updDir = true ;
      };
      if (xf <= 0)           {
        xfdir =  1;
        updDir = true ;
      };
    }
    if (mh > bitmapSize) {
      yf += yfc * yfdir;
      if (yf >= (mh - bitmapSize) << 4) {
        yfdir = -1;
        updDir = true ;
      };
      if (yf <= 0)           {
        yfdir =  1;
        updDir = true ;
      };
    }

    if (updDir) {
      // Add -1, 0 or 1 but bind result to 1 to 1.
      // Let's take 3 is a minimum speed, otherwise it's too slow.
      xfc = constrain(xfc + random(-1, 2), 3, 16);
      yfc = constrain(xfc + random(-1, 2), 3, 16);
    }
    delay(10);
  }
}

Face face;

void loop() {

  face.execute();

}

void setup() {
  Serial.begin(115200);
  face.loadSequence(0);
}

// vim:sts=4:sw=4
