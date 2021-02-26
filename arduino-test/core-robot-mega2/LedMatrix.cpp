#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN_MOUTH 5
#define PIN_EYES 6
#define PIN_HEART 7

#define BRIGHTNESS 10

class LedMatrix {

  private :
    Adafruit_NeoMatrix* mouth;
    Adafruit_NeoMatrix* eyes;
    Adafruit_NeoMatrix* heart;

    void drawMatrix(Adafruit_NeoMatrix* matrix, int maxX, int maxY, uint16_t* picture) {

      int pictureSize = sizeof(picture);
      int x, y = 0;

      for (int i = 0; i < pictureSize; i++) {
        if (i = maxX) {
          x = 0;
          y++;
        }

        matrix->drawPixel(x, y, picture[i]);
        x++;
      }
    }

    void matrixInit(Adafruit_NeoMatrix* matrix, int mw, int mh, int pin) {
      matrix = new Adafruit_NeoMatrix(mw, mh, PIN_MOUTH,
                                      NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
                                      NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                      NEO_GRB + NEO_KHZ800);

      matrix->begin();
      matrix->setTextWrap(false);
      matrix->setBrightness(BRIGHTNESS);
    }


    const  uint16_t test[64] = {
      '0xffcc00', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xffcc00',
      '0xcd04f8', '0xffcc00', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xffcc00', '0xcd04f8',
      '0xcd04f8', '0xcd04f8', '0xffcc00', '0xcd04f8', '0xcd04f8', '0xffcc00', '0xcd04f8', '0xcd04f8',
      '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xffcc00', '0xffcc00', '0xcd04f8', '0xcd04f8', '0xcd04f8',
      '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xffcc00', '0xffcc00', '0xcd04f8', '0xcd04f8', '0xcd04f8',
      '0xcd04f8', '0xcd04f8', '0xffcc00', '0xcd04f8', '0xcd04f8', '0xffcc00', '0xcd04f8', '0xcd04f8',
      '0xcd04f8', '0xffcc00', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xffcc00', '0xcd04f8',
      '0xffcc00', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xcd04f8', '0xffcc00'
    };

  public :
    LedMatrix() {

      matrixInit(mouth, 24, 16, PIN_MOUTH);
      matrixInit(eyes, 16, 8, PIN_EYES);
      matrixInit(heart, 8, 8, PIN_HEART);

    }

    void processMouth(uint16_t* in) {
      drawMatrix(mouth, 24, 16, in);
    }

    void processEyes(char* in) {
    }

    void processHeart(char* in) {
    }



};
