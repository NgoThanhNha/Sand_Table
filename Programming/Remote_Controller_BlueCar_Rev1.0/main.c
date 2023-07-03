#include <main.h>
#include <string.h>

// Khai bao cac chan LCD16x2:
#define LCD_ENABLE_PIN PIN_D0
#define LCD_RS_PIN PIN_D1
#define LCD_RW_PIN PIN_D2
#define LCD_DATA4 PIN_D7
#define LCD_DATA5 PIN_D6
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D4

// Khai bao cac chan cho 4 nut nhan
#define Button_Up       PIN_C0                          
#define Button_Down     PIN_C1 
#define Button_Left     PIN_C2                           
#define Button_Right    PIN_C3  
#int_RDA

unsigned int8 recevieData = 0;
unsigned int8 metalCheck = 0;

// Ham de xuat tin hieu logic (0 hoac 1) ra mot chan cua PIC: digitalWrite(chan muon xuat tin hieu, muc Logic);
void digitalWrite(char Pin, unsigned int1 Logic){
   if(Logic == 1){
      output_high(Pin);
   }
   else{
      output_low(Pin);
   }
}

// Cac ham de khoi tao va hien thi ky tu ra man hinh LCD, duoc viet dua tren datasheet cua HD44780:
//-----------------------------------------LCD Function------------------------------------------
void LCD_sendCmd(unsigned int8 cmdData){
   digitalWrite(LCD_RS_PIN, 0);
   digitalWrite(LCD_RW_PIN, 0);
   digitalWrite(LCD_ENABLE_PIN, 0);
   
   // Send higher 4 bit:
   digitalWrite(LCD_DATA7, ((cmdData >> 7) & 0x01));
   digitalWrite(LCD_DATA6, ((cmdData >> 6) & 0x01));
   digitalWrite(LCD_DATA5, ((cmdData >> 5) & 0x01));
   digitalWrite(LCD_DATA4, ((cmdData >> 4) & 0x01));
   digitalWrite(LCD_ENABLE_PIN, 1);
   delay_us(100);
   digitalWrite(LCD_ENABLE_PIN, 0);
   delay_ms(1);
   
   // Send lower 4 bit:
   digitalWrite(LCD_DATA7, ((cmdData >> 3) & 0x01));
   digitalWrite(LCD_DATA6, ((cmdData >> 2) & 0x01));
   digitalWrite(LCD_DATA5, ((cmdData >> 1) & 0x01));
   digitalWrite(LCD_DATA4, (cmdData & 0x01));
   digitalWrite(LCD_ENABLE_PIN, 1);
   delay_us(100);
   digitalWrite(LCD_ENABLE_PIN, 0);
   delay_ms(1);
}

void LCD_Initilize(){
   delay_ms(15);
   digitalWrite(LCD_ENABLE_PIN, 0);
   
   digitalWrite(LCD_RS_PIN, 0);
   digitalWrite(LCD_RW_PIN, 0);
   digitalWrite(LCD_DATA7, 0);
   digitalWrite(LCD_DATA6, 0);
   digitalWrite(LCD_DATA5, 1);
   digitalWrite(LCD_DATA4, 1);
   digitalWrite(LCD_ENABLE_PIN, 1);
   delay_us(100);
   digitalWrite(LCD_ENABLE_PIN, 0);
   
   delay_ms(5); //Delay 5ms
   
   digitalWrite(LCD_RS_PIN, 0);
   digitalWrite(LCD_RW_PIN, 0);
   digitalWrite(LCD_DATA7, 0);
   digitalWrite(LCD_DATA6, 0);
   digitalWrite(LCD_DATA5, 1);
   digitalWrite(LCD_DATA4, 1);
   digitalWrite(LCD_ENABLE_PIN, 1);
   delay_us(100);
   digitalWrite(LCD_ENABLE_PIN, 0);
   
   delay_ms(1);
   
   LCD_sendCmd(0x32);
   delay_ms(1);
   LCD_sendCmd(0x28); //N = 1, F = 0
   delay_ms(1);
   LCD_sendCmd(0x08);
   delay_ms(1);  
   LCD_sendCmd(0x01);
   delay_ms(1);
   LCD_sendCmd(0x06);
   delay_ms(1);
   LCD_sendCmd(0x0F); //Display ON, Cursor ON, Blink ON
   delay_ms(1);
}

