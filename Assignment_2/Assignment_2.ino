#include <B31DGMonitor.h>

/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

B31DGCyclicExecutiveMonitor monitor;

void setup()
{
  monitor.starMonitoring();
}

void PerformInstanceOfTask1()
{
  monitor.jobStarted(1);

  // Do something here

  monitor.jobEnabled(1);
}

void loop(void)
{
  // Cyclic cycle
}

/*
 * Task 1:
 * high
 * 200 micros pause
 * low
 * 50 micor seconds
 * high 
 * 30 micro secnonds
 * 
 * period 4ms/ 250hz
 */
