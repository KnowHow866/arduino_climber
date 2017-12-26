// 腳位配置
#define LEFT_PWM 6
#define WHEEL_LEFT 7
#define WHEEL_RIGHT 8
#define RIGHT_PWM 9
#define BULETOOTH_RT 0
#define BULETOOTH_TX 1

// 行走參數
#define GO_POWER 192
#define DELAY 500
// 旋轉時兩輪的速差
#define TURN_HIGH 64
#define TURN_LOW 0

// 繼電器 : 直流馬達參數
#define RELAY_PIN 13
int relayState = 0;

// 伺服馬達
#include <Servo.h>
Servo Xladder;
#define SERVO_PIN 10
#define SERVO_MAG 100
int servoState = 0;

// 系統參數
int mode  = -1;
