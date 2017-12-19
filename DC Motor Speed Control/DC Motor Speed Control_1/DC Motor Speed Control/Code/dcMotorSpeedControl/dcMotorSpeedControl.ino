//----------DC Motor Speed Control----------//

#include<LiquidCrystal.h>           //including header files for LCD(which is inbuilt in arduino...it's really Cool)
LiquidCrystal lcd(8,9,4,5,6,7);     //rs=10, rw=gnd, en=9, DB4-DB7=4 to 7
                                    //I can give any name instead of that lcd; it's user choice
int potPin = A0;                    //Potentiometer ps connected to A0 of arduino
int motorPin = 10;                  //The PWM output pin is at 10 which controls the speed of motor
int potValue = 0;                   //This is the value received from ADC chaneel 0 which will be in the range of 0 to 1023
int motorSpeed = 0;                 //This is the converted value from 0 to 1023 into 0 to 255

//The following task will be performed only once
void setup() 
{
  pinMode(potPin,INPUT);           //Making A0 as an input port to read the voltage
  pinMode(motorPin,OUTPUT);        //Making pin no 10 as output which is PWM pin and will control the speed of motor
  lcd.begin(16,2);                 //initializing lcd of 16*2 i.e. 16 columns and 2 rows
  lcd.setCursor(0,0);              //cursor will be placed at column0, row0 i.e. at 80H
                                   //similarly, column0, row1 means C0H
  lcd.print("    WELCOME !");      //Print Welcome message on lcd
  delay(3000);                     //Delay of 3000 msec i.e. 3 sec
  lcd.clear();                     //Clear the displayed message to display something new
  
  lcd.print("    DC MOTOR ");      //Dispalying data on lcd by using inbuilt function of arduino
  lcd.setCursor(0,1);              //set cursor position to column 0 and row 1
  lcd.print("  SPEED CONTROL");
  delay(3000);                     //Delay of 3000 msec i.e. 3 sec
  
  lcd.setCursor(0,0);             //set cursor position to column 0 and row 0 i.e. at the starting position of lcd
  lcd.clear();
}

//The following task will be performed repeatedly
void loop() 
{
 potValue = analogRead(potPin);                //Read the voltage from potpin which is connected to A0
 motorSpeed = map(potValue, 0, 1023, 0, 255);  //converted value from 0 to 1023 into 0 to 255
 analogWrite(motorPin,motorSpeed);             //Apply the converted value to the motor through transistor
 lcd.setCursor(0,0);                          //cursor will be placed at column0, row0 i.e. at 80H
 lcd.print("Motor Speed:");     
 lcd.println(motorSpeed);  
}

