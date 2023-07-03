#include <main.h>

// Khai bao 4 chan PB0, PB1, PB2, PB3, PD2 tuong ung de cap cho L298 la IN1, IN2, IN3, IN4
#define IN1 PIN_B0 
#define IN2 PIN_B1
#define IN3 PIN_B2
#define IN4 PIN_B3
#define Buzzer PIN_D2 // Khai bao coi buzzer la chan PD2

// PIN Metal Sensor:
// Khai bao cac chan cam bien do kim loai
#define Sensor_1 PIN_C0 
#define Sensor_2 PIN_C1
#define Sensor_3 PIN_C3

// Variables:
unsigned int8 recevieData;


void runBuzzer(){
   output_high(Buzzer);
   delay_ms(150);
   output_low(Buzzer);
}
void runForward(){
   // Xuat tin hieu Digital vao cac chan IN1 va IN2, IN3 va IN4
   // Ham chay toi, hieu dien the giua hai chan IN1 va IN2, IN3 va IN4 phai la 5V. Khi do 2 banh xe se chay toi
   output_high(IN1);
   output_low(IN2);
   output_high(IN3);
   output_low(IN4);
}
void runBackward(){
   // Ham chay toi, hieu dien the giua hai chan IN1 va IN2, IN3 va IN4 phai la -5V. Khi do 2 banh xe se chay lui
   output_low(IN1);
   output_high(IN2);
   output_low(IN3);
   output_high(IN4);
}
void runRight(){
   // Tuong tu cac ham o tren, luc nay hieu dien the giua IN1 va IN2 la 5V, IN3 va IN4 la -5V. Khi do hai banh xe se quay nguoc chieu
   output_high(IN3);
   output_low(IN4);
   output_low(IN1);
   output_high(IN2);
}
void runLeft(){
   output_high(IN1);
   output_low(IN2);
   output_low(IN3);
   output_high(IN4);
}

void Stop(){
   // Ham dung lai, xuat tin hieu muc thap (0V) o tat ca cac chan IN1, IN2, IN3, IN4 de dung dong co
   output_low(IN1);
   output_low(IN2);
   output_low(IN3);
   output_low(IN4);
}

void checkMetal(){
   // Ham do kim loai, neu 1 trong 3 cam bien phat hien kim loai thi coi buzzer se keu
   if((input(Sensor_1) == 0) | (input(Sensor_2) == 0) | (input(Sensor_3) == 0)){
     output_high(Buzzer);
     putc('1');
   }
   else{
     output_low(Buzzer);
     putc('0');
   }
}

unsigned int8 sysState = 0;
void main()
{
   Stop();
   while(TRUE)
   {
      recevieData = getch(); // Nhan du lieu tu board Bluetooth luu vao bien recevieData, tin hieu duoc gui tu Remote Controller.
      recevieData = recevieData - 48; // Chuyen doi tu kieu so trong bang ma ASCII sang kieu so nguyen.
      checkMetal();
      
      // Dua vao bien recevieData se dieu khien xe chay voi truong hop tuong ung.
      if(recevieData == 1){ 
         runForward();
         sysState = 1;
      }
      else if(recevieData == 2){
         runBackward();
         sysState = 1;
      }
      else if(recevieData == 3){
         runLeft();
         sysState = 1;
      }
      else if(recevieData == 4){
         runRight();
         sysState = 1;
      }
      else{
         Stop();
         sysState = 1;
      }
      
   }
}
