/* Данный скетч вбирает в себе функции вольтметра, тахометра и регулятора УОЗ (в разработке), также разрабатывается возможность по установке электронной отсечки по оборотам.
 * Управляется с помощью энкодера, концевик для отсечки (в разработке)
 * Средства отображения: OLED дисплей 128*64 двухцветный, диагностический светодиод, индикация, сервопривод стрелки тахометра.
 */
//#define CLK 8
//#define DIO 7
//#define servo_pin 9
#define ign_rd 0
#define SCK A5
#define SDA A4
#define S1 10
#define S2 8
#define KEY 9
#define Yhead 1
#define Yvalue 30
#define Xhead 40
#define OLED_SOFT_BUFFER_64 

#include "GyverEncoder.h"
//#include "GyverTM1637.h"
//#include "Servo.h"
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
//GyverTM1637 disp(CLK, DIO);
//Servo servo_one;
Encoder enc1(S1, S2, KEY);

enum {RPM = 1, voltmeter, ign_timing, selection_mode} mode; 
byte for_servo,Xvalue,value;
int RPM_clk;
unsigned long upd,last_ign,clk_gen;
boolean flag,chng;
//byte ;
 

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE2);
  //pinMode(2, INPUT_PULLUP);
  //servo_one.attach(servo_pin);
  attachInterrupt(ign_rd,ign, FALLING);
  mode = 1;
  Xvalue = 50;
  //disp.brightness(7);                     // яркость, 0 - 7 (минимум - максимум)
  //digitalWrite(ign_gen_pin, RISING);
  //disp.displayInt(RPM);
  oled.init();                              // инициализация
  oled.setScale(4);
  oled.clear();
  oled.update();
}

void loop() {
  enc1.tick();
  if (enc1.isDouble()) {
    flag = 1;
    mode = 4;
    oled.clear();
    oled.setScale(2);
    oled.setCursorXY(0, Yhead);
    oled.print(">");
    oled.setCursorXY(128-15, Yhead);
    oled.print("<");
    oled.setCursorXY(Xhead,Yhead);
    oled.setCursorXY(20,Yhead);
    oled.print("select");
    oled.setScale(4);
    oled.setCursorXY(Xvalue,Yvalue);
    oled.print(value);
    //oled.clear();
    oled.update(); 
  }
  if (mode != 4) {  
    if (millis()-clk_gen >= 500) {
      if (mode == 1) {
        value = RPM_func();
        //scrn_upd(value);
      }
      if (mode == 2) {
        value = voltmeter_func();
      }
      if (mode == 3) {
        //ign_timing_func()
      }
      clk_gen=millis();   
    }
  }
 while (flag == 1) {
   enc1.tick();
   if (enc1.isRight()) {
    switch (mode) {
      case 1: mode = 2;
              break;
      case 2: mode = 3;
              break;
      case 3: mode = 1;
              break;
    }
    oled.clear(0,24,128,64);
    oled.setScale(4);
    oled.setCursorXY(Xvalue,Yvalue);
    oled.print(mode);
    oled.update();
   }
   if (enc1.isLeft()) {
    switch (mode) {
      case 1: mode = 3;
        break;
      case 2: mode = 1;
        break;
      case 3: mode = 2;
        break;
    }
   oled.clear(0,24,128,64);
   oled.setScale(4);
   oled.setCursorXY(Xvalue,Yvalue);
   oled.print(mode);
   oled.update();   
   }
  if (enc1.isClick()) flag = 0;
  }
  //if (chng = 1) 
  scrn_upd(value);
}
   
void mode_switching() {
   
}

void ign() {
  RPM_clk=60/((float)(micros()-last_ign)/1000000);
  last_ign = micros();
  //Serial.println(digitalRead(ign_rd));
  //RPM_avg += (RPM - RPM_avg) * k;  // фильтр бегущего среднего, коэф. К задаёт степень фильрации
}

unsigned int RPM_func(){
    oled.clear(0,24,128,64);
    if (RPM_clk > 9) Xvalue = 45;  
    if (RPM_clk > 99) Xvalue = 35; 
    if (RPM_clk > 999) Xvalue = 20;
    //oled.setCursorXY(Xset,Yset);
    //oled.print(RPM_clk);
    //oled.update();
    if ((micros()-last_ign) > 1000000) return (0);
    //else
    //disp.displayInt(RPM_avg*10);
    //for_servo = map(RPM_avg, 0, 600, 0, 180);  //диапазон движения обратный нормальному (переменная, минимальное знач., макс. знач, мин. градус, макс градус)
    //constrain (RPM_avg, 0, 600);
    //servo_one.write(for_servo);
    return (RPM_clk);
}

unsigned int voltmeter_func() {
  return (144);
}

byte ign_timing_func() {
  
}

void scrn_upd(unsigned value) {
  oled.setScale(2);
  oled.setCursorXY(Xhead,Yhead);
  //oled.setCursorXY(30, Yhead+5);
  //if (flag == 1) {
  //  oled.setCursorXY(20,Yhead);
  //  oled.print("select");
   switch (mode) {
      case 1: oled.print("RPM");
        break;
      case 2: oled.print("volt");
        break;
      case 3: oled.print("ign");
        break;
   }
  oled.setScale(4);
  oled.setCursorXY(Xvalue,Yvalue);
  oled.print(value);
  //oled.clear();
  oled.update();
  //Xvalue = 50;
}
