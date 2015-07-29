#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
 
/*
This sketch receives strings from sending unit via nrf24 
and prints them out via serial.  The sketch waits until
it receives a specific value (2 in this case), then it 
prints the complete message and clears the message buffer.
*/

//Burn StringTx_sendtxt in another board and send any text from that board(Arduino can also be used)

int msg[1];

RF24 radio(9,8);

const uint64_t rpipe = 0xE8E8F0F0E1LL;
const uint64_t wpipe = 0xE8E8F0FFD1LL;

unsigned int strcheck = 0, count = 0;
bool gotChecksum = false;
bool rxComplete = false;
String theMessage = "";
String checksum = "";

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,rpipe);
  radio.startListening();
}

void loop(void){
  if (radio.available()){
    
      bool done = false;  
      done = radio.read(msg, 1); 
      char theChar = msg[0];
    
    if(!gotChecksum){
    if (msg[0] != int('~')){
        checksum.concat(theChar);
       }
    else
      {
        strcheck = checksum.toInt();
        gotChecksum = true;
      }
    }
    
    if (gotChecksum && (msg[0] != int('~')))
    {  
      if (msg[0] != int('^')){
        theMessage.concat(theChar);
        }
      else {
       count = theMessage.length();
       Serial.println(theMessage);
       //Serial.println(strcheck);
       //Serial.println(count);
       theMessage= ""; 
       checksum = "";
       //strcheck = 0;
       //count = 0;
       gotChecksum = false;
       rxComplete = true;
      }
    }
    
  }
  if(rxComplete){
    radio.stopListening();
    radio.openWritingPipe(wpipe);
    delayMicroseconds(150);
    if(strcheck == count){
    int ack[1];
    ack[0] = 'a';  
    radio.write(ack,1);
    }
    else{
    int ack[1];
    ack[0] = 'n';  
    radio.write(ack,1);
    }
    
    rxComplete = false;
    strcheck = 0;
    count = 0;
    radio.openReadingPipe(1,rpipe);
    radio.startListening(); 
  }
}
