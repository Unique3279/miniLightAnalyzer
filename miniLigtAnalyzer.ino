#include <Stepper.h>                       // 이거 왜이럼;
#define StepperPin1 8
#define StepperPin2 9
#define StepperPin3 10
#define StepperPin4 11
#define StartBtn 2
#define LightSensor A0

Stepper BFS(2048, StepperPin1, StepperPin2, StepperPin3, StepperPin4);

void getData(){
  int cnt = 0;                              // 현재 스텝 횟수(위치) 기록
  detachInterrupt(0);                       // 버튼 여러번 눌림 방지 위해 인터럽트 중지
  for(cnt; cnt<=100; ++cnt){                // 구간 100분할
    BFS.step(/*length*/);
    Serial.print(cnt); Serial.print(" "); Serial.println(analogRead(LightSensor));      
  }
  attachInterrupt(0, getData, FALLING);     // 인터럽트 재활성화
}

void setup(){
  BFS.setSpeed(20);
  pinMode(StartBtn, INPUT_PULLUP);          // 시작버튼 할당
  attachInterrupt(0, getData, FALLING);     // 시작버튼에 인터럽트 부착
  pinMode(LightSensor, INPUT);              // 광센서
  Serial.begin(9600);                       // 통신시작
}

void loop(){
  //  대충 기능 구현
}