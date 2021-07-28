#include <Wire.h>
#include <JY901.h>

#include "ArduinoLog.h"

#define BP_CALIB    24

class Inclino {

  private :

    int angleX, angleY, angleZ;
    int angleCalibX, angleCalibY, angleCalibZ;
    
  public :
    bool newData = false;

    Inclino() {
      Log.notice("init inclonmeter\n");
      Serial.begin(9600);
    }

    void read() {

      while (Serial.available())
      {
        JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
        newData = true;
      }

      if (newData) {

        /*
                Serial.print("Time:20"); Serial.print(JY901.stcTime.ucYear); Serial.print("-"); Serial.print(JY901.stcTime.ucMonth); Serial.print("-"); Serial.print(JY901.stcTime.ucDay);
                Serial.print(" "); Serial.print(JY901.stcTime.ucHour); Serial.print(":"); Serial.print(JY901.stcTime.ucMinute); Serial.print(":"); Serial.println((float)JY901.stcTime.ucSecond + (float)JY901.stcTime.usMiliSecond / 1000);
                Serial.print("Acc:"); Serial.print((float)JY901.stcAcc.a[0] / 32768 * 16); Serial.print(" "); Serial.print((float)JY901.stcAcc.a[1] / 32768 * 16); Serial.print(" "); Serial.println((float)JY901.stcAcc.a[2] / 32768 * 16);
                Serial.print("Acc:"); Serial.print((float)JY901.stcAcc.a[0] / 32768 * 16); Serial.print(" "); Serial.print((float)JY901.stcAcc.a[1] / 32768 * 16); Serial.print(" "); Serial.println((float)JY901.stcAcc.a[2] / 32768 * 16);
                Serial.print("Gyro:"); Serial.print((float)JY901.stcGyro.w[0] / 32768 * 2000); Serial.print(" "); Serial.print((float)JY901.stcGyro.w[1] / 32768 * 2000); Serial.print(" "); Serial.println((float)JY901.stcGyro.w[2] / 32768 * 2000);
                Serial.print("Angle:"); Serial.print((float)JY901.stcAngle.Angle[0] / 32768 * 180); Serial.print(" "); Serial.print((float)JY901.stcAngle.Angle[1] / 32768 * 180); Serial.print(" "); Serial.println((float)JY901.stcAngle.Angle[2] / 32768 * 180);
                Serial.print("Mag:"); Serial.print(JY901.stcMag.h[0]); Serial.print(" "); Serial.print(JY901.stcMag.h[1]); Serial.print(" "); Serial.println(JY901.stcMag.h[2]);
                Serial.print("Pressure:"); Serial.print(JY901.stcPress.lPressure); Serial.print(" "); Serial.println((float)JY901.stcPress.lAltitude / 100);
                Serial.print("DStatus:"); Serial.print(JY901.stcDStatus.sDStatus[0]); Serial.print(" "); Serial.print(JY901.stcDStatus.sDStatus[1]); Serial.print(" "); Serial.print(JY901.stcDStatus.sDStatus[2]); Serial.print(" "); Serial.println(JY901.stcDStatus.sDStatus[3]);
                Serial.print("Longitude:"); Serial.print(JY901.stcLonLat.lLon / 10000000); Serial.print("Deg"); Serial.print((double)(JY901.stcLonLat.lLon % 10000000) / 1e5); Serial.print("m Lattitude:");
                Serial.print(JY901.stcLonLat.lLat / 10000000); Serial.print("Deg"); Serial.print((double)(JY901.stcLonLat.lLat % 10000000) / 1e5); Serial.println("m");
                Serial.print("GPSHeight:"); Serial.print((float)JY901.stcGPSV.sGPSHeight / 10); Serial.print("m GPSYaw:"); Serial.print((float)JY901.stcGPSV.sGPSYaw / 10); Serial.print("Deg GPSV:"); Serial.print((float)JY901.stcGPSV.lGPSVelocity / 1000); Serial.println("km/h");
        */


        //    Serial.print("Gyro:"); Serial.print((float)JY901.stcGyro.w[0] / 32768 * 2000); Serial.print(" "); Serial.print((float)JY901.stcGyro.w[1] / 32768 * 2000); Serial.print(" "); Serial.println((float)JY901.stcGyro.w[2] / 32768 * 2000);
        //     Serial.print("Angle:"); Serial.print((float)JY901.stcAngle.Angle[0] / 32768 * 180); Serial.print(" "); Serial.print((float)JY901.stcAngle.Angle[1] / 32768 * 180); Serial.print(" "); Serial.println((float)JY901.stcAngle.Angle[2] / 32768 * 180);
        //   Serial.print("Acc:"); Serial.print((float)JY901.stcAcc.a[0] / 32768 * 16); Serial.print(" "); Serial.print((float)JY901.stcAcc.a[1] / 32768 * 16); Serial.print(" "); Serial.println((float)JY901.stcAcc.a[2] / 32768 * 16);

        //     newData = false;
      }
    }
    void aqqInclino()
    {
      read();
      if (newData)
      {
        int aX = map(JY901.stcAngle.Angle[0], -32767, 32768, -180, 180);
        int aY = map(JY901.stcAngle.Angle[1], -32767, 32768, -180, 180);
        int aZ = map(JY901.stcAngle.Angle[2], -32767, 32768, -180, 180);

        angleX = aX - angleCalibX;
        angleY = aY - angleCalibY;
        angleZ = aZ - angleCalibZ;

        if (angleX < -180)
          angleX += 360;
        if (angleY < -180)
          angleY += 360;
        if (angleZ < -180)
          angleZ += 360;

        if (digitalRead(BP_CALIB) == 0)
        {
          angleCalibX = aX;
          angleCalibY = aY;
          angleCalibZ = aZ;
        }

        Serial.print("X="); Serial.print(angleX); Serial.print("  ");
        Serial.print("Y="); Serial.print(angleY); Serial.print("  ");
        Serial.print("Z="); Serial.print(angleZ); Serial.println("  ");

        newData = false;
      }


    }

    /*void processInclino()
    {
      int x = map(angleX, -ANGLE_INCLINO, ANGLE_INCLINO, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
      int y = map(angleY, -ANGLE_INCLINO, ANGLE_INCLINO, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

      x = BoundInt(x, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
      y = BoundInt(y, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

      //Seuil d'envoi
      if (x > JOY_SEND_MARGIN || x <  -JOY_SEND_MARGIN || y > JOY_SEND_MARGIN || y < -JOY_SEND_MARGIN)
      {
        //Transformée géométrique
        SpeedVector robot = ToRobot(x, y);
        int l = robot.left * 100;
        int r = robot.right * 100;
        sendWheels(l, r);
        //   Log.notice("Wheels  l:%d r:%d\n", l, r);
      }
    }*/



};
