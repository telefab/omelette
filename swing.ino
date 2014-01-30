// SWING
// Controlling a platform with two servos
// and a touchless 3d tracking interface
// 
// Inspired from the work of Kyle McDonald:
// http://www.instructables.com/id/DIY-3D-Controller/

#include <Servo.h>

#define resolution 8
#define mains 50 // 60: north america, japan; 50: most other places

#define L11 3000
#define L12 4000
#define L21 3000
#define L22 4000
#define L31 3000
#define L32 4000

#define SERVO1 5
#define SERVO2 6
#define SMINI3 180
#define SMAIN3 180
#define SMINI2 120
#define SMAIN2 120
#define SMINI1 60
#define SMAIN1 60
Servo miniServo;
Servo mainServo;
int posMini = SMINI2;
int posMain = SMAIN2;

int dist1 = 0;
int dist2 = 0;
int dist3 = 0;

#define refresh 2 * 1000000 / mains

void setup() {
  Serial.begin(9600);

  // unused pins are fairly insignificant,
  // but pulled low to reduce unknown variables
  for(int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  for(int i = 8; i < 11; i++)
    pinMode(i, INPUT);
  
  miniServo.attach(SERVO1);
  mainServo.attach(SERVO2);
  miniServo.write(posMini);
  mainServo.write(posMain);
  
  startTimer();
}

void loop() {  
  dist1 = time(8, B00000001);
  dist2 = time(9, B00000010);
  dist3 = time(10, B00000100);
  
  rotateMiniServo();
  rotateMainServo();
  
  Serial.print(dist1, DEC);
  Serial.print(" ");
  Serial.print(dist2, DEC);
  Serial.print(" ");
  Serial.println(dist3, DEC);

} 

long time(int pin, byte mask) {
  unsigned long count = 0, total = 0;
  while(checkTimer() < refresh) {
    // pinMode is about 6 times slower than assigning
    // DDRB directly, but that pause is important
    pinMode(pin, OUTPUT);
    PORTB = 0;
    pinMode(pin, INPUT);
    while((PINB & mask) == 0)
      count++;
    total++;
  }
  startTimer();
  return (count << resolution) / total;
}

extern volatile unsigned long timer0_overflow_count;

void startTimer() {
  timer0_overflow_count = 0;
  TCNT0 = 0;
}

unsigned long checkTimer() {
  return ((timer0_overflow_count << 8) + TCNT0) << 2;
}

/*
 *
 * Rotating the mini servo
 *
*/
void rotateMiniServo(){
 if(dist1<L11){
   if(posMini > SMINI1){
       for(posMini;posMini>SMINI1;posMini--){
          miniServo.write(posMini);
          delay(15);
       }
   }
 }
 else if(L11<dist1 && dist1<L12){
     if(posMini < SMINI2){
       for(posMini;posMini<SMINI2;posMini++){
          miniServo.write(posMini);
          delay(15);
       } 
     }else if(posMini > SMINI2){
       for(posMini;posMini>SMINI2;posMini--){
          miniServo.write(posMini);
          delay(15);
       } 
     }
 }else if(dist1>L12){
   if(posMini < SMINI3){
     for(posMini;posMini<SMINI3;posMini++){
      miniServo.write(posMini);
      delay(15); 
     }
   }
 }
   
}
/*
 * Rotating the main servo
 *
 *
*/
void rotateMainServo(){
 if(dist2<L21){
   if(posMain > SMAIN1){
       for(posMain;posMain>SMAIN1;posMain--){
          miniServo.write(posMain);
          delay(15);
       }
   }
 }
 else if(L21<dist2 && dist2<L22){
     if(posMain < SMAIN2){
       for(posMain;posMain<SMAIN2;posMain++){
          miniServo.write(posMain);
          delay(15);
       } 
     }else if(posMain > SMAIN2){
       for(posMain;posMain>SMAIN2;posMain--){
          miniServo.write(posMain);
          delay(15);
       } 
     }
 }else if(dist2>L22){
   if(posMain < SMAIN3){
     for(posMain;posMain<SMAIN3;posMain++){
      miniServo.write(posMain);
      delay(15); 
     }
   }
 }
   
}
