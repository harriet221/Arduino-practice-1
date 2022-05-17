#include <Wire.h> // I2C 통신
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 16,2); // LCD 16x2 문자열 출력

int trigPin = 7; // 발신부 핀
int echoPin = 6; // 수신부 핀
int led = 8; // LED 핀
int button = 13; // 버튼(switch) 핀

int cnt; // count : 횟수 세는 용
long pre_distance; // 이전 거리 : 30cm 이상 멀어졌는지 확인 용
long cur_distance; // 현재 거리 : 10cm 이하로 가까워졌는지 확인 용

boolean buttonState = false; // 버튼 상태 초기화 = 풀다운 저항 (0V LOW / 5V HIGH)
boolean buttonSwitch_state_old = LOW; // 버튼 이전 상태
boolean buttonSwitch_state_new = LOW; // 버튼 현재 상태

void setup() {
  Serial.begin(9600); // 데스크톱과의 시리얼 통신
  pinMode(echoPin, INPUT); // 수신부 입력
  pinMode(trigPin, OUTPUT); // 발신부 출력
  pinMode(led, OUTPUT); // LED 출력
  pinMode(button, INPUT); // 버튼 입력
  
  lcd.init(); // lcd 초기화
  lcd.backlight(); // 백라이트
  lcd.setCursor(0,0); // 커서 왼쪽 정렬
  lcd.print("PUSHUP COUNTER"); // 초기 출력
  delay(2000); // 해당 출력 3초 보이기
  lcd.clear(); // 지우기
  pre_distance = 300;
  cnt = 0; // 횟수 초기화
}

void loop() {
  long duration; // 거리 변수
  digitalWrite(trigPin, HIGH); // 발신부에서 초음파 발생
  delayMicroseconds(10); // 마이크로초 이후
  digitalWrite(trigPin, LOW); // 발생 중지

  duration = pulseIn(echoPin, HIGH); // 수신부에서 수신되는 펄스의 HIGH 구간 길이 측정
  cur_distance = ((float)(340 * duration) / 1000) / 2; // 현재 거리로 계산

  if(cur_distance <= 100){ // 100mm = 10cm
    if(pre_distance >= 300){ // 300mm = 30cm
      pre_distance = cur_distance; // 이전 거리 갱신
      cnt++; // 횟수 증가
      lcd.clear(); // LCD 화면 초기화
      lcd.print(cnt); // 횟수 출력
      digitalWrite(led, HIGH); // 횟수 바뀌었으므로 LED 켜졌다가
      delay(300); // 0.3초 이후 ((반짝))
      digitalWrite(led, LOW); // 다시 꺼짐
    }
  }
  else{ 
    pre_distance = cur_distance; // 이전 거리 갱신
    delay(300); // 0.3초 딜레이
  }
  
  if( digitalRead(button) ){ // 버튼 입력 있으면 실행
    buttonSwitch_state_new = digitalRead(button); // 버튼 상태 읽기
    delay(1);  // 채터링 방지
    if(buttonSwitch_state_new != buttonSwitch_state_old) { // 버튼 상태 달라졌으면 실행
      if(buttonSwitch_state_new == HIGH) { // 버튼 현재 상태가 high (누름)이면 실행
        cnt = 0; // 횟수 0으로 초기화
        lcd.clear(); // LCD 화면 초기화
        lcd.print(cnt); // 횟수 출력 (0)
        digitalWrite(led, HIGH); //  횟수 바뀌었으므로 LED 켜졌다가
        delay(500); // 0.5초 이후
        digitalWrite(led, LOW); // 다시 꺼짐
      }
    }
  }
}
