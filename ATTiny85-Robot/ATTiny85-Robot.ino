/*******************************************************************
 *Zippy Robot by www.varind.com 2014 
 *based on:
 *
 *SoftServo sketch for Adafruit Trinket.
 * 
 * Required library is the Adafruit_SoftServo library
 * available at https://github.com/adafruit/Adafruit_SoftServo
 * The standard Arduino IDE servo library will not work with 8 bit
 * AVR microcontrollers like Trinket and Gemma due to differences
 * in available timer hardware and programming. We simply refresh
 * by piggy-backing on the timer0 millis() counter
 * 
 *******************************************************************/

#include <Adafruit_SoftServo.h>  // SoftwareServo (works on non PWM pins)
#define SERVO1PIN 1 //Waist
#define SERVO2PIN 2 //Legs
#define trigPin 3
#define echoPin 4

Adafruit_SoftServo myServo1, myServo2;  //create TWO servo objects
long duration, distance, previous_distance, previous_duration;

void setup() {
  // Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(0, OUTPUT); 
  myServo1.attach(SERVO1PIN);
  myServo1.write(90);
  myServo2.attach(SERVO2PIN);
  myServo2.write(90);
  delay(15);
    digitalWrite(0, LOW);
}

void loop()  {

  ping(5);
  if (distance > 20.0){  // Walk forward
  digitalWrite(0, LOW);
    myServo1.write(60);  // Move Waist
    delay(100);  
    myServo2.write(50);  // Move Legs
    delay(150);
    myServo1.write(95);
    delay(100);  
    myServo2.write(140);  
    delay(100);
  }
  ping(5);
  if (distance <= 20.0 && distance > 0.0){  // Walk backward
    digitalWrite(0, HIGH);
    myServo1.write(90);  
    myServo2.write(90);  
    delay(1000);
    for (int x=0; x<5; x++){
      myServo1.write(30);
      delay(50);  
      myServo2.write(140);  
      delay(150);
      myServo1.write(130);
      delay(50);  
      myServo2.write(50);  
      delay(150);
    }
    myServo1.write(90);  
    myServo2.write(90);  
    delay(2000);
  }
}

void ping (int pings){
  duration = 0;
  for (int x=0; x < pings; x++){ 
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = duration + pulseIn(echoPin, HIGH);
    delayMicroseconds(10);
  }
  duration=duration/pings;
  distance = (duration/2) / 29.1;
  previous_duration = duration;
}


volatile uint8_t counter = 0;

SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    myServo1.refresh();
    myServo2.refresh();
  }
  
   
}


