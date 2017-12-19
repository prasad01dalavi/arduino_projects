int i1=8,i2=9,i3=10,i4=11;    //motor driver ic input signals to drive motors
String s;                     //declaring a string type variable to receive the signals

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);       //starting serial communication for bluetooth module HC-05 
  //making arduino pins as output for the motor control signals
  pinMode(i1,OUTPUT);
  pinMode(i2,OUTPUT);
  pinMode(i3,OUTPUT);
  pinMode(i4,OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  while(Serial.available()==0);     //waiting to receive a signal
  s=Serial.readString();        //stroring the signal recived from BT app

  if(s=="F")
  {
    //vehicle will go in forward direction
    digitalWrite(i1,HIGH);
    digitalWrite(i2,LOW);
    digitalWrite(i3,HIGH);
    digitalWrite(i4,LOW);
  }

  if(s=="L")
  {
    //vehicle will go in left direction
    digitalWrite(i1,LOW);
    digitalWrite(i2,LOW);
    digitalWrite(i3,HIGH);
    digitalWrite(i4,LOW);   
  }

  if(s=="R")
  {
    //vehicle will go in right direction
    digitalWrite(i1,HIGH);
    digitalWrite(i2,LOW);
    digitalWrite(i3,LOW);
    digitalWrite(i4,LOW);     
  }

  if(s=="B")
  {
    //vehicle will go in backward direction
    digitalWrite(i1,LOW);
    digitalWrite(i2,HIGH);
    digitalWrite(i3,LOW);
    digitalWrite(i4,HIGH);
  }
  
}
