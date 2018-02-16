/*
 * ESP8266 Connections
 * 1. Rx   2. 3.3V
 * 3. NC   4. NC
 * 5. NC   6. 3.3V
 * 7. GND  8. Tx  
 */
 
#include <SoftwareSerial.h>
#include <stdlib.h>  // for string operations

int ledPin = 13;  // led(inbuilt) is for indicating the upload process

float my_data = 50;
String apiKey = "49I73T91FC0WX3DU"; // thingspeak API key

SoftwareSerial ser(10, 11); // pin #10 = Tx and pin #11 = Rx

void setup() {                
  pinMode(ledPin, OUTPUT);    
  Serial.begin(9600);  
  ser.begin(9600);        // start software serial  
  ser.println("AT+RST");  // restart ESP8266
}

void loop() {

  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(500);               
  digitalWrite(ledPin, LOW);
  
  my_data++;  // change (increment) the value of data to upload on thingspeak
  float temp = my_data;

  // convert to string
  char buf[16];
  String strTemp = dtostrf(temp, 4, 1, buf);

  Serial.println(strTemp);

  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);

  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(strTemp);
  getStr +="&field2=";
  getStr += String(strTemp);
  getStr +="&field3=";
  getStr += String(strTemp);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
    Serial.println(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

  // thingspeak needs 15 sec delay between updates
  ser.println("AT+RST");
  delay(30000);  // giving 30 sec delay for updates
}
