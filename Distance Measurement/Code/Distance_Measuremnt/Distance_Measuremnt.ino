#include<LiquidCrystal.h>           //including header files for LCD(which is inbuilt in arduino...it's really Cool)
LiquidCrystal lcd(10,9,4,5,6,7);    //rs=10, rw=gnd, en=9, DB4-DB7=4 to 7
                                    //I can give any name instead of that lcd; it's user choice

int trigPin=13;   //trigger pin is used to provide a pulse to transmit the ultrasonic signal
int echoPin=11;   //echo pin of ultrasonic sensor is used to receive the reflected signal
float pingTime;   //store the time between tx and rx of the signal
int distance;     //Distance to be calculated in cm

void setup() 
{
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);  //it is tx the signal therefore output
  pinMode(echoPin,INPUT);   //it's rx the signal therefore input
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("   Distance ");
  lcd.setCursor(0,1);
  lcd.print("  Measurement ");
  delay(2000);
  
  lcd.clear();                      //lcd will be cleared i.e. no data will be there on lcd 
  lcd.setCursor(0,0);
  lcd.print("Distance:");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin,LOW);    //Set trigger pin to low
  delayMicroseconds(2000);    //very short delay of 2000 usec or 2 msec
  digitalWrite(trigPin,HIGH);   //making trigpin high
  delayMicroseconds(10);      //very very short delay for high pulse
  digitalWrite(trigPin,LOW);    //pulse transmission is finished
                              //i.e. we have transmitted low-high-low pulse
  pingTime=pulseIn(echoPin,HIGH);    //measuring the time to rx the tx pulse
  
  distance=33000*(pingTime/1000000);      //by formula, Distance=speed*time
  distance=distance/2;
  
  lcd.setCursor(9,0);
  lcd.print(distance);
  lcd.print(" cm");
}
