#include <MPU6050_tockn.h>
#include <Wire.h>

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int minVal=265;
int maxVal=402;

double x;
double y;
double z;

void initMPU();
void pollXZAngles();
float getAngleOtoPi(float);

void initMPU() {

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
}



void pollXYZAngles(float *x,float*y, float*z)
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
    int xAng = map(AcX,minVal,maxVal,-90,90);
    int yAng = map(AcY,minVal,maxVal,-90,90);
    int zAng = map(AcZ,minVal,maxVal,-90,90);

       *x= getAngleOtoPi( RAD_TO_DEG * (atan2(-yAng, -zAng)+PI) );
       *y= getAngleOtoPi( RAD_TO_DEG * (atan2(-xAng, -zAng)+PI) );
       *z= getAngleOtoPi( RAD_TO_DEG * (atan2(-yAng, -xAng)+PI) );
      

}

float getAngleOtoPi(float inputAngle)
{
  return 180 < inputAngle && inputAngle < 360 ? inputAngle - 360 : inputAngle;
}
