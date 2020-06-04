#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
float lat = 28.5458,lon = 77.1703; //creating variable for latitude and longitude object
SoftwareSerial gpsSerial(3,4); //rx,tx
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);// A0 through A5 are analog pins on the lcd
TinyGPS gps; //creating gps object
void setup() {
  Serial.begin(9600); // connecting serial
  Serial.println("The GPS received signal");
  gpsSerial.begin(9600); //connecting gps sensor
  lcd.begin(16,2);

}

void loop() {
  while(gpsSerial.available()){ //checking for gps data
    if(gps.encode(gpsSerial.read())) //encoding gps data
    {
      gps.f_get_position(&lat,&lon); //getting latitude and longitude
      //displaying position

      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("GPS Signal");
      //Serial.print("Position:");
      //Serial.print("Latitude:");
      //Serial.print(lat,6);
      //Serial.print(";");
      //Serial.print("Longitude:");
      //Serial.print(lon,6);

      lcd.setCursor(1,0);
      lcd.print("LAT:");
      lcd.setCursor(5,0);
      lcd.print(lat);
      //Serial.print(lat);
      //Seriall.print(" ");

      lcd.setCursor(0,1);
      lcd.print(",LON:");
      lcd.setCursor(5,1);
      lcd.print(lon);
      }
    }

    String latitude = String(lat,6);
    String longitude = String(lon,6);
    Serial.println(latitude+";"+longitude);
    delay(1000);
    

}
