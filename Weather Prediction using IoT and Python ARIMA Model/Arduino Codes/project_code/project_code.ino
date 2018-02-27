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
 
#include <SoftwareSerial.h>  			 // to use arduino software uart
#include <stdlib.h>  					 // for string operations
#include <LiquidCrystal.h>               // Include LCD Library
#include <DHT.h>                         // Include library for DHT11 Sensor
#include <Wire.h>                        // I2C lcd Protocol Library
#include <Adafruit_BMP085.h>             // Include BMP180 Sensor Library

// function prototypes
void thingspeak_upload();                               // Function prototype (this function is used for uploading the data on thingspeak cloud)
double dewPointFast(double celsius, double humidity);   // Function Prototype (this function is used for calculating the dew point0

// variables declaration
int ledPin = 13;                         // led(inbuilt) is for indicating the upload process
int dht_pin = 2;                         // Declaring a variable for pin where DHT11 data pin has been connected
float temperature_celsius;               // Declare a variable to hold the data of the temperature in celsius
float humidity_percentage;               // Declare a variable to hold the data of the humidity percentage 
float dPC;                               // Declare a variable to store the dew point value in Celsius
int pressure;                            // Declare a variable to store the pressure read from bmp in pascals
String apiKey = "RFNJ9N1B8XSZUYQY";      // thingspeak API key for authenticate uploding of data on cloud

SoftwareSerial ser(10, 11);              // pin #10 = Tx and pin #11 = Rx (connect Tx of ESP to pin 10 and Rx of ESP to 11)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);     // Create an object of lcd
DHT dht(dht_pin, DHT11);                 // Create an object of DHT11 connected to pin no. 2
Adafruit_BMP085 bmp;                     // Create an object of BMP library

void setup() 
{                
  pinMode(ledPin, OUTPUT);  			// making led pin as output
  lcd.begin(16, 2);         			// set up the LCD's number of columns and rows   
  Serial.begin(9600);       			// setting hardware uart baudrate and starting serial communication
  ser.begin(9600);          			// setting software uart baudrate and starting serial communication  
  dht.begin();              			// Start the DHT sensor reading program  
  ser.println("AT+RST");   				// restart ESP8266
  
// displayig initial introduction of project
  lcd.print("Weather Forecast !");      // Print a message to the LCD.
  delay(3000);                          // Delay of 5 second
  lcd.clear();                          // Clear the contents displayed on lcd
}

void loop() 
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)  

  temperature_celsius = dht.readTemperature(); 			 // Declare a variable as well as read the temperature value in *c
  lcd.clear();  										 // clear the lcd screen
  lcd.setCursor(0,0);  								     // Set lcd cursor to 1st row 1st column
  lcd.print("  Temperature");                            // print the message on lcd
  lcd.setCursor(0,1);                                    // select 1st column second row i.e. second line(row) of 16x2 lcd
  lcd.print("    ");   									 // achieving center of second line
  lcd.print(temperature_celsius);                        // display temperature value on lcd
  lcd.print(" *C ");                                     // read temperature is in degree celsius
  delay(2000);                                           // Delay for showing that temperature value on lcd
  
  humidity_percentage = dht.readHumidity();     		 // Declare a variable as well as read the Humid value in %
  lcd.clear();  										 // clear the lcd screen
  lcd.setCursor(0,0);  									 // Set lcd cursor to 1st row 1st column
  lcd.print("    Humidity");							 // print the message on lcd			
  lcd.setCursor(0,1);									 // select 1st column second row i.e. second line(row) of 16x2 lcd
  lcd.print("     ");   								 // achieving center of second line
  lcd.print(humidity_percentage);						 // display humidity value on lcd
  lcd.print("%");                                        // humidity value is expressed in percentage
  delay(2000);											 // Delay for showing that humidity value on lcd
  
  
  dPC = dewPointFast(temperature_celsius, humidity_percentage);   // Call the function to calculate dew point in celcius
  lcd.clear(); 													  // clear the lcd screen
  lcd.setCursor(0,0);  										      // Set lcd cursor to 1st row 1st column
  lcd.print("    Dew Point");								      // print the message on lcd	
  lcd.setCursor(0,1);                                             // select 1st column second row i.e. second line(row) of 16x2 lcd  
  lcd.print("     ");  										      // achieving center of second line
  lcd.print(dPC);												  // display the dew point value (can be -ve also)
  lcd.print(" *C ");											  // dew point value is in degree celsius
  delay(2000);													  // Delay for showing that dew point value on lcd

  pressure = bmp.readPressure();								  // Declare a variable as well as read the pressure value in pascals
  lcd.clear();  												  // clear the lcd screen
  lcd.setCursor(0,0);  											  // Set lcd cursor to 1st row 1st column
  lcd.print("    Pressure");									  // print the message on lcd
  lcd.setCursor(0,1);											  // select 1st column second row i.e. second line(row) of 16x2 lcd
  lcd.print("     ");   										  // achieving center of second line
  lcd.print(pressure);											  // display the pressure value on lcd
  lcd.print(" Pa");												  // pressure value is measure in pascals from BMP180(temperature and pressure) Sensor
  delay(2000);                  								  // Delay for showing that dew point value on lcd

  // Now upload these measure values on thingspeak cloud so calling the function -> thingspeak_upload() to upload these values
  thingspeak_upload(temperature_celsius, humidity_percentage, dPC, pressure); 
}


