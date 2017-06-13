#include <LiquidCrystal.h>
#include <avr/sleep.h>
#include <Narcoleptic.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int lcd_key     = 0;
int adc_key_in  = 0;
int lastActionTime = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 pinMode(10, OUTPUT);
 digitalWrite(10, HIGH);
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // print a simple message
 
 attachInterrupt(A0, wakeUpNow, CHANGE);
 
}

void wakeUpNow(){
  
}

void markAction(){
  digitalWrite(10, HIGH);
  lcd.display();
  lastActionTime = millis()/1000;
}

void sleepNow()         // here we put the arduino to sleep
{
      
    set_sleep_mode(SLEEP_MODE_ADC);   // sleep mode is set here
 
    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
 
    attachInterrupt(A0,wakeUpNow, CHANGE); 
    sleep_mode();            // here the device is actually put to sleep!!                  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
 
    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
    detachInterrupt(A0);      
}

void checkIdleTime(){
  if(millis()/1000 >= lastActionTime+5){
    //digitalWrite(10, LOW);
    //lcd.noDisplay();

    //shut off extra things
    Narcoleptic.disableTimer1();
  Narcoleptic.disableTimer2();
  Narcoleptic.disableSerial();
  //Narcoleptic.disableADC();
  //Narcoleptic.disableWire();
  Narcoleptic.disableSPI();
  Narcoleptic.delay(5000);
  }
}
 int slept =0;
void loop()
{
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print(millis()/1000);      // display seconds elapsed since power-up
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
