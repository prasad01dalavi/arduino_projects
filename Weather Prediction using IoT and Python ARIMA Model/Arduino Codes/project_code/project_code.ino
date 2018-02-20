/*
The LCD Circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC or VDD pin to 5V
 * 10K resistor between +5V and gnd
 * wiper to LCD VO or VEE pin (pin 3)
*/

/*
The DHT11 Circuit:
 * VDD pin of DHT11 is connected to +5V
 * GND pin of DHT11 is connected to gnd
 * DATA pin of DHT11 is connected to pin 2 of arduino uno 
 */
 
/*
 * ESP8266 Connections
 * 1. Rx   2. 3.3V
 * 3. NC   4. NC
 * 5. NC   6. 3.3V
 * 7. GND  8. Tx  
 */
 
#include <SoftwareSerial.h>  // to use arduino software uart
#include <stdlib.h>  // for string operations
#include <LiquidCrystal.h>               // Include LCD Library
#include <DHT.h>                         // Include library for DHT11 Sensor
#include <Wire.h>                        // I2C lcd Protocol Library
#include <Adafruit_BMP085.h>             // Include BMP180 Sensor Library

// function prototypes
void thingspeak_upload();
double dewPointFast(double celsius, double humidity);   // Function Prototype (this function is used for calculating the dew point0

// variables declaration
int ledPin = 13;                         // led(inbuilt) is for indicating the upload process
int dht_pin = 2;                         // Declaring a variable for pin where DHT11 data pin has been connected
float temperature_celsius;               // Declare a variable to hold the data of the temperature in celsius
float humidity_percentage;               // Declare a variable to hold the data of the humidity percentage 
float dPC;                               // Declare a variable to store the dew point value in Celsius
int pressure;                            // Declare a variable to store the pressure read from bmp in pascals
String apiKey = "RFNJ9N1B8XSZUYQY";      // thingspeak API key

SoftwareSerial ser(10, 11);              // pin #10 = Tx and pin #11 = Rx (connect Tx of ESP to pin 10 and Rx of ESP to 11)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);     // Create an object of lcd
DHT dht(dht_pin, DHT11);                 // Create an object of DHT11 connected to pin no. 2
Adafruit_BMP085 bmp;                     // Create an object of BMP library

void setup() 
{                
  pinMode(ledPin, OUTPUT);  // making led pin as output
  lcd.begin(16, 2);         // set up the LCD's number of columns and rows   
  Serial.begin(9600);       // setting hardware uart baudrate and starting serial communication
  ser.begin(9600);          // setting software uart baudrate and starting serial communication  
  dht.begin();              // Start the DHT sensor reading program
  
  ser.println("AT+RST");   // restart ESP8266
  
// displayig initial introduction of project
  lcd.print("Weather Forecast !");      // Print a message to the LCD.
  delay(3000);                          // Delay of 5 second
  lcd.clear();                          // Clear the contents displayed on lcd
}

void loop() 
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)  

  temperature_celsius = dht.readTemperature();  // Declare a variable as well as read the temperature value in *c
  lcd.clear();  //clear the lcd screen
  lcd.setCursor(0,0);  
  lcd.print("  Temperature");
  lcd.setCursor(0,1);
  lcd.print("    ");   //achieving center of second line
  lcd.print(temperature_celsius);
  lcd.print(" *C ");
  delay(2000);
  
  humidity_percentage = dht.readHumidity();     // Declare a variable as well as read the Humid value in %
  lcd.clear();  //clear the lcd screen
  lcd.setCursor(0,0);  
  lcd.print("    Humidity");
  lcd.setCursor(0,1);
  lcd.print("     ");   //achieving center of second line
  lcd.print(humidity_percentage);
  lcd.print("%");
  delay(2000);
  
  dPC = dewPointFast(temperature_celsius, humidity_percentage);   // Call the function to calculate dew point in celcius
  lcd.clear();  //clear the lcd screen
  lcd.setCursor(0,0);  
  lcd.print("    Dew Point");
  lcd.setCursor(0,1);
  lcd.print("     ");   //achieving center of second line
  lcd.print(dPC);
  lcd.print(" *C ");
  delay(2000);

  pressure = bmp.readPressure();
  lcd.clear();  //clear the lcd screen
  lcd.setCursor(0,0);  
  lcd.print("    Pressure");
  lcd.setCursor(0,1);
  lcd.print("     ");   //achieving center of second line
  lcd.print(pressure);
  lcd.print(" Pa");
  delay(2000);                  
 // thingspeak_upload(temperature_celsius, humidity_percentage, dPC, pressure);
}


void thingspeak_upload(float temp, float humid, float dew, int pressure)
{
  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(500);               
  digitalWrite(ledPin, LOW);
  
  // convert to string
  char tempBuf[16];   // declaring a buffer array
  String strTemp = dtostrf(temp, 4, 1, tempBuf);  // this will convert the number using buffer and stdlib function

  char humidBuf[16];
  String strHumid = dtostrf(humid, 4, 1, humidBuf);

  char dewBuf[16];
  String strDew = dtostrf(dew, 4, 1, dewBuf);

  char pressureBuf[16];
  String strPressure= dtostrf(pressure, 4, 1, pressureBuf);

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
  getStr += String(strHumid);
  getStr +="&field3=";
  getStr += String(strDew);
  getStr +="&field4=";
  getStr += String(strPressure);
  getStr += "\r\n\r\n";  //press 'enter' using software command (carriage return and new line)

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

double dewPointFast(double celsius, double humidity)    // Function definition to calculate Dew with the help of temperature and humidity 
{
double a = 17.271;    // Coefficient to calculate dew point
double b = 237.7;
double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
double Td = (b * temp) / (a - temp);    // Dew point temperature
return Td;
}

