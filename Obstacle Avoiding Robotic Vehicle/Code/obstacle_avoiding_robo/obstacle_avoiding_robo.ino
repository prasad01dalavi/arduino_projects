//----------About L293D---------------//
/* The Pin no 8 of this IC provides supply voltage to the motor(s)
 *  if we give high voltage it will run the motor faster
 *  EN1(1) and EN2(9) are connected to 5V to enable the motor driver at output1 and output2 respectively
 *  If IN1(2)=1 and IN2(7)=0 ==> o/p1(3) is activated and o/p2(6) remains grounded ( This can be our forward case )
 *  If IN1(2)=0 and IN2(7)=1 ==> o/p1(3) is remains grounded and o/p2(6) is activated ( This can be our reverse case )
 *  Simularly, If IN3(10)=1 and IN4(15)=0 ==> o/p3(11) is activated and o/p4(14) remains grounded ( This can be our forward case )
 *  If IN3(10)=0 and IN4(15)=1 ==> o/p3(11) is remains grounded and o/p4(14) is activated ( This can be our reverse case )
 */


#include<LiquidCrystal.h>           //including header files for LCD(which is inbuilt in arduino...it's really Cool)
LiquidCrystal lcd(10,9,4,5,6,7);    //rs=10, rw=gnd, en=9, DB4-DB7=4 to 7
                                    //I can give any name instead of that lcd; it's user choice

int trigPin=13;   //trigger pin is used to provide a pulse to transmit the ultrasonic signal
int echoPin=11;   //echo pin of ultrasonic sensor is used to receive the reflected signal
float pingTime;   //store the time between tx and rx of the signal
int distance;     //Distance to be calculated in cm

//declaring variables for input pins of L293D
int in1=0;        //left motor controls   
int in2=1;

int in3=2;        //right motor controls   
int in4=3;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);  //it is tx the signal therefore output
  pinMode(echoPin,INPUT);   //it's rx the signal therefore input
  
  pinMode(in1,OUTPUT);  
  pinMode(in2,OUTPUT); 
  pinMode(in3,OUTPUT); 
  pinMode(in4,OUTPUT); 

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("    Obstacle");
  lcd.setCursor(0,1);
  lcd.print(" Avoiding Robot");
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

  if(distance>20)           //forward
  {
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);

      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
  }

  if(distance<20)             //right
  {
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);

      digitalWrite(in3,LOW);
      digitalWrite(in4,HIGH);
  } 
}
