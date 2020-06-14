#include<SoftwareSerial.h>

SoftwareSerial mySerialPins(9,10);
 void sendMessage(){
  mySerialPins.println("AT+CMGF=1");
  delay(1000);
  mySerialPins.println("AT+CMGS=(+256 781479007)");
  delay(1000);
  mySerialPins.println((char)26);
  delay(1000);
   }

  void receiveMessage(){
  mySerialPins.println("AT+CNMI=2,2,0,0,0,0");
  delay(1000);
 }


void setup() {
  mySerialPins.begin(9600);
  Serial.begin(9600);
  delay(100);

}

void loop() {
 if(Serial.available()>0){
  switch(Serial.read()){
    case 's':
      sendMessage();
      break;
    case 'r':
      receiveMessage();
      break;
  }
 }

 if(mySerialPins.available()>0){
  Serial.write(mySerialPins.read());
 }





}
