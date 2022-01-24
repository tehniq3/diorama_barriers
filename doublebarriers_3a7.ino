/* Sweep by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain, modified 8 Nov 2013 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep

 adapted by Nicu FLORICA (niq_ro)
 ver.2.0 - adaugat al doilea servo
 ver.3.0 - schimbat libraria cu Servotimer2 - https://github.com/nabontra/ServoTimer2
 ver.3.a - eliminat buclele FOR
 ver.3.a.1 - revenit la libraria Servo clasica
 ver.3.a.7 - deconectare virtuala servo pentru a scoate sunete + lumina alba intermitenta
*/

#define ridicare 2
#define coborare 3
#define servo1 9
#define servo2 10
#define difuzor 12
#define ledrosu1 4
#define ledrosu2 5
#define ledalb 6
#include <Servo.h>
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
// twelve servo objects can be created on most boards


int pos = 0;    // variable to store the servo position
int posmin = 9; // minimum position of the servo
int posmax = 60; // maximum position of the servo

int minipauza = 15;  // pentru reducere viteza, implicit e 15.. viteza scade daca numarul creste
int pauzamica = 250; // pauza mica pentru apasare buton
byte directie = 0;  // 0 - stop, 1 - ridicare, 2 - coborare
long tpcomanda;
int timp1, timp2;
int tpmillis = 0;
int pitch = 400;
long tppauzasunet = 10;
long tpsunet = 0;
int sunet = 200;

void setup() {
  Serial.begin(9600);
  
  pinMode(ridicare, INPUT);
  pinMode(coborare, INPUT);
  digitalWrite(ridicare, HIGH);  // rezistenta pull-up interna
  digitalWrite(coborare, HIGH);  // rezistenta pull-up interna

  pinMode(ledrosu1, OUTPUT);
  pinMode(ledrosu2, OUTPUT);
  pinMode(ledalb, OUTPUT);
  digitalWrite(ledrosu1, LOW);
  digitalWrite(ledrosu2, LOW);
  digitalWrite(ledalb, HIGH);

 pinMode(difuzor, OUTPUT); 
// digitalWrite(difuzor, LOW);

    pos = posmin;
    myservo1.attach(servo1);  // attaches the servo on pin 9 to the servo object
    myservo2.attach(servo2);
    myservo1.write(posmin);              // tell servo to go to position in variable 'pos'
    delay(minipauza/2);  
    myservo2.write(posmax); 
    delay(minipauza/2); 
    myservo1.detach();  // detaches the servo from pin 9
    myservo2.detach(); 
      
}

void loop() {

  if ((digitalRead(ridicare) == LOW) and (pos < posmax))
  {
  directie = 1;
  pos = posmin;
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
  
  if ((digitalRead(coborare) == LOW) and (pos > posmin))
  {
  directie = 2;
  pos = posmax;
  delay(pauzamica);
  tpcomanda = millis();
  noTone(difuzor);
  myservo1.attach(servo1);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(servo2);
  }
  

  if ((directie == 1) and (millis() - tpcomanda > minipauza))
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

  if ((directie == 2) and (millis() - tpcomanda > minipauza))
  {
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(minipauza/2);  
    myservo2.write(90-pos); 
    delay(minipauza/2);                       // waits 15ms for the servo to reach the position
    pos-=1;
    if (pos <= posmin) 
    {
      directie = 0;
      myservo1.detach();  // detaches the servo from pin 9
      myservo2.detach();
    }
    tpcomanda = millis();
  }


if (pos > posmin) // bariera ridicata
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
else
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

// tpmillis2 = millis()/1000;
if ((millis() - tppauzasunet > tpsunet) and (pos > posmin))
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
    Serial.print(tpsunet);
    Serial.print("/");
    Serial.print(millis());
     Serial.print("---> ");
     Serial.print(millis() - tppauzasunet);
     Serial.print(" > ? ");
     Serial.println(tpsunet);
    sunet = 200;
  }

  }
}


}  // end main loop
