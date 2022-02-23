#include <SoftwareSerial.h>
#include "Fonctions.h"
#include "ArduinoLog.h"
#include <Button.h>
#include "U8glib.h"
#include <Wire.h>
#include "Inclino.cpp"
#include "Leds.cpp"

//SDA 4 SCL 5
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  // I2C / TWI
char mod = 'C';

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

SoftwareSerial buttons(11, 10); //tx,rx

char key[2];

#define INPUT_SIZE 2
char glove[INPUT_SIZE];
char lastKey[INPUT_SIZE];
bool newData = false;

Inclino inclino;
Lights lights;

void setup()
{
  Serial.begin(115200);
  buttons.begin(9600);
  Log.begin   (LOG_LEVEL, &Serial);
  Log.notice("start glove right core\n");
  //analogWrite(VIBRATION, 0);
}

bool recvWithStartEndMarkersButtons(char* input, bool newData, byte inputSize) {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (buttons.available() > 0 && newData == false) {
    rc = buttons.read();
    //Serial.print("rc : ");Serial.println(rc);
    if (recvInProgress == true) {
      if (rc != endMarker) {
        input[ndx] = rc;
        ndx++;
        if (ndx >= inputSize) {
          ndx = inputSize - 1;
        }
      }
      else {
        //Serial.println("end receive");
        input[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        return true;
      }
    }

    else if (rc == startMarker) {
      //Serial.println("receive in progress");
      recvInProgress = true;
    }
  }
  return false;
}

void draw()
{
  u8g.setColorIndex(1);

  //Mode
  if (newData) {
    strcpy(lastKey ,glove);
  }

  u8g.setFont(u8g_font_gdr30r);
  //char modScreen[2] = "";
  //modScreen[0] = glove;
  u8g.drawStr(3, 30, lastKey);

  //Config
  /*u8g.setFont(u8g_font_gdr14r);
    char conf[6] = "A1 B1";
    conf[1] = '0' + ConfA;
    conf[4] = '0' + ConfB;
    u8g.drawStr(60, 26, conf);*/

  //Message
  /*u8g.setFont(u8g_font_gdr14r);
    char printKey[3] = {mod};
    //printKey[1] = key[0];
    u8g.drawStr(3, 60, glove);

    //u8g.drawLine(7+a, 10, 40, 55);*/
}

void loop()
{

  //Log.notice("start receive\n");
  newData = recvWithStartEndMarkersButtons(glove, newData, INPUT_SIZE);
  //Log.notice("stop receive\n");

  if (newData) {
    Serial.print("<"); Serial.print(glove); Serial.println(">");
  }

  //vibrator.execute();
  //lights.fade();

  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );

  if (newData) {
    newData = false;
  }

}
