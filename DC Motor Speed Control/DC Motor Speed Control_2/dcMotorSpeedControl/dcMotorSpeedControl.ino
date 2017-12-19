//----------DC Motor Speed Control----------//

#include<LiquidCrystal.h>            //including header files for LCD(which is inbuilt in arduino...it's really Cool)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //rs=8, rw=gnd, en=9, DB4-DB7=4 to 7

int potPin = A0;                     //potPin is connected to analog input A0
int potValue;                        //Declare a variable to store the read value from A0 (0 to 1023)
int motorSpeed;                      //Declare a variable to store the converted pwm output value (0 to 255)
int FWD = 13;                        //Declare a variable to drive the motor in forward direction
int REV = 12;                        //Declare a variable to drive the motor in reverse direction

int PWM11Control = 11;               //PWM output to control the speed of the motor in fwd direction
int PWM10Control = 10;               //PWM output to control the speed of the motor in rev direction

int FWDSW = 2;                       //Switch is connected to pin no 2 to drive the motor in foward direction
int REVSW = 1;                       //Switch is connected to pin no 1 to drive the motor in reverse direction
int STOPSW = 0;                      //Switch is connected to pin no 0 to stop the motor

int FWDSWStatus = 1;                 //Declare a variable to store the forward switch status
int FWDFlag = 0;                     //Declare the variable to remember the switch pressed

int REVSWStatus = 1;                 //Declare a variable to store the reverse switch status
int REVFlag = 0;                     //Declare the variable to remember the switch pressed

int STOPSWStatus = 1;               //Declare a variable to store the stop
int STOPFlag = 0;                   //Declare the variable to remember the switch pressed

void setup()
{
  // put your setup code here, to run once:
//---------------------Motor Controls setup Code-------------------------------------------------//
  pinMode(FWDSW,INPUT);            //Make forward driven switch as input to read the value
  digitalWrite(FWDSW,HIGH);        //Activate the interanl pull up of 20k
  pinMode(REVSW,INPUT);            //Make reverser driven switch as input to read the value
  digitalWrite(REVSW,HIGH);        //Activate the interanl pull up of 20k
  pinMode(STOPSW,INPUT);           //Make stop switch as input to read the value
  digitalWrite(STOPSW,HIGH);       //Activate the interanl pull up of 20k
  
  pinMode(PWM11Control,OUTPUT);    //Make the PWM11 FWD driven speed control motor pin as output
  pinMode(PWM10Control,OUTPUT);    //Make the PWM10 REV driven speed control motor pin as output
  pinMode(FWD,OUTPUT);             //Make fwd driving pin as output 
  pinMode(REV,OUTPUT);             //Make rev driving pin as output
//------------------------------------------------------------------------------------------------//

//---------------------LCD Setup Code-------------------------------------------------------------//
  lcd.begin(16, 2);                //initializing lcd of 16*2 i.e. 16 columns and 2 rows
  lcd.setCursor(0, 0);             //cursor will be placed at column0, row0 i.e. at 80H
  //similarly, column0, row1 means C0H
  lcd.print("    WELCOME !");      //Print Welcome message on lcd
  delay(2000);                     //Delay of 3000 msec i.e. 2 sec
  lcd.clear();                     //Clear the displayed message to display something new

  lcd.print("    DC MOTOR ");      //Dispalying data on lcd by using inbuilt function of arduino
  lcd.setCursor(0, 1);             //set cursor position to column 0 and row 1
  lcd.print("  SPEED CONTROL");
  delay(1000);                     //Delay of 3000 msec i.e. 1 sec
//----------------------------------------------------------------------------------------------//
}

void loop()
{
  // put your main code here, to run repeatedly:
  potValue = analogRead(potPin);                //Read the voltage from potpin which is connected to A0
  motorSpeed = map(potValue, 0, 1023, 0, 255);  //converted value from 0 to 1023 into 0 to 255
  
//----------------------Forward Speed Control Mechanism------------------------------------------------//
  FWDSWStatus = digitalRead(FWDSW);             //Reading the switch value
  if(FWDSWStatus == LOW)                        //if the switch is pressed
  {
    FWDFlag = 1;                               //Tells me that to remember the switch is pressed
    digitalWrite(FWD,HIGH);                    //This will drive the motor in forward direction 
                 
    REVFlag = 0;                               //stopping the motor to be driven in other direction
    digitalWrite(REV,LOW);

    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("    FORWARD");
  }
  
  if(FWDFlag == 1)
    analogWrite(PWM11Control,motorSpeed);
//-----------------------------------------------------------------------------------------------------//

   
//----------------------Reverse Speed Control Mechanism------------------------------------------------//
  REVSWStatus = digitalRead(REVSW);             //Reading the switch value
  if(REVSWStatus == LOW)                        //if the switch is pressed
  {
    REVFlag = 1;                               //Tells me that to remember the switch is pressed
    digitalWrite(REV,HIGH);                    //This will drive the motor in forward direction 
                 
    FWDFlag = 0;                               //stopping the motor to be driven in other direction
    digitalWrite(FWD,LOW);

    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("    REVERSE");
  }
  
  if(REVFlag == 1)
    analogWrite(PWM10Control,motorSpeed);
//----------------------------------------------------------------------------------------------------//

//--------------------------Stop the Motor------------------------------------------------------------//
  STOPSWStatus = digitalRead(STOPSW);            //Reading the switch value
  if(STOPSWStatus == LOW)                        //if the switch is pressed
  {
    REVFlag = 0;                                //Make the reverese flag 0 because we want to stop the motor 
    digitalWrite(REV,LOW);                      //This will stop the motor
                 
    FWDFlag = 0;                                //stopping the motor to be driven in other direction
    digitalWrite(FWD,LOW);                      //This will stop the motor

    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("     STOP !");
  }
//-----------------------------------------------------------------------------------------------------//
  if(FWDFlag == 1 | REVFlag == 1)
  {
    lcd.setCursor(0, 1); 
    lcd.print(" Motor Speed:");
    lcd.println(motorSpeed);
  }

}
