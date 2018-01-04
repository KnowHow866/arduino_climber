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
  Serial.println(WHEEL_RIGHT);

  // 伺服馬達
  Boom.attach(SERVO_PIN_DOWN);
  Boom.write(servo_pos_down);
  Arm.attach(SERVO_PIN_UP);
  Arm.write(servo_pos_up);

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
        if (var == 'p') {
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
          case 'd':
            turn(1);
            break;
          // 左轉
          case 'f':
            turn(0);
            break;
          // 打開 or 關閉 繼電器
          case 'o':
            switchRelay();
            break;
          // 起降伺服馬達
          case 'g':
            // 起大手
            ctrlBoom (-15);
            break;
          case 'i':
            ctrlBoom (15);
            break;
          case 'h':
            // 起小手
            ctrlArm (5);
            break;
          case 'j':
            ctrlArm (-5);
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
    for (int i = 0; i < 8; i++) {
      ctrlBoom (-20);
      delay(500);
    }
    for (int i = 0; i < 8; i++) {
      ctrlArm (5);
      delay(500);
    }
    for (int i = 0; i < 8; i++) {
      ctrlBoom (20);
      delay(500);
    }
    for (int i = 0; i < 8; i++) {
      ctrlArm (-5);
      delay(500);
    }
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
  Serial.println (RIGHT_PWM);

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
  if (relayState == 0) {
    relayState = 1;
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    relayState = 0;
    digitalWrite(RELAY_PIN, LOW);
  }

  Serial.print("繼電器：");
  Serial.println(relayState);
}

// 伺服馬達角度加減
void ctrlBoom (int ctrl) {
  if (servo_pos_down + ctrl <= 180) {
    servo_pos_down += ctrl;
    Boom.write(servo_pos_down);
    Serial.print("伺服馬達 大臂：");
    Serial.println(servo_pos_down);
  }
  else {
    Serial.println("到達底線，不能夠再寫入馬達");
  }
}
void ctrlArm (int ctrl) {
  if (servo_pos_up + ctrl >= 0 && servo_pos_up + ctrl <= 70) {
    servo_pos_up += ctrl;
    Arm.write(servo_pos_up);
    Serial.print("伺服馬達 小臂：");
    Serial.println(servo_pos_up);
  }
  else {
    Serial.println("超過限制，不能夠再寫入馬達");
  }
}



