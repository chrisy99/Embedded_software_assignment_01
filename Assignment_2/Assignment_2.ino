#include <B31DGMonitor.h>/* 
B31DG Assignment 2
  
*/  
const int outSig = 1; // output signal pin
const int sqrSigA = 2; // Square wave signal input for task 2
const int sqrSigB = 3; // Square wave signal input for task 3

float f_sigA, f_sigB;
#define SigA_fmin = 333;
#define SigB_fmin = 500;
#define SigA_fmax = 333;
#define SigB_fmax = 500;


B31DGCyclicExecutiveMonitor monitor;

void setup()
{
  monitor.startMonitoring();

  pinMode(outSig, OUTPUT);
  pinMode(sqrSigA, INPUT);
  pinMode(sqrSigB, INPUT);xs

  float f_sigA, f_sigB;

  Serial.begin(9600);
}

void PerformInstanceOfTask1()
{
  monitor.jobStarted(1);

  // Do something here

  monitor.jobEnded(1);
}

void loop(void)
{
  // Cyclic executive
}
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

void Task5()
{
  float scaleA, scaleB;

  // frequencies of task 2 and 3 scaled between 0 and 99
  if (f_sigA >=  333) scaleA = 0;
  else if (f_sigA <=  1000) scaleA = 99;
  else scaleA = (f_sigA - 333) * 99 / 667;
  if (f_sigB >=  500) scaleA = 0;
  if (f_sigB <=  1000) scaleA = 99;
  else scaleB = (f_sigB - 500) * 99 / 500;
  
  // serial port log of scaled frequencies 
  Serial.print("Task 2 frequency, Task 3 frequency : (" );
  Serial.print(scaleA);
  Serial.print(",");
  Serial.print(scaleB);
  Serial.print(")");
}
