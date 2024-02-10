#include <PID_v1.h>

float Kp=10; //Initial Proportional Gain
float Ki=0.0; //Initial Integral Gain
float Kd=0.0; //Initial Differential Gain
double Setpoint=0, Input, Output; //These are just variables for storingvalues
int sampleRate = 100;
PID throttlePID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void initPID()
{
 throttlePID.SetMode(AUTOMATIC); //Turn on the PID loop
 throttlePID.SetSampleTime(sampleRate);
 throttlePID.SetOutputLimits(-1024, 1023);
 throttlePID.SetTunings(Kp, Ki, Kd);

}

void setPidGains(float _kp,float _ki, float _kd,int _sampleRate)
{
    Kp=_kp;
    Ki=_ki;
    Kd=_kd;
    sampleRate=_sampleRate;
    throttlePID.SetSampleTime(sampleRate);
    throttlePID.SetTunings(Kp, Ki, Kd);
}

float processPIDOutput(float input,float setPoint)
{
    Input=input;
    Setpoint=setPoint;
    throttlePID.Compute();
    return Output;

}
