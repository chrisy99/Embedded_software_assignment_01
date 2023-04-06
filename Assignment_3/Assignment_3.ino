//#include "frequency.h"

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
const int buttonPin = 6;
const int led2Pin = 7;

// Task 4 value num filter
const int filterSize = 4;
// boundary constant values
const int SigA_Tmax = 3100; // max period for task 2
const int SigB_Tmax = 2100; // max period for task 3
const int SigA_fmin = 333; // min frequency bound for task 2 
const int SigB_fmin = 500; 
const int SigA_fmax = 1000; 
const int SigB_fmax = 1000; 

// Task periods
const int P_T1 = 4;
const int P_T2 = 20;
const int P_T3 = 8;
const int P_T4 = 20;
const int P_T5 = 100;
const int P_T6 = 10;

const float halfThresh = 2047.5; // half of max from potentiometer (4095.00)

// semaphore to store and accesss frequencies from task 2,3 and 5
SemaphoreHandle_t sem_sigAB;

// Event queue for task 6,1) debounce and 2) led
QueueHandle_t t6_queue;

// Task 6 fields
int ledState = HIGH;        // the current state of the output pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 0.2;    // the debounce time; increase if the output flickers

float readings[4]; // store for potentiometer readings
int read_count = 0;

struct frequency {
	float t2;
	float t3;
} frequency;

// Code from arduino examples -> digital -> Debounce
void Task6_1( void * pvParameters ){
  (void) pvParameters;
  for (;;)
  {
    int reading = digitalRead(buttonPin);

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    Serial.println(millis() - lastDebounceTime);
    if ((millis() - lastDebounceTime) > debounceDelay) {
      
      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading; 
        xQueueSend(t6_queue, &buttonState, 10);
      }
    }
    lastButtonState = reading;
    vTaskDelay(P_T6 / portTICK_PERIOD_MS);
  }
}
void Task6_2( void * pvParameters ){
  (void) pvParameters;
  for (;;)
  {
    if (xQueueReceive(t6_queue, &buttonState, 10) == pdPASS){
      if (buttonState == HIGH){
        ledState = !ledState;
        digitalWrite(led2Pin, ledState);
      }
        // set the LED:

      
    }
    vTaskDelay(P_T6 / portTICK_PERIOD_MS);
  }
}

void Task1( void * pvParameters ){
  (void) pvParameters;
  for(;;)
  {
    digitalWrite(outSig, HIGH);
    delayMicroseconds(200);
    digitalWrite(outSig, LOW);

    delayMicroseconds(50);
    digitalWrite(outSig, HIGH);
    delayMicroseconds(30);
    digitalWrite(outSig, LOW);
    vTaskDelay(P_T1 / portTICK_PERIOD_MS);
  }
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

void Task2( void * pvParameters ){
  (void) pvParameters;
  float f_sigA;
  for(;;)
  {
    if(xSemaphoreTake(sem_sigAB,portMAX_DELAY) == pdTRUE){
      f_sigA = pulseIn2(sqrSigA, 3100);
      if (f_sigA == 0){
        f_sigA = SigB_Tmax; // zero value from timeout set to worst case period to avoid inf freqeuncy 
      }
      frequency.t2 = 1000000 / f_sigA;
      xSemaphoreGive(sem_sigAB);
    }
    vTaskDelay(P_T2 / portTICK_PERIOD_MS);
  } 
}

void Task3( void * pvParameters ){
  (void) pvParameters;
  float f_sigB;
  for(;;)
  {
    if(xSemaphoreTake(sem_sigAB,portMAX_DELAY) == pdTRUE){
      f_sigB = pulseIn2(sqrSigB, 2100);
      if (f_sigB == 0){
        f_sigB = SigB_Tmax; // zero value from timeout set to worst case period to avoid inf freqeuncy
      }
      frequency.t3 = 1000000 / f_sigB;
      xSemaphoreGive(sem_sigAB);
    }
    vTaskDelay(P_T3 / portTICK_PERIOD_MS);
  }
}

void Task4( void * pvParameters ){
  (void) pvParameters;
  for(;;)
  {
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
    read_count++;
    vTaskDelay(P_T4 / portTICK_PERIOD_MS);
  }
}

void Task5( void * pvParameters ){
  (void) pvParameters;
  int scaleA, scaleB ;
  for(;;)
  {
    if(xSemaphoreTake(sem_sigAB,portMAX_DELAY) == pdTRUE){
      // frequencies of task 2 and 3 scaled between 0 and 99
      scaleA = map(frequency.t2, SigA_fmax, SigA_fmin,0 ,99);
      scaleB = map(frequency.t3, SigB_fmax, SigB_fmin,0 ,99);
                                                                                                                                  
      Serial.print(scaleA);
      Serial.print(",");
      Serial.println(scaleB);
      xSemaphoreGive(sem_sigAB);
    }
    vTaskDelay(P_T5 / portTICK_PERIOD_MS);
  }
}

void setup(){
  Serial.begin(9600);

  pinMode(outSig, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(sqrSigA, INPUT);
  pinMode(sqrSigB, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(led2Pin, OUTPUT);

  sem_sigAB = xSemaphoreCreateMutex();  

  t6_queue = xQueueCreate(1, sizeof(float));
  // set initial LED state
  digitalWrite(led2Pin, ledState);

   xTaskCreate(
        Task1,
        "task 1",
        512,
        NULL,
        4,
        NULL);

    xTaskCreate(
        Task2,
        "task 2",
        512,
        NULL,
        3,
        NULL);

    xTaskCreate(
        Task3,
        "task 3",
        1024,
        NULL,
        3,
        NULL);

    xTaskCreate(
        Task4,
        "task 4",
        1024,
        NULL,
        2,
        NULL);

    xTaskCreate(
        Task5,
        "task 5",
        1024,
        NULL,
        1,
        NULL);

    xTaskCreate(
        Task6_1,
        "task 6.1",
        512,
        NULL,
        1,
        NULL);

    xTaskCreate(
        Task6_2,
        "task 6.2",
        1024,
        NULL,
        1,
        NULL);
}

void loop(void)
{
  /*
  for (;;){
    Task6_1();
    Task6_2();
    Serial.println("");
  }*/
  }
  