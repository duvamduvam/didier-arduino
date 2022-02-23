#include "ArduinoLog.h"

#define BP_CALIB    24


  //Serial.print("1 : "); Serial.print(analogRead(A3)); Serial.print(" "); 
  //Serial.print("2 : "); Serial.print(analogRead(A4)); Serial.print(" "); 
  //Serial.print("3 : "); Serial.print(analogRead(A5)); Serial.print(" "); 

class Inclino {

  private :

    int angleX, angleY, angleZ;
    int angleCalibX, angleCalibY, angleCalibZ;
    
  public :
    bool newData = false;

    Inclino() {
      Log.notice("init inclonmeter\n");
    }

    void read() {

      while (Serial.available())
      {
        //JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
        newData = true;
      }

      if (newData) {


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
        int aX = 0;//map(JY901.stcAngle.Angle[0], -32767, 32768, -180, 180);
        int aY = 0;//map(JY901.stcAngle.Angle[1], -32767, 32768, -180, 180);
        int aZ = 0;//map(JY901.stcAngle.Angle[2], -32767, 32768, -180, 180);

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
