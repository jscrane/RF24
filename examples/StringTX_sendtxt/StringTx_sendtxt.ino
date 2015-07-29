#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
 
/*
This sketch sends a string to a corresponding Arduino
with nrf24 attached using sendtxt() function.  
I has a Checksum as well as acknowledge support.
-- By Prasad Pandit (prasadp4009@gmail.com)
*/

RF24 radio(9,8);
const uint64_t wpipe = 0xE8E8F0F0E1LL;
const uint64_t rpipe = 0xE8E8F0FFD1LL;
void setup(void){
  Serial.begin(9600);
  radio.begin();
  delay(10);
  //radio.openWritingPipe(pipe);}
}
void loop(void){
  bool ack = false;
  String theMessage = "Hi This is MSP430..!!";
  ack = radio.sendtxt(theMessage, wpipe, rpipe);
  if(ack)
  Serial.println("Transmission ACK Successful");
  else
  Serial.println("Transmission ACK Failed");

  /*int messageSize = theMessage.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSend[1];
    charToSend[0] = theMessage.charAt(i);
    radio.write(charToSend,1);
  }  
//send the 'terminate string' value...  
  msg[0] = 2; 
  radio.write(msg,1);
//delay sending for a short period of time.  radio.powerDown()/radio.powerupp
//with a delay in between have worked well for this purpose(just using delay seems to
//interrupt the transmission start). However, this method could still be improved
as I still get the first character 'cut-off' sometimes. I have a 'checksum' function
on the receiver to verify the message was successfully sent.
*/
  /*radio.powerDown(); 
  delay(100);
  radio.powerUp();
  delay(10);
  */
  delay(1000);
  
}

