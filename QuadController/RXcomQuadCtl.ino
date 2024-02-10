#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
byte address[6] = "00001";

/* ping read data struct for Com*/
#ifndef COMDATAST 
#define COMDATAST
typedef struct {
  uint8_t throttlePos;
  uint8_t navCmd;
  uint8_t debugAckRequest;
  bool isProcessedMsg;
} comRFReadDataRXSt;
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
void InitRXComm() ;
uint8_t comRXreadRFData(comRFReadDataRXSt *inData, comRFAckDataRXSt *ackData);


void InitRXComm() {

  

  radio.begin();


  // Set the PA Level low to prevent power supply related issues since this is a

  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.

  radio.setPALevel(RF24_PA_MAX);

  // Open a writing and reading pipe on each radio, with opposite addresses



  radio.enableAckPayload();

  radio.openReadingPipe(0, address);

  radio.startListening();

}


uint8_t comRXreadRFData(comRFReadDataRXSt *inData, comRFAckDataRXSt *ackData)
{
  uint8_t retSts=0;
  ackData->stats |= 1<<7;
  if (radio.available()) {
    radio.writeAckPayload(0, ackData, sizeof(comRFAckDataRXSt));
    radio.read(inData, sizeof(comRFReadDataRXSt));
    retSts=1;
  }

}
