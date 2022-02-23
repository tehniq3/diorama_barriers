/* Automatic barriers for railways 
 
 based on Sweep -  http://www.arduino.cc/en/Tutorial/Sweep
 by BARRAGAN <http://barraganstudio.com> - This example code is in the public domain.
 modified 8 Nov 2013 by Scott Fitzgerald

 sketch adapted by Nicu FLORICA (niq_ro) for railways barrier(s)

 ver.2.0 - added second servo
 ver.3.0 - switch to Servotimer2 librry - https://github.com/nabontra/ServoTimer2
 ver.3.a - removed FOR loops
 ver.3.a.1 - reused classic Servo library
 ver.3.a.7 - disconnect servo when play sound + added flashing white light
 ver.4.0 - added an IR sensor, disconnect switches
 ver.4.a - added second IR sensor
 ver.5.0 - added 2 more IR sensors
*/

#define servo1 9    // servo no.1 control
#define servo2 10   // servo no.2 control
#define difuzor 12  // passive piezo speaker
#define ledrosu1 4  // red led no.1
#define ledrosu2 5  // red led no.2
#define ledalb 6    // white led
#define senzor1 7   // IR sensor no.1
#define senzor2 8   // IR sensor no.2
#define senzor3 2   // IR sensor no.3
#define senzor4 3   // IR sensor no.4

#include <Servo.h>
Servo myservo1;  // create servo object to control first servo
Servo myservo2;  // create servo object to control second servo

int pos = 0;     // variable to store the servo position
int pos0 = 0;    // variable to store previous servo postion
int posmin = 9;  // minimum position of the servo
int posmax = 61; //81 // maximum position of the servo
// posmin + posmax <= 90

int minipauza = 15;  // small pause for step of barrier (default = 15)
int pauzamica = 250; // small pause between some steps
byte directie = 0;  // 0 - stop, 1 - raise (up), 2 - lower (down)
unsigned long tpcomanda;
int timp1, timp2;
unsigned long tpmillis = 0;
int pitch = 400;
unsigned long tppauzasunet = 10;
unsigned long tpsunet = 0;
int sunet = 200;
byte trecere = 0;
byte trecere0 = 0;
unsigned long tpoprire = 5000;  // delay in ms for raise again the barrier(s)
unsigned long tptrecere = 0;


void setup() {
  pinMode(senzor1, INPUT);
  pinMode(senzor2, INPUT);
  pinMode(senzor3, INPUT);
  pinMode(senzor4, INPUT);

  pinMode(ledrosu1, OUTPUT);
  pinMode(ledrosu2, OUTPUT);
  pinMode(ledalb, OUTPUT);
  digitalWrite(ledrosu1, LOW);
  digitalWrite(ledrosu2, LOW);
  digitalWrite(ledalb, HIGH);

 pinMode(difuzor, OUTPUT); 
 
 myservo1.attach(servo1);
 myservo2.attach(servo2);
    myservo1.write(posmin);              // tell servo to go to position in variable 'posmin'
    delay(minipauza/2);  
    myservo2.write(posmax); 
    delay(minipauza/2);   
    myservo1.detach();  // detaches the servo from pin 9
    myservo2.detach(); 
    pos = posmin;
}

void loop() {

if (trecere0 == 0)  // if barriers are open and a train enter in barriers zone
{
if ((digitalRead(senzor1) == LOW) or (digitalRead(senzor2) == LOW) or (digitalRead(senzor3) == LOW) or (digitalRead(senzor4) == LOW))
{
  trecere = 1;
  tptrecere = millis();
  directie = 1;
//  pos = posmin;
pos = pos0;  // 
  delay(pauzamica);
   for (int i=1; i < 100; i++)
    {
     int pitch = 200 + i *20;
     tone(difuzor, pitch);
     delay(10); 
    }
  noTone(difuzor);

  tpcomanda = millis();
  myservo1.attach(servo1);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(servo2);
}
}

// if train(s) is still in barriers zone
if ((digitalRead(senzor1) == LOW) or (digitalRead(senzor2) == LOW) or (digitalRead(senzor3) == LOW) or (digitalRead(senzor4) == LOW))
{
  tptrecere = millis();
}


if ((trecere == 1) and (millis() - tptrecere > tpoprire))  // first step when barriers rise 
{
  directie = 2;
  pos = posmax;
  delay(pauzamica);
  tpcomanda = millis();
  noTone(difuzor);
  myservo1.attach(servo1);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(servo2); 
  trecere = 0;
}


  if ((directie == 1) and (millis() - tpcomanda > minipauza))  // barriers rise
  { 
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(minipauza/2);  
    myservo2.write(90-pos); 
    delay(minipauza/2);                // waits 15ms for the servo to reach the position
  pos+=1;
  if (pos >= posmax)
  {
    directie = 0;
    myservo1.detach();  // detaches the servo from pin 9
    myservo2.detach();
  }
  tpcomanda = millis();
  }

  if ((directie == 2) and (millis() - tpcomanda > minipauza))  // barriers lower
  {
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(minipauza/2);  
    myservo2.write(90-pos); 
    delay(minipauza/2);               // waits 15ms for the servo to reach the position
    pos-=1;
    if (pos <= posmin) 
    {
      directie = 0;
      myservo1.detach();  // detaches the servo from pin 9
      myservo2.detach();
    }
    tpcomanda = millis();
  }


if (pos > posmin) // if barriers are raised, red leds flash
{
  digitalWrite(ledalb, LOW); 
  tpmillis = millis()/1000;
  if (tpmillis%2 == 0)
  {
   digitalWrite(ledrosu1, HIGH);
   digitalWrite(ledrosu2, LOW);
  }
  else            
  {
   digitalWrite(ledrosu1, LOW);
   digitalWrite(ledrosu2, HIGH);
  }
}
else  // if barriers are open, white led flash
{
 //digitalWrite(ledalb, HIGH);  
 digitalWrite(ledrosu1, LOW);
 digitalWrite(ledrosu2, LOW);
 tpmillis = millis()/1000;
 if (tpmillis%2 == 0)
  {
   digitalWrite(ledalb, HIGH);
  }
  else
  {
   digitalWrite(ledalb, LOW);
  }
 
}

if ((millis() - tppauzasunet > tpsunet) and (pos > posmin))  // sounds / audible warning
{ 
  if (directie == 0)
  {
  tone(difuzor, sunet);
  tpsunet = millis();
  sunet = sunet + 20;
  if (sunet > 2000) 
  {
    noTone(difuzor);
    tpsunet = millis() + 2000; 
    sunet = 200;
  }

  }
}

trecere0 = trecere;
pos0 = pos;
}  // end main loop
