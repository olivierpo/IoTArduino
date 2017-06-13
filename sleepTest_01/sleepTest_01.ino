#include <LiquidCrystal.h>
#include <avr/sleep.h>
#include <Narcoleptic.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
int lastActionTime = 0;
int diff = 0;
int prev_millis = 0;
bool wokeup = true;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define DISABLE_TIMER1 1
#define DISABLE_TIMER2 1
#define DISABLE_SERIAL 1
#define DISABLE_ADC 0
#define DISABLE_WIRE 0
#define DISABLE_SPI 1
#define ALLOW_SLEEP 1

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNONE; 
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  


 return btnNONE;
}

void setup()
{
  //Serial.begin(9600);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  lcd.begin(16, 2);              
  lcd.setCursor(0,0);
  lcd.print("Push the buttons"); // print a simple message
  
  attachInterrupt(A0, wakeUpNow, CHANGE);
 
}

void wakeUpNow(){
  
}

void markAction(){
  wokeup = true;
  digitalWrite(10, HIGH);
  lcd.display();
  lastActionTime = (millis()-diff)/1000;
}

void checkIdleTime(){
  if((millis()-diff)/1000 >= lastActionTime+5){
    
    if(ALLOW_SLEEP){
      //digitalWrite(10, LOW);
      //lcd.noDisplay();
    }
    if(wokeup)
      prev_millis = (millis()-diff);

    wokeup = false;
    //shut off extra things
    if(DISABLE_TIMER1)
      Narcoleptic.disableTimer1();
    if(DISABLE_TIMER2)
      Narcoleptic.disableTimer2();
    if(DISABLE_SERIAL)
      Narcoleptic.disableSerial();
    if(DISABLE_ADC)
      Narcoleptic.disableADC();
    if(DISABLE_WIRE)
      Narcoleptic.disableWire();
    if(DISABLE_SPI)
      Narcoleptic.disableSPI();

    //Use watchdog timer
    if(ALLOW_SLEEP)
      Narcoleptic.delay(50);
    diff = millis() - prev_millis;

    Serial.print("millis: ");
    Serial.print(millis());
    Serial.print(" -- prev_millis: ");
    Serial.print(prev_millis);
    Serial.print(" -- diff: ");
    Serial.println(diff);
  }
}
 int slept =0;
void loop()
{
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print((millis()-diff)/1000);      // display seconds elapsed since power-up
 checkIdleTime();
 

 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     markAction();
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     markAction();
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     markAction();
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     markAction();
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     markAction();
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }

}
