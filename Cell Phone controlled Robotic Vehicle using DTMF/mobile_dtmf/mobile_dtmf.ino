int i1=8,i2=9,i3=10,i4=11;    //motor driver ic input signals to drive motors
int q1=2,q2=3,q3=4,q4=5;      //dtmf signals 
int sq1,sq2,sq3,sq4;          //status of the dtmf signals

void setup() 
{
  // put your setup code here, to run once:
  pinMode(i1,OUTPUT);
  pinMode(i2,OUTPUT);
  pinMode(i3,OUTPUT);
  pinMode(i4,OUTPUT);

  pinMode(q1,INPUT);
  pinMode(q2,INPUT);
  pinMode(q3,INPUT);
  pinMode(q4,INPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  sq1=digitalRead(q1);
  sq2=digitalRead(q2);
  sq3=digitalRead(q3);
  sq4=digitalRead(q4);

  if(sq4==LOW && sq3==LOW && sq2==HIGH && sq1==LOW)     //when 0010 i.e. '2' is pressed
  {
    //vehicle will go in forward direction
    digitalWrite(i1,HIGH);
    digitalWrite(i2,LOW);
    digitalWrite(i3,HIGH);
    digitalWrite(i4,LOW);    
  }

  if(sq4==LOW && sq3==HIGH && sq2==LOW && sq1==LOW)     //when 0100 i.e. '4' is pressed
  {
    //vehicle will go in left direction
    digitalWrite(i1,LOW);
    digitalWrite(i2,LOW);
    digitalWrite(i3,HIGH);
    digitalWrite(i4,LOW);    
  }

  if(sq4==LOW && sq3==HIGH && sq2==HIGH && sq1==LOW)     //when 0110 i.e. '6' is pressed
  {
    //vehicle will go in right direction
    digitalWrite(i1,HIGH);
    digitalWrite(i2,LOW);
    digitalWrite(i3,LOW);
    digitalWrite(i4,LOW);    
  }

   if(sq4==HIGH && sq3==LOW && sq2==LOW && sq1==LOW)     //when 1000 i.e. '8' is pressed
  {  
    //vehicle will go in backward direction
    digitalWrite(i1,LOW);
    digitalWrite(i2,HIGH);
    digitalWrite(i3,LOW);
    digitalWrite(i4,HIGH);    
  }
}
