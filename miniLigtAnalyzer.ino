#include <Stepper.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

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

#define PI 3.1415926535897932384626433832795028841971693993751
#define MAX_STEP 100                        // 최대 스텝 수
#define d 1000                              // 회절격자필름 간격. 단위 nm

Stepper BFS(2048, BFS_Pin1, BFS_Pin2, BFS_Pin3, BFS_Pin4);    //  2048 = 2pi radian
Stepper BFS_2(2048, BFS2_Pin1, BFS2_Pin2, BFS2_Pin3, BFS2_Pin4);    //  stepper for automated reset

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
sensors_event_t event;

int len = 2 / (35 * PI) * 2048;             // 한 스텝에서 도는 각도
int cnt = 0;                                // 현재 스텝 횟수(위치) 기록

void setup(){
  BFS.setSpeed(60);
  BFS_2.setSpeed(60);

  pinMode(StartBtn, INPUT_PULLUP);          // 시작버튼 세팅
  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);

  pinMode(ResetBtn, INPUT_PULLUP);          // 리셋버튼 세팅
  
  Serial.begin(9600);                       // 통신시작

  if(!tsl.begin())
  {
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
  
  /* We're ready to go! */
  Serial.println("Ready.");
}

void getData(){                            
  detachInterrupt(digitalPinToInterrupt(StartBtn));                       // 버튼 여러번 눌림 방지 위해 인터럽트 중지
  detachInterrupt(digitalPinToInterrupt(ResetBtn));

  for(cnt; cnt<=MAX_STEP; ++cnt){                // 구간 100분할 후 동작
    BFS.step(len);
    BFS_2.step(len);
    tsl.getEvent(&event);
    Serial.print(cnt); Serial.print(" "); Serial.println(analogRead(event.light) * calcLIC(cnt));      // 횟수 (tab) 데이터 형식으로 시리얼 출력
    // 파이썬 추가 작업 필요
  }

  attachInterrupt(digitalPinToInterrupt(ResetBtn), resetPos, FALLING);    // 리셋 버튼 재활성화
}

void resetPos(){
  detachInterrupt(digitalPinToInterrupt(StartBtn));     // 작동하는 동안 버튼 비활성화
  detachInterrupt(digitalPinToInterrupt(ResetBtn));

  int stepToMove = -1 * cnt * len;    // getData에서 움직인 길이만큼 반대방향으로 이동
  BFS.step(stepToMove);
  BFS_2.step(stepToMove);
  cnt = 0;      // 이동횟수 초기화

  attachInterrupt(digitalPinToInterrupt(StartBtn), getData, FALLING);    // 시작버튼 재활성화
}

/* 스텝 수에 따른 각도 계산 함수 */
double calcDegree(int step) {
  return PI * (5 / 18) - atan((1 - 2 * step / MAX_STEP) * tan(PI / 6));
}

/* 스텝 수에 따른 파장 계산 함수 */
double calcWavelength(int step) {
  return d * sin(calcDegree(step));
}

/* 스텝 수에 따른 빛의 세기 보정 함수 */
double calcLIC(int step) {
  return 1 + (1 - 2 * step / MAX_STEP) * (1 - 2 * step / MAX_STEP) * (1 / 3);
}

void loop(){
  tsl.getEvent(&event);
  delay(100);
}
