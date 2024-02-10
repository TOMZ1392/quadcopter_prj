#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>



 
RF24 radio(7, 8); // CE, CSN




byte address[6] = "00001";
 

 
void setup() {
 
Serial.begin(115200);
 
Serial.print("Init");

 
 radio.begin();

 
// Set the PA Level low to prevent power supply related issues since this is a
 
// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
 
radio.setPALevel(RF24_PA_MAX);
 
 // Open a writing and reading pipe on each radio, with opposite addresses
 

 
  radio.enableAckPayload();
 
  radio.openReadingPipe(0,address);
 
  radio.startListening(); 
 

 
}
char inputdata[5];
char out[2]={'O','K'};
void loop() {
 
 
  
  
   
   if (radio.available()){
    radio.writeAckPayload(0,out,2);
   radio.read( &inputdata, 5 );
     Serial.println(inputdata);
    // delay(1000);
   
   } 
 
  
  
 
}
 
