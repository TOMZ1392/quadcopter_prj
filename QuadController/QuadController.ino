
#include <Servo.h>


//#define EN_DEBUG
#define EN_DEBUG_ERR
#define INTERVAL_MINUS 10
#define INTERVAL_PLUS 10


Servo ESC1, ESC2;
#define ESCPIN1 5
#define ESCPIN2 6
#define PASSDLY 50
#define THROTTLE_LIMIT   70
Servo ESC;



volatile uint16_t accVal1 = 0;
volatile uint16_t accVal2 = 0;

char c = '-';
bool isupd = false;
bool rampActivated = true;
extern void initPID();
extern void setPidGains(float _kp, float _ki, float _kd, int _sampleRate);
extern float processPIDOutput(float);
extern void initMPU();
extern void pollXYZAngles(float*, float*, float*);
//extern void lcdInit(String);

//extern void printOnLcd(keyValPair_st *, int);
int pidSampleRate = 100;
float angleX = 0, angleY = 0, angleZ = 0;
//keyValPair_st dispInfo[4];

#ifndef COMDATAST 
#define COMDATAST
typedef struct{
  uint8_t throttlePos;
  uint8_t navCmd;
  uint8_t debugAckRequest;
  bool isProcessedMsg;
}comRFReadDataRXSt;
#endif

/* Acknowledgement pong data struct for Com*/

#ifndef COMACKDATAST 
#define COMACKDATAST
typedef struct {
  uint8_t stats;
  uint8_t throtPos;
  uint8_t battSts;
  float angleX;
  float angleY;
  float angleZ;
  uint8_t pidGainX;
  uint8_t pidGainY;
  uint8_t pidGainZ;


} comRFAckDataRXSt;
#endif
extern void InitRXComm() ;
extern uint8_t comRXreadRFData(comRFReadDataRXSt *inData, comRFAckDataRXSt *ackData);

void setup()
{

  Serial.begin(9600);
  ;
  //lcdInit("Ready to race?");
  initMPU();
  InitRXComm();
  //pinMode(SWITCH_INPUT, INPUT);
  ESC1.attach(ESCPIN1, 1000, 2000);
  ESC2.attach(ESCPIN2, 1000, 2000);
  ESC1.write(0);
  ESC2.write(0);
  initPID();
  //  delay(5000);
  // printOnLcd("Boot phase", "Complete.. Firing up.");
}

//void processThrottle(btnMap_en key);
uint8_t throttleABCD = 0;



float pidOutGainX = 0, pidOutGainY = 0, pidOutGainZ = 0;
float angleSetPointX = 0, angleSetPointY = 0, angleSetPointZ = 0;

short normGainX = 0;
boolean emrSgt = false;
/* ping read data struct for Com*/

comRFReadDataRXSt comRdDataRX;
/* Acknowledgement pong data struct for Com*/
comRFAckDataRXSt comRxAckData;

extern uint8_t comRXreadRFData(comRFReadDataRXSt *,comRFAckDataRXSt *);
void scheduleLiftGenStates();

void loop()
{
  /*while(throttleABCD<60 && !emrSgt){
      throttleABCD+=1;
      Serial.println(throttleABCD);
      ESC1.write(throttleABCD);
       ESC2.write(throttleABCD-4);
       delay(400);
      }*/
  Serial.println(throttleABCD);
  comRXreadRFData(&comRdDataRX,&comRxAckData);
  comRxAckData.battSts=0xFF;
  throttleABCD=comRdDataRX.throttlePos;
 
  switch(comRdDataRX.navCmd){
    case 0:// Test mode
        ESC1.write(comRdDataRX.throttlePos+ 39);
        ESC2.write(comRdDataRX.throttlePos );
    break;
    case 1:// Balancing logic
          scheduleLiftGenStates();
        

  }
}
void scheduleLiftGenStates(){
uint8_t stateVar=0;
boolean doLoop=true;
while(doLoop)
{
    switch(stateVar)
    {
        case 0:
             pollXYZAngles(&angleX, &angleY, &angleZ);
             comRxAckData.angleX=angleX;
             comRxAckData.angleY=angleY;
             comRxAckData.angleY=angleY;
             
             stateVar=1;
        break;
        case 1:
             pidOutGainX = processPIDOutput(angleX, angleSetPointX);
             pidOutGainY = processPIDOutput(angleY, angleSetPointY);
             comRxAckData.pidGainX=pidOutGainX;
             comRxAckData.pidGainY=pidOutGainY;
             comRxAckData.pidGainZ=0.0;
             stateVar=2;
        break;
        case 2:
             normGainX = map(pidOutGainX, -1024, 1023, -100, 100);
             stateVar=3;
        break;
        case 3:
             comRxAckData.throtPos=throttleABCD;
             uint8_t throtFireHighX = map(throttleABCD - normGainX, 0, 179, 0, 179);
             uint8_t throtFireLowX = map(throttleABCD + normGainX, 0, 179, 0, 179);
             ESC1.write(throtFireHighX);
             ESC2.write(throtFireLowX);
             stateVar=4;
        break;
        case 4:
        doLoop=false;
        default:
        break;
    }
}

}


// void oldproc(){
//  if (btnPrssloc_en != NO_BTN_PRS && btnPrssloc_en != BTN_ERR)
//  {
//    //UPDATES
//    Serial.println("pressed");
// //   processThrottle(btnPrssloc_en);
//    emrSgt = !emrSgt;
//    rampActivated = false;
//  }
// // if (throttleABCD > 50)
//  {
//    pollXYZAngles(&angleX, &angleY, &angleZ);
//    
//    Serial.print("AngleX : ");
//    Serial.print(angleX);
//strcpy(dispInfo[0].msg,"<X:");
//dispInfo[0].val=18.0;
//    pidOutGainX = processPIDOutput(angleX, angleSetPointX);
//
//
//    Serial.print(" Raw pid out: ");
//    Serial.print(pidOutGainX);
//    strcpy(dispInfo[1].msg,"GV:");
//    dispInfo[1].val=22.2;
//    //printOnLcd((keyValPair_st*)((void*)&dispInfo),2);
//    
//    normGain = map(pidOutGainX, -1024, 1023, -100, 100);
//    Serial.print(" Normal: ");
//    Serial.print(normGain);
//    uint8_t throtFire = map(throttleABCD - normGain, 0, 179, 0, 179);
//    //        int throtFire=throttleABCD - normGain;
//   
//    Serial.print(" throt1: ");
//    throtFire = throtFire < THROTTLE_LIMIT ? throtFire : THROTTLE_LIMIT;
//    Serial.print(throtFire);
//    throtFire=throttleABCD<46?0:throtFire; //  force off pos
//    ESC1.write(throtFire);
//    throtFire = map(throttleABCD + normGain, 0, 179, 0, 179);
//    throtFire = throtFire < THROTTLE_LIMIT ? throtFire : THROTTLE_LIMIT;
//    // throtFire=throttleABCD + normGain;
//    Serial.print(" throt2: ");
//    Serial.println(throtFire);
//    throtFire=throttleABCD<46?0:throtFire; 
//    ESC2.write(throtFire - 3);
//    // delay(100);
//
//  }
//}
