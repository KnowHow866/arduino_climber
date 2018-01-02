// 腳位配置
#define LEFT_PWM 6
#define WHEEL_LEFT 7
#define WHEEL_RIGHT 8
#define RIGHT_PWM 5
#define BULETOOTH_RT 0
#define BULETOOTH_TX 1

// 行走參數
#define GO_POWER 128
#define DELAY 200
// 旋轉時兩輪的速差
#define TURN_HIGH 64
#define TURN_LOW 0

// 繼電器 : 直流馬達參數
#define RELAY_PIN 13
int relayState = 0;

// 伺服馬達
#include <Servo.h>
Servo Arm;
Servo Boom;
#define SERVO_PIN_UP 10   // 小臂
#define SERVO_PIN_DOWN 11 // 大臂
int servo_pos_up = 0;
int servo_pos_down = 180;
int servoState = 0;

// 系統參數
int mode  = -1;
