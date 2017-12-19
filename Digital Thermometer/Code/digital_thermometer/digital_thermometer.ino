#include<LiquidCrystal.h>           //including header files for LCD(which is inbuilt in arduino...it's really Cool)
LiquidCrystal lcd(10,9,4,5,6,7);    //rs=10, rw=gnd, en=9, DB4-DB7=4 to 7 pins of arduino
                                    //I can give any name instead of that lcd; it's user choice
int n=A0;                         // assigning n variable to A0 channel of adc
                                  //there are total 6 channels to read analog voltages(A0 to A5)
int read_value;                          //declaring the variable to store the read value(which will be in between 0 to 1023)
int temperature;                  //voltage is in decimal point(any value in between 0V to 5V)

void setup() 
{
  // put your setup code here, to run once:
  lcd.begin(16,2);                 //initializing lcd of 16*2 i.e. 16 columns and 2 rows
  lcd.setCursor(4,0);              //cursor will be placed at column0, row0 i.e. at 80H
                                   //similarly, column0, row1 means C0H
  lcd.print("DIGITAL");            //Dispalying data on lcd by using inbuilt function of arduino
  lcd.setCursor(2,1);
  lcd.print("THERMOMETER");        //Dispalying data on lcd by using inbuilt function of arduino
  delay(2000);
}

void loop() 
{
  // put your main code here, to run repeatedly:
   read_value=analogRead(n);                       //reading the analog value at pin A0
   temperature=(.5/1023.)*read_value*1000;    //formula to convert the value between 0 to 1023 into 0 to 1
                                           //writing 1. and 1023. to tell the arduino that the result should be in float
   lcd.print("                ");          //clearing the display  
   lcd.setCursor(0,1);
   lcd.print("                ");          //clearing the display  
   lcd.setCursor(0,0);
   lcd.print("Temperature:");             //Dispalying data on lcd by using inbuilt function of arduino                           
   lcd.setCursor(12,0);
   lcd.print(temperature);
   lcd.setCursor(14,0);
   lcd.print("C");
}
