// constants won't change. They're used here to set pin numbers:
const int pushButton1Pin = 4; 
const int pushButton2Pin = 5; 
const int signalA = 1;    // red
const int signalB = 2  ;    // red

// variables will change:pus
int pushButton1State = 0;  // variable for reading the pushbutton status
int pushButton2State = 0;  // variable for reading the pushbutton status

//parameteres (microsenconds)
int a = 200;
int b = 900;
int c = 15;// number of pulses
int d = 5500;

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
 if (pushButton1State == HIGH) 
 {
    // watch dog
    digitalWrite(signalB, HIGH);
    delayMicroseconds(50); 
    digitalWrite(signalB, LOW);
    
    // check for waveform mod switch
    if (pushButton2State == LOW ) 
    {
      c = 15; // normal mode
    } else {
      c = 12; // mode 1
    }

    // pulse block
    for (int i = 0; i <= c; i++)
    {
      digitalWrite(signalA, HIGH);
      delayMicroseconds(i*50+a); 
      digitalWrite(signalA, LOW);
      delayMicroseconds(b);
    }
    delayMicroseconds(d);
 } else {
    digitalWrite(signalA, LOW);
 } 
}
