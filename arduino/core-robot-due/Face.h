#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "Visuals.h"
#include "ArduinoLog.h"

class Face {

  private :

    const int BRIGHTNESS = 40;

    //const byte MOUTH_PIN = 12;
    //const byte L_EYE_PIN = 10;
    //const byte R_EYE_PIN = 11;
    const byte MOUTH_PIN = 12;
    const byte L_EYE_PIN = 10;  
    const byte R_EYE_PIN = 11;

    const byte MOUTH = 1;
    const byte L_EYE = 2;
    const byte R_EYE = 3;

    struct BmpTime {
      byte nb; // bmp number
      long t; // duration
    };

    struct Sequence {
      word seq_time;
      byte mouth_size;
      BmpTime* mouth;
      byte r_eye_size;
      BmpTime* r_eye;
      byte l_eye_size;
      BmpTime* l_eye;
    };

    Adafruit_NeoMatrix *mouth = new Adafruit_NeoMatrix(8, 8, 3 , 2, MOUTH_PIN,
        NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
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
    long r_eye_t;
    long l_eye_t;

    //part face position
    byte mouth_p;
    byte r_eye_p;
    byte l_eye_p;

    Sequence current_seq;
    //Eyes
    BmpTime eyes[4] = {{YMIDDLE, 500}, {YRIGHT, 100}, {YMIDDLE, 200}, {YLEFT, 200} } ;
    BmpTime heartBeat[4] = {{YLOVE, 200}, {YBLACK, 200}, {YLOVE, 200}, {YBLACK, 200} } ;
    BmpTime heartBeat2[4] = {{YLOVE2, 200}, {YBLACK, 200}, {YLOVE2, 200}, {YBLACK, 200} } ;
    BmpTime mouthBlack[1] = {{MBLACK, 200}};
    BmpTime eyesBlack[1] = {{YBLACK, 200}};
    BmpTime eyesAngryL[1] = {{YANGRYL, 200}};
    BmpTime eyesAngryR[1] = {{YANGRYR, 200}};
    BmpTime eyesSurprise[1] = {{YSURPRISE, 200}};
    BmpTime eyesSurpriseR[1] = {{YANGRYL, 200}};
    BmpTime eyesSurpriseL[1] = {{YANGRYR, 200}};
    BmpTime eyesNeutral[1] = {{YNEUTRAL, 200}};
    BmpTime eyesCross[1] = {{YCROSS, 200}};
    //BmpTime eyes[1] = {{YSMILEY, 200}};
    BmpTime eyesSmiley[1] = {{YSMILEY, 200}};
    BmpTime eyesLine1[1] = {{YLINES, 200}};
    BmpTime eyesLine2[1] = {{YLINES2, 200}};
    BmpTime eyesLine3[1] = {{YLINES3, 200}};
    BmpTime eyesLine4[1] = {{YLINES4, 200}};
    BmpTime eyesBlue[1] = {{YBLUE, 200}};
    BmpTime eyesBored[1] = {{YBORED, 200}};

  

    //Mouth
    BmpTime still[1] = {{MCLOSE, 2000}} ;
    BmpTime speak[7] = {{MCLOSE, 20}, {MOPEN1, 10}, {MOPEN2, 20}, {MOPEN3, 10}, {MOPEN2, 20}, {MOPEN1, 10}, {MCLOSE, 20}} ;
    BmpTime speak1[2] = {{MCLOSE, 20}, {MOPEN1, 50}} ;
    BmpTime speak2[2] = {{MOPEN1, 30}, {MOPEN2, 50}} ;
    BmpTime speak3[2] = {{MOPEN2, 30}, {MOPEN3, 50}} ;
    BmpTime mouthAngry[1] = {{MANGRY, 200}} ;
    BmpTime mouthFunny[1] = {{MFUNNY, 200}} ;
    BmpTime mouthNeutral[1] = {{MNEUTRAL, 200}} ;
    BmpTime mouthHoo[1] = {{MHOO, 200}} ;
    BmpTime mouthHaa[1] = {{MHAA, 200}} ;
    BmpTime mouthD[1] = {{MD, 200}} ;
    BmpTime mouthEh[1] = {{MEH, 200}} ;
    BmpTime mouthL[1] = {{ML, 200}} ;
    BmpTime mouthM[1] = {{MM, 200}} ;
    BmpTime mouthHo[1] = {{MHO, 200}} ;
    BmpTime mouthR[1] = {{MR, 200}} ;
    BmpTime mouthS[1] = {{MS, 200}} ;
    BmpTime mouthUh[1] = {{MUH, 200}} ;
    BmpTime mouthWoo[1] = {{MWOO, 200}} ;
    BmpTime speak4[10] = {{20, MHOO}, {30, MEH}, {20, MEH}, {40, ML}, {30, MM}, {30, MHO}, {40, MR}, {20, MS}, {30, MUH}, {50, MWOO}};
    BmpTime mouthHelp[4] = {{HELP, 200},{MBLACK, 200},{HELP, 200},{MBLACK, 200}} ;
    BmpTime full[1] = {{FULL, 2000}} ;    



#define nbSeq 24
    Sequence sequences[nbSeq] = {
      {10000, 1, still,  4, eyes, 4, eyes},
      {50, 7, speak,  4, eyes, 4, eyes}, // speak
      {50, 7, speak, 4, heartBeat, 4, heartBeat}, //love
      {10000000, 1, mouthBlack, 1, eyesBlack, 1, eyesBlack}, //shutdown
      {250, 7, speak,  4, eyes, 4, eyes}, // speak 2.5s
      {1000, 7, speak,  4, eyes, 4, eyes}, //speak 10s
      {2000, 7, speak,  4, eyes, 4, eyes},
      {1000, 1, mouthFunny,  1, eyesSurpriseL, 1, eyesSurpriseR},
      {1000, 7, still,  1, eyesBored, 1, eyesBored},
      {1000, 1, mouthAngry,  1, eyesAngryL, 1, eyesAngryR},
      {2000, 10, mouthHoo,  1, eyesSmiley, 1, eyesSmiley},
      {500, 10, speak4,  1, eyesLine2, 1, eyesLine2},
      {500, 7, speak,  4, eyes, 4, eyes}, // speak 1s ||12
      {1000, 7, speak,  4, eyes, 4, eyes}, // speak 1s ||12      
      {1500, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 13
      {2000, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 14
      {2500, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 15
      {3500, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 16
      {5600, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 17
      {7000, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 18
      {8000, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 19
      {15000, 7, speak,  4, eyes, 4, eyes}, // speak 1s || 19     
      {15000, 7, speak,  1, eyesBored, 1, eyesBored}, // speak 1s || 19            
      {10000, 7, mouthHelp,  4, eyes, 4, eyes}
    };

    void initMatrix(Adafruit_NeoMatrix * matrix) {
      matrix->begin();
      matrix->clear();
      matrix->setBrightness(BRIGHTNESS);
    }

  public :

    Face() {
      initMatrix(mouth);
      initMatrix(l_eye);
      initMatrix(r_eye);
      loadSequence(default_seq);

    }

    void process(char* in) {
      /*char numChar[3] = {};
        //numChar[0] = in[1];
        //numChar[1] = in[2];
        extractChar(numChar, in, 1, 2) ;
        current_seq_nb = 0;*/

      if (!strcmp(in, "")) {
        return;
      }


      extractChar(in, in, 1, 2);
      in[2] = 0;
      int new_seq = atoi(in);

      if (new_seq < nbSeq) {
        current_seq_nb = new_seq;
        loadSequence(current_seq_nb);
        Log.notice("######## Face process input:\"%s\" sequence:%d current_seq_nb:%d ########\n", in, new_seq, current_seq_nb );
      } else {
        Log.notice("######## Face process input:\"%s\" sequence:%d not in range\n", in, new_seq);
      }
    }

    long loadPart(Adafruit_NeoMatrix * part, const uint16_t bitmap[] , long part_time, int w, int h) {
      //cur_time = millis() + part_time;
      part->drawRGBBitmap(0, 0, bitmap, w, h);
      part->show();
      return millis() + part_time;
    }

    void loadSequence(byte seq_nb) {

      //Log.notice("loadSequence millis():%d seq_nb:%d seq_time:%d\n", millis(), seq_nb, seq_time);

      current_seq = sequences[seq_nb];
      seq_time = millis() + sequences[seq_nb].seq_time;

      BmpTime bmptime = current_seq.mouth[0];
      mouth_t = loadPart(mouth, bitmap24[bmptime.nb],  bmptime.t,  24, 16);

      bmptime = current_seq.l_eye[0];
      l_eye_t = loadPart(l_eye, bitmap8[bmptime.nb],  bmptime.t,  8, 8);

      bmptime = current_seq.r_eye[0];
      r_eye_t = loadPart(r_eye, bitmap8[bmptime.nb],  bmptime.t,  8, 8);

    }

    void execute() {

      //Log.notice("execute millis():%d seq_time:%d mouth_t:%d\n", millis(), seq_time, mouth_t);

      if (millis() > seq_time) {
        loadSequence(default_seq);
      } else {
        if (millis() > mouth_t) {
          //Log.notice("mouth next current_seq.mouth_size:%d mouth_p:%d\n", current_seq.mouth_size, mouth_p);
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

      }
    }

};
