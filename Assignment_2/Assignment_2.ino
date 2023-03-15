#include <B31DGMonitor.h>
#include <Ticker.h>
/*
  B31DG Assignment 2

*/
const int outSig = 1; // output signal pin
const int sqrSigA = 5; //7 Square wave signal input for task 2
const int sqrSigB = 9; //4 Square wave signal input for task 3
const int ledPin = 19; //19
const int analogOut = 3; //4
const int filterSize = 4;
const float halfThresh = 2047.5; // half of max from potentiometer (4095.00)

float start_time, end_time;
float worst_time = 0;
double f_sigA = 0;
double f_sigB = 0;
bool task3_delay = false;

int t3_count = 0;
int read_count = 0;
int tickCount = 0;

float readings[4] ;
bool task2_frame = false;

Ticker ticker;
#define SigA_fmin 333
#define SigB_fmin 500
#define SigA_fmax 1000
#define SigB_fmax 1000
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
  
  monitor.startMonitoring();
  ticker.attach_ms(4, tick); // TODO : length of hyper period, define later
  tick();
  
}

void tick()
{
  
  
  Task1();
  task2_frame = ((tickCount+1) % 5 == 0);
  
  if (tickCount % 2 == 0)
  {
    if (!task2_frame)
    {
      Task3();
    }
    else{
      task3_delay = true;  
    }
    
  }else if (task3_delay)
  {
    Task3();
    task3_delay = false;
  }
  
  if (task2_frame)
  {
      Task2();
      Task4();
  }
  
  if (tickCount % 25 == 0)  Task5(); 

  tickCount++;
  if (tickCount == 50) tickCount = 0;
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
double pulseIn2(int pin, long timeout)
{ 
  
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

void Task2()
{
  monitor.jobStarted(2);
  
  f_sigA = pulseIn2(sqrSigA, 3100);
  f_sigA = 1000000 / f_sigA;

  monitor.jobEnded(2);

}

void Task3()
{
  monitor.jobStarted(3);
  
  f_sigB = pulseIn2(sqrSigB, 2100);
  f_sigB = 1000000 / f_sigB;

  monitor.jobEnded(3);  
  // Serial.println(tickCount);
}

void Task4()
{
  monitor.jobStarted(4);
  
  float sum = 0;
  float num;
  long filterVal;

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
  if (filterVal > halfThresh)
  {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  monitor.jobEnded(4);
  read_count++;
}

double Scale(double f, double maxf, double minf)
{
  double fScaled = 0;

  if (f <  minf)       fScaled = 0;  // Lxower scale bound
  else if (f >  maxf)  fScaled = 99; // Upper scale bound
  else                 fScaled = (f - minf) * 99 / (maxf - minf); // scale equation

  return fScaled;
}
void Task5()
{
  float start, endtime;

  float scaleA, scaleB ;
  // frequencies of task 2 and 3 scaled between 0 and 99
  monitor.jobStarted(5);
  scaleA = Scale(f_sigA, SigA_fmax, SigA_fmin);
  scaleB = Scale(f_sigB, SigB_fmax, SigB_fmin);

//  start = micros();
                                                                                                                               
  Serial.print(scaleA);
  Serial.print(",");
  Serial.println(scaleB);
  monitor.jobEnded(5);
//  endtime = micros() - start;
//  Serial.print(endtime);
//  Serial.println(" <<<");
}