void thingspeak_upload(float temp, float humid, float dew, int pressure)
{
  // blink LED on board to indicate that data uploading process has been started
  digitalWrite(ledPin, HIGH);   
  delay(500);               
  digitalWrite(ledPin, LOW);
  
  // Show uploading process has been started on lcd display
  lcd.clear();				         // first clear the lcd display
  lcd.setCursor(0,0);  		         // Set lcd cursor to 1st row 1st column
  lcd.print("Uploading Data..")      // display the uploading message on lcd
  delay(1000);                       // just wait a second                   

  // convert to string so that we can upload that value on thingspeak
  char tempBuf[16];   							  // declaring a buffer array for string conversion of the value
  String strTemp = dtostrf(temp, 4, 1, tempBuf);  // this will convert the number using buffer and stdlib function

  // similarly converting the other values into the string 
  char humidBuf[16];
  String strHumid = dtostrf(humid, 4, 1, humidBuf);

  char dewBuf[16];
  String strDew = dtostrf(dew, 4, 1, dewBuf);

  char pressureBuf[16];
  String strPressure= dtostrf(pressure, 4, 1, pressureBuf);

  // printing the string values on serial monitor to verify that the conversion is successfully done
  Serial.println(strTemp);
  Serial.println(strHumid);
  Serial.println(strDew);
  Serial.println(strPressure);

  // now starting the process of uploading the data by first making the TCP connection to the cloud 
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; 				// api.thingspeak.com i.e. Thingspeak cloud ip address
  cmd += "\",80";							// specifying the port number i.e. actually HTTP port
  ser.println(cmd);                         // sending this connection information to ESP8266 which will forward it to cloud using wifi internet connection

  if(ser.find("Error"))					    // If any error accured while forwarding the connection request to cloud
  {					
    Serial.println("AT+CIPSTART error");    // The connection error will be displayed on serial monitor 
 	// display the error message on lcd also
  	lcd.clear();				            // first clear the lcd display
  	lcd.setCursor(0,0);  		            // Set lcd cursor to 1st row 1st column
  	lcd.print("Uploading Failed")           // display the uploading message on lcd
  	delay(2000);
    return;
  }

  // prepare GET string
  String getStr = "GET /update?api_key=";   // Uploading data using Thigspeak API (Application Programming Interface)
  getStr += apiKey;                         // attaching write api key abtained from thingspeak 
  getStr +="&field1=";                      // attach the field number to which we want to upload the value here it is field 1 
  getStr += String(strTemp);                // uploading temperature value to thingspeak field1
  getStr +="&field2=";						// want to use field2 
  getStr += String(strHumid);               // uploading humidity value in % on field2
  getStr +="&field3=";                      // want to use field3
  getStr += String(strDew);                 // uploading dew point in celsius value on field3
  getStr +="&field4=";                      // want to use field4
  getStr += String(strPressure);            // uploading pressure in pascal on field4 of thingspeak
  getStr += "\r\n\r\n";  					// press 'enter' using software command (carriage return and new line)

  // before sending the GET string we will inform ESP822 about the length of data that we want to upload on thingspeak
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());           // measuring the length of our GET string
  ser.println(cmd);                         // now sending 'AT+CIPSEND=<length of GET string>' to ESP8266 
  Serial.println(cmd);                      // also displaying it on serial monitor

  if(ser.find(">"))                         // if we receive '>' from ESP8266 means ESP8266 has made successful connection with thingspeak cloud
  {
    ser.print(getStr);                      // GET request will be forwared by ESP8266 to upload the data on thingspeak
    Serial.println(getStr);                 // Display the GET Request string on serial monitor for debugging and to know field the values which will be uploaded
    // display the success message on lcd also
  	lcd.clear();				            // first clear the lcd display
  	lcd.setCursor(0,0);  		            // Set lcd cursor to 1st row 1st column
  	lcd.print("Upload Success!")           // display the uploading message on lcd
  	delay(2000);
  }
  else                                      // else it means ESP8266 has lost the connection to thingspeak cloud 
  { 
    ser.println("AT+CIPCLOSE");             // As ESP8266 has lost the connection, we need to close the connection using this command
    Serial.println("Closing the connection...");          // Display the message of closing the connection on serial monitor

  }

  // thingspeak needs 15 sec delay between updates
  ser.println("AT+RST");                            // Restart ESP8266 for new value upload
  delay(30000);                                     // giving 30 sec delay for uploading thingspeak data
}

double dewPointFast(double celsius, double humidity)    // Function definition to calculate Dew with the help of temperature and humidity 
{
double a = 17.271;    // Coefficient to calculate dew point
double b = 237.7;
double temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
double Td = (b * temp) / (a - temp);    // Dew point temperature
return Td;
}

