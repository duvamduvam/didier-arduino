

/////////////////////////////////// CO2
#include "dht.h"
dht DHT;

/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in
//normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (0.220) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTGAE             (0.030) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2

/*****************************Globals***********************************************/
float           CO2Curve[3]  =  {2.602, ZERO_POINT_VOLTAGE, (REACTION_VOLTGAE / (2.602 - 3))};
//two points are taken from the curve.
//with these two points, a line is formed which is
//"approximately equivalent" to the original curve.
//data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280)
//slope = ( reaction voltage ) / (log400 –log1000)

////////////////////////////////////// ECRAN
#include "U8glib.h"
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

/************************Hardware Related Macros************************************/
#define         CO2_PIN                       (A0)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (2)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier

///////////////////////////////////// TEMPERATURE HUMIDITE
#define   TEMPERATURE_HUMIDITE_PIN (A1)
#define HUMITIDTE_SOL_PIN A2
float sol = 0;

double maxTemp = 111;
double minTemp = 111;
double minHS = 111;
double maxHS = 111;
double minHA = 111;
double maxHA = 111;
double minCO2 = 111;
double maxCO2 = 111;




void setup()
{
  Serial.begin(9600);                              //UART setup, baudrate = 9600bps
  //pinMode(BOOL_PIN, INPUT);                        //set pin to input
  //digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors
  Serial.print("program jardin\n");

  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255, 255, 255);
  }

}

void loop()
{

  ////// CO2
  int co2;
  float volts;

  volts = MGRead(CO2_PIN);
  Serial.print( "SEN0159:" );
  Serial.print(volts);
  Serial.print( "V           " );

  co2 = MGGetPercentage(volts, CO2Curve);
  Serial.print("CO2:");
  if (co2 == -1) {
    Serial.print( "<400" );
  } else {
    Serial.print(co2);
  }

  Serial.print( "ppm | " );
  Serial.print(volts);
  Serial.println( "V           " );



  // pour co2, ne sais pas quoi ca sert
  /*if (digitalRead(BOOL_PIN) ){
      Serial.print( "=====BOOL is HIGH======" );
    } else {
      Serial.print( "=====BOOL is LOW======" );
    }*/

  Serial.print("\n");

  // Humidité témpérature

  DHT.read11(TEMPERATURE_HUMIDITE_PIN);

  Serial.print("Humidité air = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("témpérature = ");
  Serial.print(DHT.temperature);
  Serial.println("C  ");



  // sol
  for (int i = 0; i <= 100; i++)
  {
    sol = sol + analogRead(HUMITIDTE_SOL_PIN);
    delay(1);
  }
  sol = sol / 100.0;
  Serial.print("humidieté sol :" );
  Serial.println(sol);

  String stringSol = "";
  stringSol += String(int(sol)) + "." + String(getDecimal(sol));

  Serial.print("string sol ");
  Serial.println(stringSol);

  int delayEcran = 2000;

  if (co2 > maxCO2||maxCO2==111) {
    maxCO2 = co2;
  } else if (co2 > minCO2||minCO2==111) {
    minCO2 = co2;
  }
  draw("CO2 : ", String(co2), (String)minCO2 , (String)maxCO2);
  delay(delayEcran);
  if (DHT.temperature > maxTemp|| maxTemp==111) {
    maxCO2 = DHT.temperature;
  } else if (DHT.temperature > minTemp || minTemp==111) {
    minCO2 = DHT.temperature;
  }
  draw("temp : ", String(DHT.temperature), (String)minCO2 , (String)maxCO2);
  delay(delayEcran);
  if (DHT.humidity > maxHA||maxHA==111) {
    maxHA = DHT.humidity;
  } else if (DHT.humidity > minHA||minHA==111) {
    minHA = DHT.humidity;
  }
  draw("HA : ", String(DHT.humidity), (String)minHA , (String)maxHA);
  delay(delayEcran);
  delay(delayEcran);
  if (sol > maxHS||maxHS==111) {
    maxHS = sol;
  } else if (sol > minHS||maxHS==111) {
    minHS = sol;
  }
  draw("HS : ", stringSol, (String)minHS , (String)maxHS);
  delay(delayEcran);

}


//function to extract decimal part of float
long getDecimal(float val)
{
  int intPart = int(val);
  long decPart = 10 * (val - intPart); //I am multiplying by 1000 assuming that the foat values will have a maximum of 3 decimal places
  //Change to match the number of decimal places you need
  if (decPart > 0)return (decPart);       //return the decimal part of float number if it is available
  else if (decPart < 0)return ((-1) * decPart); //if negative, multiply by -1
  else if (decPart = 0)return (00);       //return 0 if decimal part of float number is not available
}

void draw(String libelle, String value, String min, String max ) {

  u8g.firstPage();
  do {
    // graphic commands to redraw the complete screen should be placed here
    u8g.setFont(u8g_font_unifont);
    //u8g.setTextSize(5);
    //u8g.setFont(u8g_font_osb21);
    String output = libelle + value;
    u8g.drawStr( 0, 22, output.c_str());
  } while ( u8g.nextPage() );

  delay(2000);

  u8g.firstPage();
  do {
    // graphic commands to redraw the complete screen should be placed here
    u8g.setFont(u8g_font_unifont);
    String space = " ";

    String MIN_MAX = min + space + max ;
    u8g.drawStr( 0, 22, MIN_MAX.c_str());
  } while ( u8g.nextPage() );
}

/*****************************  MGRead *********************************************
  Input:   mg_pin - analog channel
  Output:  output of SEN-000007
  Remarks: This function reads the output of SEN-000007
************************************************************************************/
float MGRead(int mg_pin)
{
  int i;
  float v = 0;

  for (i = 0; i < READ_SAMPLE_TIMES; i++) {
    v += analogRead(mg_pin);
    delay(READ_SAMPLE_INTERVAL);
  }
  v = (v / READ_SAMPLE_TIMES) * 5 / 1024 ;
  return v;
}

/*****************************  MQGetPercentage **********************************
  Input:   volts   - SEN-000007 output measured in volts
         pcurve  - pointer to the curve of the target gas
  Output:  ppm of the target gas
  Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm)
         of the line could be derived if y(MG-811 output) is provided. As it is a
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic
         value.
************************************************************************************/
int  MGGetPercentage(float volts, float *pcurve)
{
  if ((volts / DC_GAIN ) >= ZERO_POINT_VOLTAGE) {
    return -1;
  } else {
    return pow(10, ((volts / DC_GAIN) - pcurve[1]) / pcurve[2] + pcurve[0]);
  }
}
