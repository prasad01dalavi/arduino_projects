 

/*

  Tutorial 4 – Digital Input and Output Game

  In this game, the LED will loop from white, yellow, green, red

  then back to white.  The goal is to press the push button at the exact

  moment when the green LED is ON. Each time you got it right, the LED

  will speed up and the difficulty will increase.

 

*/

int currentLED = 2;

int delayValue = 200;

 

void setup() {

  // initialize digital pin 12 as input;

  pinMode(12, INPUT);   // button input

 

  // initialize digital pin 2 to 5 as output:

  pinMode(2, OUTPUT);   // white LED

  pinMode(3, OUTPUT);   // yellow LED

  pinMode(4, OUTPUT);   // green LED

  pinMode(5, OUTPUT);   // red LED

}

 

int checkInput() { 

  if (digitalRead(12) == 0) {

     return 1;

  } else {

    return 0;

  }

}

 

void loop(){

  // Check if the button is press at the right moment

  if (digitalRead(12) == 0) {

     if (currentLED == 4) {

       // Blink the correct (green) LED

       digitalWrite(4, HIGH);

       delay(200);

       digitalWrite(4, LOW);

       delay(200);

       digitalWrite(4, HIGH);

       delay(200);

       digitalWrite(4, LOW);

       delay(200);

 

// Speed up the LEDs

       delayValue = delayValue - 20; 

 

    } else {

       // Blink the wrong LED

       digitalWrite(currentLED, HIGH);

       delay(200);

       digitalWrite(currentLED, LOW);

       delay(200);

       digitalWrite(currentLED, HIGH);

 

       delay(200);

       digitalWrite(currentLED, LOW);

       delay(200);

    }

  }

  // Loop LED from white –> yellow –> green –> red

  digitalWrite(currentLED, HIGH);

  delay(delayValue);

  digitalWrite(currentLED, LOW);

  delay(delayValue);

  currentLED = currentLED + 1;

  if (currentLED > 5) {

     currentLED = 2;

  }

}

 
