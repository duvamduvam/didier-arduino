#include "U8glib.h"

//SDA 4 SCL 5
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);	// I2C / TWI

void draw(void)
{
  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_gdr30r);
  u8g.drawStr(3, 50, "A1");
  
  u8g.setFont(u8g_font_gdr14r);
  u8g.drawStr(60, 20, "je sais pas lll");
  u8g.drawStr(60, 55, "je sais pas lll");
}

void setup(void) {
  // flip screen, if required
  //u8g.setRot180();
}

void loop(void) {

  // picture loop
  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );

  // rebuild the picture after some delay
  delay(200);
}
