#include <B31DGMonitor.h>
#include <Ticker.h>
#include <ArduinoQueue.h>
/* 
B31DG Assignment 2
  
*/  
const int outSig = 1; // output signal pin
const int sqrSigA = 7; // Square wave signal input for task 2
const int sqrSigB = 4; // Square wave signal input for task 3
const int ledPin = 19;
const int analogOut = 3;
const int filterSize = 4;
const float halfThresh = 2047.5; // half of max from potentiometer (4095.00)

float start_time, end_time;
float worst_time = 0;
float f_sigA, f_sigB;

int fault_count = 0;
int read_count = 0;
int hyperCount = 0;

float readings[4] ;

ArduinoQueue<float> intQueue(20);
Ticker ticker;
#define SigA_fmin 333
#define SigB_fmin 500
#define SigA_fmax 333
#define SigB_fmax 500
#define Hyper_Period 4


B31DGCyclicExecutiveMonitor monitor;

void setup()
{
  pinMode(outSig, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(sqrSigA, INPUT);
  pinMode(sqrSigB, INPUT);

  float f_sigA, f_sigB;

  Serial.begin(9600);

  ticker.attach_ms(4, tick); // TODO : length of hyper period, define later
  tick();
  monitor.startMonitoring();
}

void tick()
{
  Task4();  
  
//
//  hyperCount++;
//  Task1();
//
//  if (hyperCount%20 == 0)
//  {
//    Task2();
//  }
}

void loop(void)
{}

void Task1()
{
    monitor.jobStarted(1);
    digitalWrite(outSig, HIGH);
    delayMicroseconds(200);  
    digitalWrite(outSig, LOW);     

                         
    delayMicroseconds(50);
    digitalWrite(outSig, HIGH);
    delayMicroseconds(30);
    monitor.jobEnded(1);

    digitalWrite(outSig, LOW);
}
int pulseIn2(int pin)
{
  long period;
  
  if (digitalRead(pin) == HIGH)
  {
    period = pulseIn(pin, LOW);  
  }
  else
  {
    period = pulseIn(pin, HIGH); 
  } 
  return 2*period;
}

void Task2()
{    
  f_sigA = pulseIn2(sqrSigA);
  
  Serial.print(f_sigA); 
  Serial.print(" == ");
  f_sigA = 1000000 / f_sigA;
  Serial.println(f_sigA); 
  
}

void Task3()
{
  f_sigB = pulseIn2(sqrSigB);
  f_sigB = 1000000 / sqrSigB;
}

void Task4()
{
  float sum = 0;
  float num;
  long filterVal;
  float start, endtime;
  
  start = micros();
  readings[read_count % filterSize ] = analogRead(analogOut);

  // adjusts for when less then 4 readings(filter size) have been recorded
  if (read_count < filterSize){
    num = read_count;
  }else{
    num = filterSize;
  }
  
  for (int i = 0; i < num ; i++){
    sum += readings[i];
  }

  filterVal = sum / filterSize;
  if (filterVal > halfThresh)
  {
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }
  
  endtime = micros() - start;
  Serial.print(endtime);
  Serial.print(" === ");
  Serial.println(filterVal);

  read_count++;
  /* Interpretation:
   *  1) Read one 
   *  input and push to queue of readings
   *  2) pop() if size == 5
   *  2) Compute value from sum/size ( it is assumed size is always <= 4)
   *  3) if (value > (max_v/2)) light led
   */
}

float Scale(float f, float maxf, float minf)
{
  float fScaled = 0;
  
  if (f <  minf)       fScaled = 0;  // Lxower scale bound
  else if (f >  maxf)  fScaled = 99; // Upper scale bound
  else                 fScaled = (f - minf) * 99 / (maxf - minf); // scale equation

  return fScaled;
}
void Task5()
{
  // frequencies of task 2 and 3 scaled between 0 and 99
  float scaleA = Scale(f_sigA, SigA_fmax, SigA_fmin);
  float scaleB = Scale(f_sigB, SigB_fmax, SigB_fmin);
  
  // serial port log of scaled frequencies 
  Serial.print("Task 2 frequency, Task 3 frequency : (" );
  Serial.print(scaleA);
  Serial.print(",");
  Serial.print(scaleB);
  Serial.print(")");
}
