#include <Stepper.h>   // 이거 왜이럼; // 오 고침
#define BFS_Pin1 4
#define BFS_Pin2 5
#define BFS_Pin3 6
#define BFS_Pin4 7

#define BFS2_Pin1 8
#define BFS2_Pin2 9
#define BFS2_Pin3 10
#define BFS2_Pin4 11

#define StartBtn 2
#define ResetBtn 3
#define LightSensor A0

Stepper BFS(2048, BFS_Pin1, BFS_Pin2, BFS_Pin3, BFS_Pin4);    //  2048 = 2pi radian
Stepper BFS_2(2048, BFS2_Pin1, BFS2_Pin2, BFS2_Pin3, BFS2_Pin4);    //  servo for automated reset
int len = 0;
int cnt = 0;

void setup(){
  BFS.setSpeed(20);
  BFS_2.setSpeed(20);
  pinMode(StartBtn, INPUT_PULLUP);          // 시작버튼 할당
  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);     // 시작버튼에 인터럽트 부착
  pinMode(ResetBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ResetBtn), resetPos, FALLING);
  pinMode(LightSensor, INPUT);              // 광센서
  Serial.begin(9600);                       // 통신시작
}

void getData(){                            
  detachInterrupt(digitalPinToInterrupt(StartBtn));                       // 버튼 여러번 눌림 방지 위해 인터럽트 중지
  detachInterrupt(digitalPinToInterrupt(ResetBtn));
  for(cnt; cnt<=100; ++cnt){                // 구간 100분할
    BFS.step(len);
    BFS_2.step(len);
    Serial.print(cnt); Serial.print(" "); Serial.println(analogRead(LightSensor));      
  }
  attachInterrupt(digitalPinToInterrupt(ResetBtn), resetPos, FALLING);
}

void resetPos(){
  detachInterrupt(digitalPinToInterrupt(StartBtn));
  detachInterrupt(digitalPinToInterrupt(ResetBtn));
  int stepToMove = -1 * cnt * len;
  BFS.step(stepToMove);
  BFS_2.step(stepToMove);
  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);
}

void loop(){
  //  대충 기능 구현
}