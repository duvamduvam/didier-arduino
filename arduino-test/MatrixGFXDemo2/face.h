#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "visuals.h"
#include "ArduinoLog.h"

class Face {

  private :

    const byte BRIGHTNESS = 5;

    struct BmpTime {
      byte nb; // bmp number
      int t; // duration
    };

    struct Sequence {
      int seq_time;
      BmpTime* mouth;
      BmpTime* heart;
      BmpTime* r_eye;
      BmpTime* l_eye;
    };

    Adafruit_NeoMatrix *mouth = new Adafruit_NeoMatrix(8, 8, 3 , 2, 6,
        NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    Adafruit_NeoMatrix *heart = new Adafruit_NeoMatrix(8, 8, 5,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    Adafruit_NeoMatrix *l_eye = new Adafruit_NeoMatrix(8, 8, 2,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    Adafruit_NeoMatrix *r_eye = new Adafruit_NeoMatrix(8, 8, 3,
        NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);

    boolean started = false;

    long seq_time;

    const byte default_seq = 0;
    byte current_seq = 0;

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

    Sequence sequence;

    BmpTime mouthTest[7] = {{0, 300}, {1, 300}, {2, 300}, {3, 300}, {2, 300}, {1, 300}, {0, 300}} ;
    BmpTime eyesTest[7] = {{0, 300}, {1, 100}, {2, 200} } ;
    BmpTime heartTest[7] = {{4, 100}, {5, 200}, {6, 200} } ;

    Sequence sequences[3] = {
      {5000, mouthTest, heartTest, eyesTest, eyesTest},
      {7000, mouthTest, heartTest, eyesTest, eyesTest},
      {3000, mouthTest, heartTest, eyesTest, eyesTest}
    };

    void play24(byte pos) {

    }

    void play8() {

    }

  public :

    Face() {
      mouth->begin();
      mouth->clear();
      mouth->setBrightness(BRIGHTNESS);
      l_eye->begin();
      l_eye->clear();
      l_eye->begin();
      l_eye->clear();
      heart->begin();
      heart->clear();

      loadSequence(default_seq);


    }

    void process(byte new_seq) {
      if (current_seq != new_seq) {
        current_seq = new_seq;
        loadSequence(current_seq);
      }
    }


    void loadSequence(byte seq_nb) {

      seq_time = millis() + sequences[seq_nb].seq_time;

      mouth_t = sequences[seq_nb].mouth->t;
      byte nb = sequences[seq_nb].mouth->nb;
      mouth->drawRGBBitmap(0, 0, bitmap24[nb], 24, 16);
      mouth->show();

      l_eye_t = sequences[seq_nb].l_eye->t;
      nb = sequences[seq_nb].l_eye->nb;
      l_eye->drawRGBBitmap(0, 0, bitmap8[nb], 8, 8);
      l_eye->show();

      r_eye_t = sequences[seq_nb].r_eye->t;
      nb = sequences[seq_nb].r_eye->nb;
      r_eye->drawRGBBitmap(0, 0, bitmap8[nb], 8, 8);
      r_eye->show();

      heart_t = sequences[seq_nb].heart->t;
      nb = sequences[seq_nb].heart->nb;
      heart->drawRGBBitmap(0, 0, bitmap8[nb], 8, 8);
      heart->show();

    }

    boolean hasNext(byte seqSize, byte pos) {
      return (seqSize - 1) > pos;
    }

    void execute() {

      if (millis() > sequence.seq_time) {
        loadSequence(default_seq);
        current_seq = default_seq;
      } else {
        if (millis() > mouth_t) {
          if (hasNext(sizeof(sequence.mouth), mouth_p)) {
            mouth_p++;
            mouth->drawRGBBitmap(0, 0, bitmap24[mouth_p], 24, 16);
            mouth_t = sequence.mouth[mouth_p].t;
          }
        }
        if (millis() > l_eye_t) {
          if (hasNext(sizeof(sequence.l_eye), l_eye_p)) {
            l_eye_p++;
            l_eye->drawRGBBitmap(0, 0, bitmap8[l_eye_p], 8, 8);
            l_eye_t = sequence.l_eye[l_eye_p].t;
          }
        }
        if (millis() > r_eye_t) {
          if (hasNext(sizeof(sequence.r_eye), r_eye_p)) {
            r_eye_p++;
            r_eye->drawRGBBitmap(0, 0, bitmap8[r_eye_p], 8, 8);
            r_eye_t = sequence.l_eye[r_eye_p].t;
          }
        }
        if (millis() > heart_t) {
          if (hasNext(sizeof(sequence.heart), heart_p)) {
            heart_p++;
            heart->drawRGBBitmap(0, 0, bitmap8[heart_p], 8, 8);
            heart_t = sequence.heart[heart_p].t;
          }
        }


      }
    }

};
