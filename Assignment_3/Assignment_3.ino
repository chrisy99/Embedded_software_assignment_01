
#include <B31DGMonitor.h>
#include <Ticker.h>

/**  
 * B31DG Assignment 3
 * Author : Christopher Yip  
 */

 // Signal pins 
const int outSig = 4; // output signal pin
const int sqrSigA = 1; //7 Square wave signal input for task 2
const int sqrSigB = 9; //4 Square wave signal input for task 3
const int ledPin = 19; // T4
const int analogOut = 3; // Read pin for potentiometer
const int buttonPin = 7;
const int led2Pin = 8;

// Task 4 value num filter
const int filterSize = 4;
// boundary constant values
const int SigA_Tmax = 3100; // max period for task 2
const int SigB_Tmax = 2100; // max period for task 3
const int SigA_fmin = 333; // min frequency bound for task 2 
const int SigB_fmin = 500; 
const int SigA_fmax = 1000; 
const int SigB_fmax = 1000; 
const int tickT = 4;       // LCM of all task periods

const float halfThresh = 2047.5; // half of max from potentiometer (4095.00)

float start_time, end_time;
float worst_time = 0;
double f_sigA = 0;
double f_sigB = 0;
bool task3_delay = false;

int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 0;    // the debounce time; increase if the output flickers

int read_count = 0;
int tickCount = 0;

float readings[4]; // store for potentiometer readings
Ticker ticker;

B31DGCyclicExecutiveMonitor monitor;

void setup(){
  pinMode(outSig, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(sqrSigA, INPUT);
  pinMode(sqrSigB, INPUT);
  Serial.begin(9600);
  
  //monitor.startMonitoring();
  //ticker.attach_ms(tickT, tick); 
  //tick();
  pinMode(buttonPin, INPUT);
  pinMode(led2Pin, OUTPUT);

  // set initial LED state
  digitalWrite(led2Pin, ledState);
}

void tick(){
  Task1();
  
  if (tickCount % 10 == 4) Task2();
  if (tickCount % 10 == 9) Task2();
  if (tickCount % 10 == 0) Task3();
  if (tickCount % 10 == 2) Task3();
  if (tickCount % 10 == 5) Task3();
  if (tickCount % 10 == 6) Task3();
  if (tickCount % 10 == 8) Task3();
  if (tickCount % 5  == 0) Task4();
  if (tickCount % 25 == 0) Task5(); 
  tickCount++;
  if (tickCount == 50) tickCount = 0; // cycle reset
}
  
void loop(void)
{
  for (;;){
    Debounce();
  }
  }

// Code from arduino examples -> digital -> Debounce
void Debounce(){
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(led2Pin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

// additional task for assignment 3
void Task0(){
  // TODO
    // refactor from Debounce()
}
void Task1(){
  monitor.jobStarted(1);
  digitalWrite(outSig, HIGH);
  delayMicroseconds(200);
  digitalWrite(outSig, LOW);

  delayMicroseconds(50);
  digitalWrite(outSig, HIGH);
  delayMicroseconds(30);
  digitalWrite(outSig, LOW);
  monitor.jobEnded(1);
}

/**
 * PulseIn adaptation to reduce read time and include time out as parameter
 * Returns full period as 2*pulse in microseconds
 */
double pulseIn2(int pin, long timeout){ 
  double period;  
  if (digitalRead(pin) == HIGH)
  {
    period = pulseIn(pin, LOW, timeout);
  }
  else
  {
    period = pulseIn(pin, HIGH, timeout);
  }
  return 2 * (double)period;
}

void Task2(){
  monitor.jobStarted(2);
  f_sigA = pulseIn2(sqrSigA, 3100);
  if (f_sigA == 0){
    f_sigA = SigB_Tmax; // zero value from timeout set to worst case period to avoid inf freqeuncy 
  }
  f_sigA = 1000000 / f_sigA;
  monitor.jobEnded(2);
}

void Task3(){
  monitor.jobStarted(3);
  f_sigB = pulseIn2(sqrSigB, 2100);
  if (f_sigB == 0){
    f_sigB = SigB_Tmax; // zero value from timeout set to worst case period to avoid inf freqeuncy
  }
  f_sigB = 1000000 / f_sigB;
  monitor.jobEnded(3);  
}

void Task4(){
  monitor.jobStarted(4);
  
  float sum;
  float num;
  long filterVal;
  sum = 0;                                                    
  readings[read_count % filterSize ] = analogRead(analogOut);

  // adjusts for when less then 4 readings(filter size) have been recorded
  if (read_count < filterSize) {
    num = read_count;
  } else {
    num = filterSize;
  }

  for (int i = 0; i < num ; i++) {
    sum += readings[i];
  }

  filterVal = sum / filterSize;
  if (filterVal > halfThresh){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  monitor.jobEnded(4);
  read_count++;
}

/**
 * Returned scaled f between min and max bound 
 */
int Scale(double f, double maxf, double minf){
  double fScaled = 0;

  if (f <  minf)       fScaled = 0;  // Lower scale bound
  else if (f >  maxf)  fScaled = 99; // Upper scale bound
  else                 fScaled = (f - minf) * 99 / (maxf - minf); // scale equation

  return (int)fScaled;
}

void Task5(){
  int scaleA, scaleB ;
  // frequencies of task 2 and 3 scaled between 0 and 99
  monitor.jobStarted(5);
  scaleA = Scale(f_sigA, SigA_fmax, SigA_fmin);
  scaleB = Scale(f_sigB, SigB_fmax, SigB_fmin);
                                                                                                                               
  Serial.print(scaleA);
  Serial.print(",");
  Serial.println(scaleB);
  monitor.jobEnded(5);
}
