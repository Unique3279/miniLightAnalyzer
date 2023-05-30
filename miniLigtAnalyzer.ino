#include<Stepper.h>
#include<Serial.h>
#define StepperPin1 8
#define StepperPin2 9
#define StepperPin3 10
#define StepperPin4 11
#define StartBtn 2
#define LightSensor A0

Stepper BFS(2048, StepperPin1, StepperPin2, StepperPin3, Stepperpin4);

void setup(){
  BFS.setSpeed(20);
  pinMode(StartBtn, INPUT_PULLUP);
  attachInterrupt(0, getData, FALLING);
  pinMode(LightSensor, INPUT);
  Serial.begin(9600);
}

void getData(){
  int cnt = 0;
  detachInterrupt(0);
  for(cnt; cnt<=100; ++cnt){
    BFS.step(/*length*/);
    Serial.print(cnt); Serial.print(" ") Serial.println(analogRead(LightSensor));      
  }
  attachInterrupt(0, getData, FALLING);
}

void loop(){
  //  대충 기능 구현
}