#include <Stepper.h>
#include <math.h>

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

#define PI 3.1415926535897932384626433832795028841971693993751
#define MAX_STEP 100
#define d 1000

Stepper BFS(2048, BFS_Pin1, BFS_Pin2, BFS_Pin3, BFS_Pin4);
Stepper BFS_2(2048, BFS2_Pin1, BFS2_Pin2, BFS2_Pin3, BFS2_Pin4);

int len = 0;
int cnt = 0;

void setup() {
  BFS.setSpeed(20);
  BFS_2.setSpeed(20);

  pinMode(StartBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);

  pinMode(ResetBtn, INPUT_PULLUP);

  pinMode(LightSensor, INPUT);

  Serial.begin(9600);
}

void getData() {
  detachInterrupt(digitalPinToInterrupt(StartBtn));
  detachInterrupt(digitalPinToInterrupt(ResetBtn));

  for (cnt; cnt <= MAX_STEP; ++cnt) {
    moveStepper(len); // 스텝 모터를 움직이는 함수 호출
    double wavelength = calcWavelength(cnt); // 스텝 수에 따른 파장 계산
    double lightIntensity = analogRead(LightSensor) * calcLIC(cnt); // 빛의 세기 보정
    Serial.print(cnt);
    Serial.print("\t");
    Serial.print(wavelength);
    Serial.print("\t");
    Serial.println(lightIntensity);
  }

  resetPos(); // 초기 위치로 스텝 모터를 되돌리는 함수 호출

  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);
}

void resetPos() {
  detachInterrupt(digitalPinToInterrupt(StartBtn));
  detachInterrupt(digitalPinToInterrupt(ResetBtn));

  int stepToMove = -1 * cnt * len;
  moveStepper(stepToMove); // 스텝 모터를 이동하는 함수 호출
  cnt = 0;

  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);
}

void moveStepper(int steps) {
  if (steps > 0) {
    BFS.step(steps);
    BFS_2.step(steps);
  }
}

double calcDegree(int step) {
  return PI * (5 / 18) - atan((1 - 2 * step / MAX_STEP) * tan(PI / 6));
}

double calcWavelength(int step) {
  return d * sin(calcDegree(step));
}

double calcLIC(int step) {
  return 1 + (1 - 2 * step / MAX_STEP) * (1 - 2 * step / MAX_STEP) * (1 / 3);
}

void loop() {
  delay(100);
}
