#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>



RF24 radio(7, 3); // CE, CSN
LiquidCrystal lcd(8, 9, 6, 5, 4, 3);
const byte address[6] = "00001";

#ifndef SWITCH_COFIGURED
#define SWITCH_COFIGURED
#define SWITCH_INPUT A3
#define KEYPRS_DLY 150
//#define EN_DEBUG
#define EN_DEBUG_ERR
#define INTERVAL_MINUS 10
#define INTERVAL_PLUS 10
typedef enum
{
  NO_BTN_PRS,
  RGT_BTN,
  UP_BTN,
  LFT_BTN,
  DWN_BTN,
  SET_BTN,
  BTN_ERR = 0xFF,
} btnMap_en;

typedef struct
{
  btnMap_en buttonId;
  uint16_t buttonADClb;
} btnADCMap_t;

#define BTN_MAP_COUNT 5

btnADCMap_t btnADCMap[BTN_MAP_COUNT] = {
  {RGT_BTN, 114},
  {UP_BTN, 130},
  {LFT_BTN, 150},
  {DWN_BTN, 177},
  {SET_BTN, 1014}, // 1023
};
#endif
extern btnMap_en DetectkeyPress();
#ifndef COMDATAST
#define COMDATAST
typedef struct {
  uint8_t throttlePos;
  uint8_t navCmd;
  uint8_t debugAckRequest;
  bool isProcessedMsg;
} comRFWriteDataTxSt;
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


} comRFAckDataTxSt;
#endif
void clearAckData(comRFAckDataTxSt* dataBuf) ;

#define BUZZ_PIN 10
#define BUZZ_INIT_TONE 1000
#define BUZZ_VALD_KEYPRS 200
#define BUZZ_DLY 50

void buzzOutInit(uint16_t toneDuration);
void setup() {

  Serial.begin(115200);

  Serial.print("Init");
  // keep the buzzer mouth ready but shut
  pinMode(BUZZ_PIN,OUTPUT);
  digitalWrite(BUZZ_PIN,LOW);
  pinMode(A3,INPUT);


  radio.begin();
  lcd.begin(16, 2);
  lcd.write("TX init...");
  // lcdInit("Ready to race?");
  pinMode(SWITCH_INPUT, INPUT);
  // Set the PA Level low to prevent power supply related issues since this is a

  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.enableAckPayload();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  buzzOutInit(BUZZ_INIT_TONE);


}



comRFAckDataTxSt comTxAckDataIncoming;
btnMap_en btnPressed_en;
uint8_t currentThrottlePos = 0;



uint8 currNavCmd=0;
void loop() 
{
  
  btnPressed_en = DetectkeyPress();
  if (btnPressed_en != NO_BTN_PRS && btnPressed_en != BTN_ERR)
  {
    //inputdata=Serial.read();
    buzzOutInit(BUZZ_VALD_KEYPRS);
    
    comRFWriteDataTxSt outData;
   
    outData.navCmd = currNavCmd;
    outData.debugAckRequest = 0;
    outData.isProcessedMsg = false;

    switch (btnPressed_en) {
      case RGT_BTN:
        //Serial.println("Right key pressed");
        lcd.print("Right");
        break;
      case UP_BTN:
        //Serial.println("Up key pressed");
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Up");
        //delay(500);
        currentThrottlePos = currentThrottlePos < 174 ? currentThrottlePos + 5 : 180;
        break;
      case LFT_BTN:
        //Serial.println("Left key pressed");
        lcd.print("Left");
        break;
      case DWN_BTN:
        // Serial.println("Down key pressed");
        lcd.print("Down");
        currentThrottlePos = currentThrottlePos < 6 ? 6 : currentThrottlePos - 5;
        break;
      case SET_BTN:
       lcd.print("Set");
        //Serial.println("Set key pressed");
        currentThrottlePos=0;
       // enable balancing mode test 
        if(stateSwtCtr==0){
            currNavCmd=0;
          }
        stateSwtCtr++;
        if(stateSwtCtr==2)
        {
            stateSwtCtr=0;
            currNavCmd=1;
             lcd.print("Set:stabilize!");
          }

        break;
      default:
        break;
    }
    outData.throttlePos = currentThrottlePos;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Sending:");
    if ( !radio.write( &outData, sizeof(outData)) ) 
    {
        //lcd.clear();
        Serial.print("Error writing pipe");
        lcd.setCursor(8, 0);
        lcd.write("TR_ERR!");
       
     }
   
    
   // delay(100);
  }
  


  if (radio.isAckPayloadAvailable()) 
  {
    clearAckData(&comTxAckDataIncoming);
    radio.read(&comTxAckDataIncoming, sizeof(comTxAckDataIncoming));

    //radio.printDetails();
    //Serial.println( );
  }

}

void clearAckData(comRFAckDataTxSt* dataBuf) {
  uint8_t* tmpBuf = (uint8_t*)dataBuf;
  uint8_t i = 0;
  uint8_t len = (uint8_t)sizeof(comRFAckDataTxSt);
  for (; i < len; i++) {
    *(tmpBuf + i) = 0;
  }

}

void buzzOutInit(uint16_t toneDuration){
  uint8_t ct=0;
  uint8_t beepCycle=(uint8_t)(toneDuration/(BUZZ_DLY*2));
  while(ct<beepCycle)
  {
    digitalWrite(BUZZ_PIN,HIGH);
    delay(BUZZ_DLY);
    digitalWrite(BUZZ_PIN,LOW);
    delay(BUZZ_DLY);
  ct++;
  }
}
