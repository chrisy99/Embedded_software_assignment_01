#include <B31DGMonitor.h>
#include <Ticker.h>

/* 
B31DG Assignment 2
  
*/  
const int outSig = 1; // output signal pin
const int sqrSigA = 2; // Square wave signal input for task 2
const int sqrSigB = 3; // Square wave signal input for task 3

float f_sigA, f_sigB;

int hyperCount = 0;
Ticker ticker;
#define SigA_fmin 333
#define SigB_fmin 500
#define SigA_fmax 333
#define SigB_fmax 500
#define Hyper_Period 4


B31DGCyclicExecutiveMonitor monitor;

void setup()
{
  monitor.startMonitoring();

  pinMode(outSig, OUTPUT);
  pinMode(sqrSigA, INPUT);
  pinMode(sqrSigB, INPUT);

  float f_sigA, f_sigB;

  Serial.begin(9600);

  ticker.attach_ms(Hyper_Period, tick); // TODO : length of hyper period, define later
}

void PerformInstanceOfTask1()
{
  monitor.jobStarted(1);

  // Do something here

  monitor.jobEnded(1);
}


void tick()
{
  hyperCount ++;

  /**
   * TODO : put schedule
   */
}

void loop(void)
{}

void Task1()
{
    digitalWrite(outSig, HIGH);
    delayMicroseconds(200);  
    digitalWrite(outSig, LOW);
    delayMicroseconds(50);
    digitalWrite(outSig, HIGH);
    delayMicroseconds(30);
}

void Task2()
{
  f_sigA = 1000000.0 / pulseIn(sqrSigA, HIGH);
}

void Task3()
{
  f_sigB = 1000000.0 / pulseIn(sqrSigB, HIGH);
}

void Task4()
{
  Serial.println("unimplemented, needs confirmation");
  /* Interpretation:
   *  1) Read one input and push to queue of readings
   *  2) pop() if size == 5
   *  2) Compute value from sum/size ( it is assumed size is always <= 4)
   *  3) if (value > (max_v/2)) light led
   */
}

float Scale(float f, float maxf, float minf)
{
  float fScaled = 0;
  
  if (f <  minf)       fScaled = 0;
  else if (f >  maxf)  fScaled = 99;
  else                 fScaled = (f - minf) * 99 / (maxf - minf);

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