void LCD_sendChar(unsigned int8 charData){
   digitalWrite(LCD_RS_PIN, 1);
   digitalWrite(LCD_RW_PIN, 0);
   digitalWrite(LCD_ENABLE_PIN, 0);
   
   // Send higher 4 bit:
   digitalWrite(LCD_DATA7, ((charData >> 7) & 0x01));
   digitalWrite(LCD_DATA6, ((charData >> 6) & 0x01));
   digitalWrite(LCD_DATA5, ((charData >> 5) & 0x01));
   digitalWrite(LCD_DATA4, ((charData >> 4) & 0x01));
   digitalWrite(LCD_ENABLE_PIN, 1);
   delay_us(100);
   digitalWrite(LCD_ENABLE_PIN, 0);
   delay_ms(1);
   
   // Send lower 4 bit:
   digitalWrite(LCD_DATA7, ((charData >> 3) & 0x01));
   digitalWrite(LCD_DATA6, ((charData >> 2) & 0x01));
   digitalWrite(LCD_DATA5, ((charData >> 1) & 0x01));
   digitalWrite(LCD_DATA4, (charData & 0x01));
   digitalWrite(LCD_ENABLE_PIN, 1);
   delay_us(100);
   digitalWrite(LCD_ENABLE_PIN, 0);
   delay_ms(1);
}


void LCD_setCursor(unsigned int8 posX, unsigned int8 posY){
char cursorPos[2][16] = {{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
                                 {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F}
                                };
   unsigned int8 cursorPosition = (cursorPos[posY - 1][PosX - 1]) | 0x80;
   LCD_sendCmd(cursorPosition);
}

void LCD_BlinkON(unsigned int1 Blink){
   if(Blink == 1){
      LCD_sendCmd(0x0F);
   }
   else{
      LCD_sendCmd(0x0C);
   }
}

void LCD_print(unsigned int8 *p){
   #define LCD_printString(str) LCD_print((unsigned int8 *)(str))
   unsigned int8 i = 0;
   while(*(p+i) != 0)
   {
      LCD_sendChar(*(p+i));
      i++;
   } 
}
void LCD_printNumber(unsigned int8 Num){
   Num = Num | 0x30;
    LCD_sendChar(Num);
}

//--------------------------------------------End LCD_Func-------------------------------------------------------------

void main()
{

   delay_ms(100);
   LCD_Initilize();
   LCD_BlinkON(0);
   LCD_setCursor(2,1); // Set con tro toi vi tri thu 2 o hang 1.
   LCD_printString("Bluetooth Car"); // Hien thi ky tu len LCD.
   LCD_setCursor(4,2);  // Set con tro toi vi tri thu 4 o hang 2.
   LCD_printString("PIC16F887"); // Hien thi ky tu len LCD.

   while(TRUE)
   {  
      // Ham doc gia tri tu nut nhan, se gui UART den board Bluetooth voi ky tu tuong ung:
      //    Button Up duoc nhan: gui '1'
      //    Button Down duoc nhan: gui '2'
      //    Button Left duoc nhan: gui '3'
      //    Button Down duoc nhan: gui '4'
      
      if(input(Button_Up) == 0){
         putc('1');
         delay_ms(200);
      }
      else if(input(Button_Down) == 0){
         putc('2');
         delay_ms(200);
      }
      else if(input(Button_Left) == 0){
         putc('3');

         delay_ms(200);
      }
      else if(input(Button_Right) == 0){
         putc('4');
         delay_ms(200);
      }
      else{
         putc('0');
      }
      }
}      



