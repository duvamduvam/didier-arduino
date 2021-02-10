#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "Visuals.h"
#include "ArduinoLog.h"

class Face {

  private :

    const byte BRIGHTNESS = 5;
    const byte MOUTH_PIN = 13;
    const byte L_EYE_PIN = 12;
    const byte R_EYE_PIN = 11;
    const byte HEART_PIN = 10;

    const byte MOUTH = 1;
    const byte L_EYE = 2;
    const byte R_EYE = 3;
    const byte HEART = 4;

    struct BmpTime {
      byte nb; // bmp number
      long t; // duration
    };

    struct Sequence {
      long seq_time;
      byte mouth_size;
      BmpTime* mouth;
      byte heart_size;
      BmpTime* heart;
      byte r_eye_size;
      BmpTime* r_eye;
      byte l_eye_size;
      BmpTime* l_eye;
    };

    Adafruit_NeoMatrix *mouth = new Adafruit_NeoMatrix(8, 8, 3 , 2, MOUTH_PIN,
        NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    Adafruit_NeoMatrix *heart = new Adafruit_NeoMatrix(8, 8, HEART_PIN,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    Adafruit_NeoMatrix *l_eye = new Adafruit_NeoMatrix(8, 8, L_EYE_PIN,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    Adafruit_NeoMatrix *r_eye = new Adafruit_NeoMatrix(8, 8, R_EYE_PIN,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    boolean started = false;

    long seq_time;

    const byte default_seq = 0;
    byte current_seq_nb = 0;

    //part face time
    long mouth_t;
    long heart_t;
    long r_eye_t;
    long l_eye_t;

    //part face position
    byte mouth_p;
    byte heart_p;
    byte r_eye_p;
    byte l_eye_p;

    Sequence current_seq;

    BmpTime mouthTest[7] = {{0, 200}, {1, 1000}, {2, 200}, {3, 300}, {2, 300}, {1, 300}, {0, 300}} ;
    BmpTime mouthTest2[7] = {{0, 300}, {1, 300}, {2, 300}, {3, 300}, {2, 300}, {1, 300}, {0, 300}} ;
    BmpTime eyesTest[7] = {{10, 1000}, {1, 100}, {2, 200} } ;
    BmpTime heartTest[7] = {{4, 100}, {5, 200}, {6, 200} } ;

    Sequence sequences[3] = {
      {5000, sizeof(mouthTest) / sizeof(mouthTest[0]), mouthTest, sizeof(heartTest) / sizeof(heartTest[0]), heartTest, sizeof(eyesTest) / sizeof(eyesTest[0]), eyesTest, sizeof(eyesTest) / sizeof(eyesTest[0]), eyesTest},
      //{7000, mouthTest2, heartTest, eyesTest, eyesTest},
      //{3000, mouthTest, heartTest, eyesTest, eyesTest}
    };

    void initMatrix(Adafruit_NeoMatrix *matrix) {
      matrix->begin();
      matrix->clear();
      matrix->setBrightness(BRIGHTNESS);
    }

  public :

    Face() {
      initMatrix(mouth);
      initMatrix(l_eye);
      initMatrix(r_eye);
      initMatrix(heart);
      loadSequence(default_seq);

    }

    void process(byte new_seq) {
      if (current_seq_nb != new_seq) {
        current_seq_nb = new_seq;
        loadSequence(current_seq_nb);
      }
    }

    long loadPart(Adafruit_NeoMatrix *part, uint16_t bitmap[] , long part_time, int w, int h) {
      //cur_time = millis() + part_time;
      part->drawRGBBitmap(0, 0, bitmap, w, h);
      part->show();
      return millis() + part_time;
    }

    void loadSequence(byte seq_nb) {


      Log.notice("loadSequence millis():%d seq_nb:%d seq_time:%d\n", millis(), seq_nb, seq_time);

      current_seq = sequences[seq_nb];
      seq_time = millis() + sequences[seq_nb].seq_time;

      BmpTime bmptime = current_seq.mouth[0];
      mouth_t = loadPart(mouth, bitmap24[bmptime.nb],  bmptime.t,  24, 16);

      bmptime = current_seq.l_eye[0];
      l_eye_t = loadPart(l_eye, bitmap8[bmptime.nb],  bmptime.t,  8, 8);

      bmptime = current_seq.r_eye[0];
      r_eye_t = loadPart(r_eye, bitmap8[bmptime.nb],  bmptime.t,  8, 8);

      bmptime = current_seq.heart[0];
      heart_t = loadPart(heart, bitmap8[bmptime.nb],  bmptime.t,  8, 8);

    }

    void execute() {

      //Log.notice("execute millis():%d seq_time:%d mouth_t:%d\n", millis(), seq_time, mouth_t);

      if (millis() > seq_time) {
        loadSequence(default_seq);
      } else {
        if (millis() > mouth_t) {
          Log.notice("mouth next current_seq.mouth_size:%d mouth_p:%d\n", current_seq.mouth_size, mouth_p);
          if (current_seq.mouth_size > mouth_p + 1) {
            mouth_p++;
          } else {
            mouth_p = 0;
          }
          BmpTime bmptime = current_seq.mouth[mouth_p];
          mouth_t = loadPart(mouth, bitmap24[bmptime.nb], bmptime.t, 24, 16);
        }
        if (millis() > l_eye_t) {
          if (current_seq.l_eye_size > l_eye_p + 1) {
            l_eye_p++;
          } else {
            l_eye_p = 0;
          }
          BmpTime bmptime = current_seq.l_eye[l_eye_p];
          l_eye_t = loadPart(l_eye, bitmap8[bmptime.nb], bmptime.t, 8, 8);
        }

        if (millis() > r_eye_t) {
          if (current_seq.r_eye_size > r_eye_p + 1) {
            r_eye_p++;
          } else {
            r_eye_p = 0;
          }
          BmpTime bmptime = current_seq.l_eye[r_eye_p];
          r_eye_t = loadPart(r_eye, bitmap8[bmptime.nb], bmptime.t, 8, 8);
        }

        if (millis() > heart_t) {
          if (current_seq.heart_size > heart_p + 1) {
            heart_p++;
          } else {
            heart_p = 0;
          }
          BmpTime bmptime = current_seq.heart[heart_p];
          heart_t = loadPart(heart, bitmap8[bmptime.nb], bmptime.t, 8, 8);
        }
      }
    }

};