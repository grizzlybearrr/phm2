// LCD5110_TinyFont_View 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the tiniest font.
//
// This program requires a Nokia 5110 LCD module.
//
// It is assumed that the LCD module is connected to
// the following pins using a levelshifter to get the
// correct voltage to the module.
//      SCK  - Pin A3
//      MOSI - Pin A2
//      DC   - Pin A1
//      RST  - Pin 13
//      CS   - Pin A0
//
//размер экрана  84х48 

#include "LCD5110_Graph.h"

LCD5110 myGLCD(A3,A2,A1,13,A0);

extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];
extern uint8_t TinyFont[];




int piezoPin = 3;
int laserPin = 7;
int checkPin = A6;

int checkPinStat = 0;
int barWidth = 0;

float part=0, timerPart=0;


//режимы и флаги
bool needUpdate = true;


//счётчики. в секундах
unsigned long miliCounter = 0; //считает милисекунды

unsigned long workTime = 0;


void pinInit() {
  pinMode(piezoPin, OUTPUT);
  pinMode(laserPin, OUTPUT);

  //Serial.begin(9600); 
}





//вывод содержимого экрана
void show() {
  if(!needUpdate) return;

  part = (float)checkPinStat/1023;


  if(workTime < 20) {
    tone(piezoPin, 500 + 1000 * timerPart * part);
    digitalWrite(laserPin, HIGH);
  }

  if(workTime > 30) {
    noTone(piezoPin);
    digitalWrite(laserPin, LOW);
  }

  

  myGLCD.clrScr();

  
   myGLCD.setFont(BigNumbers);
   myGLCD.printNumI((int)(part * timerPart * 255), CENTER, 0);

   myGLCD.setFont(SmallFont);
   myGLCD.print("polulyahov", CENTER, 26);

   myGLCD.setFont(TinyFont);
   myGLCD.print("0", 0, 35);
   myGLCD.print("255", RIGHT, 35);

   barWidth = (int)(part * timerPart * 83);

   myGLCD.clrRect(1, 42, 82, 46);
   myGLCD.drawRect(0, 41, 83, 47);
   myGLCD.fillRect(0, 41, barWidth, 47);
   
  
  myGLCD.update();
  needUpdate = false;
}


void readKeys() {
  checkPinStat = analogRead(checkPin);
  //Serial.println(checkPinStat);
}


// Прерывание вызывается один раз в миллисекунду 
SIGNAL(TIMER0_COMPA_vect) 
{
 //unsigned long currentMillis = millis();

 miliCounter++;
 if(miliCounter >= 487/10) {
  miliCounter = 0;


  workTime++;

  if(timerPart<1) {
    timerPart += 0.05;
  }

  needUpdate = true;
 }
}


void setup()
{
  pinInit();

  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.invertText(false);

  // Timer0 уже используется millis() - мы создаем прерывание где-то
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}



void loop()
{
  readKeys();
  show();
}
