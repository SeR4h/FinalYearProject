
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <String.h>
#include <DHT.h> 
#include <Adafruit_Sensor.h>
#include <Nokia_LCD.h>

#define MQ2pin (A0)
#define DHTpin 8
#define DHTType DHT22
DHT dht(DHTpin,DHTType);

const int buzzer=12;
const int ledPin=13;
const int buttonPin=2;
int buttonState ;

unsigned long startTime = 0;
unsigned long intervalTime = 50000UL; //2 minutes

float gas;
float hum;
float temp;

Nokia_LCD lcd(3 /* CLK */, 4 /* DIN */, 5 /* DC */, 7 /* CE */, 6 /* RST */);
TinyGPSPlus GPS;
SoftwareSerial gpsSerial(4,3);
SoftwareSerial mySerial(11,10);
void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  dht.begin();
  lcd.begin();

pinMode(buzzer, OUTPUT);
pinMode(ledPin, OUTPUT);
pinMode(buttonPin, INPUT); 
delay(1000); 
}
void loop() {
hum = dht.readHumidity(); 
temp = dht.readTemperature(); 
gas = analogRead(MQ2pin);   

lcd.clear();
lcd.setCursor(30, 0);
lcd.println("MONITOR");
lcd.setCursor(0, 1);
lcd.print("GasLevel: ");
lcd.print(gas);
lcd.setCursor(0,3);
lcd.print("Humidity: ");
lcd.print(hum);
lcd.print("%");
lcd.setCursor(0,5);
lcd.print("Temp: ");
lcd.print(temp);
lcd.print(" *C"); 

if(gas>100 || temp<20 || temp>48){
   tone(buzzer,1000);
  digitalWrite(ledPin, HIGH);

}
//if button is pressed
if (digitalRead(buttonPin)){
    // turn LED and buzzer off :
      noTone(buzzer);
    digitalWrite(ledPin, LOW);    
  }

 // Send message after 2minutes
if((digitalRead(buttonPin) == LOW) && (digitalRead(ledPin) == HIGH) && (millis()-startTime>intervalTime)){
 
   sendMessage(); 
  }  
delay(3000);
 sendSensorData(); 
}

void sendMessage(){
    while( gpsSerial.available()>0){
    GPS.encode(gpsSerial.read());
    //Serial.write(mySerial.read());
    }
    
    if(GPS.location.lat()== 0000){
 Serial.println("Initializing..."); 
     delay(1000);
  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+256704962749\"");
  updateSerial();
  mySerial.print("I've had an attack,please find me.");
    updateSerial();
  mySerial.println("Cannot find the exact location.");
    updateSerial();      
  mySerial.println("\r");
  mySerial.write(26);    
      
    }
    else{
        mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+256704962749\"");
  updateSerial();
  mySerial.print("I've had an attack,please find me. I'm here ");
  updateSerial();
  mySerial.print("www.google.com.ph/maps/place/");    
  updateSerial();
  mySerial.print(GPS.location.lat(),6); 
  mySerial.print(",");  
  mySerial.print(GPS.location.lng(),6);    
  mySerial.println("\r");
  mySerial.write(26); 
      } 
  }
  void updateSerial(){
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
void sendSensorData(){
if (mySerial.available()){
    Serial.write(mySerial.read()); 
 mySerial.println("AT");
  delay(100); 
  mySerial.println("AT+CPIN?");
  delay(100); 
  mySerial.println("AT+CREG?");
  delay(100); 
  mySerial.println("AT+CGATT?");
  delay(100); 
  mySerial.println("AT+CIPSHUT");
  delay(100); 
  mySerial.println("AT+CIPSTATUS");
  delay(500); 
  mySerial.println("AT+CIPMUX=0");
  delay(500); 
  ShowSerialData();
mySerial.println("AT+CSTT=\"yellopix.mtn.co.ug\"");//start task and setting the APN,
  delay(100); 
  ShowSerialData(); 
  mySerial.println("AT+CIICR");//bring up wireless connection
  delay(300); 
  ShowSerialData(); 
  mySerial.println("AT+CIFSR");//get local IP adress
  delay(200); 
  ShowSerialData(); 
  mySerial.println("AT+CIPSPRT=0");
  delay(300); 
  ShowSerialData();  
 mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(600); 
  ShowSerialData(); 
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(400);
  ShowSerialData();  
  String str="GET https://api.thingspeak.com/update?api_key=MK1A55JIC4TF73LP&field1=" + String(temp) +"&field2="+String(hum) +"&field3="+String(gas);
  Serial.println(str);
  mySerial.println(str);//send data to remote server  
  delay(400);
  ShowSerialData(); 
  mySerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println(); 
  ShowSerialData(); 
  mySerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
}  
  }
void ShowSerialData()
{
  while(mySerial.available()!=0)
  Serial.write(mySerial.read());
  delay(5000); 
  
}