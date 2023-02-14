// constants won't change. They're used here to set pin numbers:
const int pushButton1Pin = 4; 
const int pushButton2Pin = 5; 
const int signalA = 1;    // red
const int signalB = 2  ;    // red

// variables will change:pus
int pushButton1State = 0;  // variable for reading the pushbutton status
int pushButton2State = 0;  // variable for reading the pushbutton status

void setup() 
{
  pinMode(signalA, OUTPUT);
  pinMode(signalB, OUTPUT);
  pinMode(pushButton1Pin, INPUT);
  pinMode(pushButton2Pin, INPUT);

  //parameteres (microsenconds)
  a = 200;
  b = 900;
  c = 15;// number of pulses
  d = 5500;

}

void loop() 
{
  // read the state of the pushbutton value:
  pushButton1State = digitalRead(pushButton1Pin);
  pushButton2State = digitalRead(pushButton2Pin);


 //check if the pushbutton1 is pressed. If it is, enable stream of pulses
 if (pushButton1State == HIGH) 
 {
    // check for waveform mod switch
    if (pushButton2State == HIGH) 
    {
      c = 15; // normal mode
    } else {
      c = 12; // mode 1
    }

    // pulse block
    for (i =0; i <= c; i++)
    {
      digitalWrite(signalA, HIGH);
      delayMicroseconds(i*50+a); 
      digitalWrite(signalA, LOW);
      delayMicroseconds(b);
    }
    delayMicroseconds(d);
 }
 } else {
    digitalWrite(signalA, LOW);
 }


 
  
}
