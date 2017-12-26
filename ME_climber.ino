/*
 * 2017/11/22
 * 
 * 設定參數檔之後
 * 啟動程式並搭配序列服監控視窗測試
 * 
 */
#include <SoftwareSerial.h>
#ifndef CONFIG
  #include "config.h"
#endif

// 藍牙設定
SoftwareSerial BT (BULETOOTH_RT, BULETOOTH_TX);  //RT TX

void setup() {
  // 傳輸設定 
  Serial.begin (9600);

  // 腳位設定  
  pinMode (WHEEL_LEFT, OUTPUT);
  pinMode (WHEEL_RIGHT, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // 伺服馬達
  Xladder.attach(SERVO_PIN);

  //藍牙設定
  BT.begin(9600);
}

void loop() {
  Serial.println ("waiting command ... ");
  Serial.println ("keyin b for blueteeth mode");
  Serial.println ("keyin t for test mode");
  while (1) {
//    char init = Serial.read();
//  預設直接進入藍牙模式
    char init = 'b';
    // 藍芽模式
    if (init == 'b') {
      mode = 1;
      break;
    }
    // 測試模式
    else if (init == 't') {
      mode = 0;
      break;
    }
    else continue;
  }

  // 藍芽模式
  if (mode == 1) {
    Serial.println ("藍芽模式");
    while (1) {
      if (BT.available()) {
        Serial.print("get cmd: ");
        char var = BT.read();
        // 停機指令
        if (var == 'i') {
          mode = -1;
          break;
        }
        switch (var) {
          // 前進
          case 'c':
            go_straight(1);
            break;
          // 後退
          case 'e':
            go_straight(0);
            break;
          // 右轉
          case 'f':
            turn(1);
            break;
          // 左轉
          case 'd':
            turn(0);
            break;
          // 打開 or 關閉 繼電器
          case 'a':
            switchRelay();
            break;
          // 起降伺服馬達
          case 'b':
            switchLadder();
            break;
          default:
            Serial.print ("不支援的指令：" );
            Serial.println (var);
        }
      }
    }
  }
  // 測試模式
  else if (mode == 0){
    Serial.println ("測試模式: ");
    Serial.println ("前進");
    go_straight(1);
    Serial.println ("後退");
    go_straight(0);
    Serial.println ("左轉");
    turn(1);
    Serial.println ("右轉");
    turn(0);
    mode = -1;
    switchRelay ();
    switchLadder ();
    Serial.println ("結束模式");
    Serial.println ("-------------------");
    Serial.println ();
  }
  else ;
}

/*
 * 行走function，之後檔案切割出去
 */
void go_straight (int is_ahead) {
  Serial.print ("Motor go ... power: " );
  Serial.println (GO_POWER);
  
  analogWrite (LEFT_PWM, GO_POWER);
  analogWrite (RIGHT_PWM, GO_POWER);
  if (is_ahead) {
    digitalWrite (WHEEL_LEFT, HIGH);
    digitalWrite (WHEEL_RIGHT, HIGH);
  }
  else {
    digitalWrite (WHEEL_LEFT, LOW);
    digitalWrite (WHEEL_RIGHT, LOW);
  }
  delay (DELAY);
  analogWrite (LEFT_PWM, 0);
  analogWrite (RIGHT_PWM, 0);
}

void turn (int is_right) {
  Serial.print ("Motor turn :");
  if (is_right) {
    Serial.println (is_right);
    analogWrite (LEFT_PWM, TURN_HIGH);
    analogWrite (RIGHT_PWM, TURN_LOW);
  }
  else {
    Serial.println (is_right);
    analogWrite (LEFT_PWM, TURN_LOW);
    analogWrite (RIGHT_PWM, TURN_HIGH);
  }
  digitalWrite (WHEEL_LEFT, HIGH);
  digitalWrite (WHEEL_RIGHT, HIGH);
  delay (DELAY);
  analogWrite (LEFT_PWM, 0);
  analogWrite (RIGHT_PWM, 0);
}

// 繼電器開關
void switchRelay () {
  if (relayState == 0) relayState = 1;
  else relayState = 0;

  digitalWrite(RELAY_PIN, relayState);
  Serial.print("繼電器：");
  Serial.println(relayState);
}

// 伺服馬達開關
void switchLadder () {
  if (servoState == 0) servoState = 1;
  else servoState = 0;

  Xladder.write(servoState * SERVO_MAG);
  Serial.print("伺服馬達：");
  Serial.println(servoState);
}


