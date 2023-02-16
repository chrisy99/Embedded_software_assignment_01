/*
 * Assignment 1  
 * Christopher Yip
 * 15/2/2023
 * 
 * Generates 2 signals while Button 1 is pressed down:
 * Signal B with a short 50 microseconds pulse before the signal A block,
 * Signal A with a block of (c)15 pulses per cycle.
 * Wave cycles are separated by a delay of d (5500) microseconds.
 * 
 * Mode 1 is triggered while holding down Button 2, reducing the number of pulses in each block by 3.
 * 
 */
 
const int pushButton1Pin = 4; 
const int pushButton2Pin = 5; 
const int signalA = 1;    // red
const int signalB = 2  ;    // red

// variables will change:pus
int pushButton1State = 0;  // variable for reading the pushbutton status  
int pushButton2State = 0;  // variable for reading the pushbutton status

// parameteres (microsenconds)
const int a = 200;
const int b = 900;  // width between pulses
const int c = 15;   // number of pulses
const int d = 5500; // delay between pulse blocks
#define mode(x,y)(x == HIGH? y/2 :y)

void setup() 
{
  pinMode(signalA, OUTPUT);
  pinMode(signalB, OUTPUT);
  pinMode(pushButton1Pin, INPUT);
  pinMode(pushButton2Pin, INPUT);
}

void loop() 
{
  // read the state of the pushbutton value:
  pushButton1State = digitalRead(pushButton1Pin);
  pushButton2State = digitalRead(pushButton2Pin);

 //check if the pushbutton1 is pressed. If it is, enable stream of pulses
 if (pushButton1State == LOW) 
 {
    // watch dog signal
    digitalWrite(signalB, HIGH);
    delayMicroseconds(50);  
    digitalWrite(signalB, LOW);

    // pulse block
    for (int i = 0; i <  c ; i++)
    {
      digitalWrite(signalA, HIGH);
      delayMicroseconds(i*50+a); 
      digitalWrite(signalA, LOW);
      delayMicroseconds(mode(pushButton2State, b));
    }
    delayMicroseconds(mode(pushButton2State, d));
 } else {
    digitalWrite(signalA, LOW);
 } 
}
