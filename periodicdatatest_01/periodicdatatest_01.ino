#include <LiquidCrystal.h>
#include <avr/sleep.h>
#include <Narcoleptic.h>
#include <avr/wdt.h>

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

const uint8_t SleepMode = SLEEP_MODE_PWR_DOWN;
const uint8_t SleepTime = WDTO_250MS; // WDTO_15MS, WDTO_30MS, WDTO_60MS, WDTO_120MS, WDTO_250MS, WDTO_500MS, WDTO_1S, WDTO_2S, WDTO_4S
#define SLEEPDURATION 250 //must equal above sleeptime

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
  Serial.begin(4800);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  lcd.begin(16, 2);  
  Serial.println("CLEARDATA"); 
  Serial.println("LABEL,Computer Time, Arduino Time (Millis.)");           
}

void sleepNow(){
  #ifdef BODSE
  // Turn off BOD in sleep (Arduino mega 2560 BOD triggers a RESET on activation)
  MCUCR |= _BV(BODSE);
  MCUCR |= _BV(BODS);
  #endif
  
  MCUSR = 0; //clear the MCU status register
  #ifdef WDTCSR
    WDTCSR &= ~_BV(WDE);
    WDTCSR = _BV(WDIF) | _BV(WDIE) | _BV(WDCE);
  #else
    WDTCR &= ~_BV(WDE);
    WDTCR = _BV(WDIF) | _BV(WDIE) | _BV(WDCE);
  #endif
    
  wdt_enable(SleepTime);
  wdt_reset(); //reset the watchdog timer
  #ifdef WDTCSR
    WDTCSR |= _BV(WDIE); //make sure the watchdog timer doesnt trigger a device reset
  #else
    WDTCR |= _BV(WDIE);
  #endif
  set_sleep_mode(SleepMode);

  // Disable all interrupts
  uint8_t SREGcopy = SREG; 
  cli();
  
  #ifdef EIMSK
    uint8_t EIMSKcopy = EIMSK; EIMSK = 0; //disable internal interrupt mask register
  #endif
  #ifdef PCMSK0
    uint8_t PCMSK0copy = PCMSK0; PCMSK0 = 0; //disable pin change mask registers
  #endif
  #ifdef PCMSK1
    uint8_t PCMSK1copy = PCMSK1; PCMSK1 = 0;
  #endif
  #ifdef PCMSK2
    uint8_t PCMSK2copy = PCMSK2; PCMSK2 = 0;
  #endif
  #ifdef TIMSK0
    uint8_t TIMSK0copy = TIMSK0; TIMSK0 = 0;//disable timer interrupt mask registers
  #endif
  #ifdef TIMSK1
    uint8_t TIMSK1copy = TIMSK1; TIMSK1 = 0;
  #endif
  #ifdef TIMSK2
    uint8_t TIMSK2copy = TIMSK2; TIMSK2 = 0;
  #endif
  #ifdef SPCR
    uint8_t SPCRcopy = SPCR; SPCR &= ~_BV(SPIE); //disable SPI interrupt enable to 0
  #endif
  #ifdef UCSR0B
    uint8_t UCSR0Bcopy = UCSR0B; UCSR0B &= ~(_BV(RXCIE0) | _BV(TXCIE0) | _BV(UDRIE0)); //RXCIE0: disable complete interrupts. --- TXCIE0: disable transmission complete interrupts --- UDRIE0: disable data register empty interrupts
  #endif
  #ifdef TWCR
    uint8_t TWCRcopy = TWCR; TWCR &= ~_BV(TWIE); //disable two wire interface interrupt
  #endif
  #ifdef ACSR
    uint8_t ACSRcopy = ACSR; ACSR &= ~_BV(ACIE); //disable analog comparator interrupt
  #endif
  #ifdef ADCSRA
    uint8_t ADCSRAcopy = ADCSRA; ADCSRA &= ~_BV(ADIE); //disable adc conversion interrupt
  #endif
  #if defined(SPMCSR) && defined(SPMIE)
    uint8_t SPMCSRcopy = SPMCSR; SPMCSR &= ~_BV(SPMIE); //disable spm interrupt (store program memory)
  #endif

  sei(); //set the bit and reenable interrupt for watchdog timer
  sleep_mode();            // here the device is actually put to sleep!!
  wdt_disable();           // first thing after waking from sleep: disable watchdog...

    // Reenable all interrupts
  #if defined(SPMCSR) && defined(SPMIE)
    SPMCSR = SPMCSRcopy;
  #endif
  #ifdef ADCSRA
    ADCSRA = ADCSRAcopy;
  #endif
  #ifdef ACSR
    ACSR = ACSRcopy;
  #endif
  #ifdef TWCR
    TWCR = TWCRcopy;
  #endif
  #ifdef UCSR0B
    UCSR0B = UCSR0Bcopy;
  #endif
  #ifdef SPCR
    SPCR = SPCRcopy;
  #endif
  #ifdef TIMSK2
    TIMSK2 = TIMSK2copy;
  #endif
  #ifdef TIMSK1
    TIMSK1 = TIMSK1copy;
  #endif
  #ifdef TIMSK0
    TIMSK0 = TIMSK0copy;
  #endif
  #ifdef PCMSK2
    PCMSK2 = PCMSK2copy;
  #endif
  #ifdef PCMSK1
    PCMSK1 = PCMSK1copy;
  #endif
  #ifdef PCMSK0
    PCMSK0 = PCMSK0copy;
  #endif
  #ifdef EIMSK
    EIMSK = EIMSKcopy;
  #endif
  
  SREG = SREGcopy;
    
  #ifdef WDTCSR
    WDTCSR &= ~_BV(WDIE);
  #else
    WDTCR &= ~_BV(WDIE);
  #endif
}

void markAction(){
  wokeup = true;
  digitalWrite(10, HIGH);
  lcd.display();
  lastActionTime = (millis()-diff)/1000;
}

int sleptTimeMillis = 0;
int previousDraw = 0;

/**
 * If enough time has passed since last action, disable unneeded peripherals, calibrate to correct time and sleep.
 */
void powerSaveMode(){
  if((millis()-diff)/1000 >= lastActionTime+5){
    
    if(ALLOW_SLEEP){
      digitalWrite(10, LOW);
      lcd.noDisplay();
    }
    if(wokeup)
      prev_millis = (millis()-diff);

    wokeup = false;
    //shut off extra things
    if(DISABLE_TIMER1)
      Narcoleptic.disableTimer1();
    if(DISABLE_TIMER2)
      Narcoleptic.disableTimer2();
    if(DISABLE_SERIAL){
      Serial.end();
      Narcoleptic.disableSerial();
    }
    if(DISABLE_ADC)
      Narcoleptic.disableADC();
    if(DISABLE_WIRE)
      Narcoleptic.disableWire();
    if(DISABLE_SPI)
      Narcoleptic.disableSPI();
    //Use watchdog timer
    if(ALLOW_SLEEP){
      sleepNow();
    }
    sleptTimeMillis += SLEEPDURATION;
    diff = millis() - prev_millis;
//    Serial.flush();
//    Serial.print("millis: ");
//    Serial.print(millis());
//    Serial.print(" -- prev_millis: ");
//    Serial.print(prev_millis);
//    Serial.print(" -- diff: ");
//    Serial.println(diff);
  }
}

int prevMultiple;

void loop()
{
 int currMultiple = ((sleptTimeMillis + millis())/1000);
 if( currMultiple%5 == 0 && prevMultiple != currMultiple){
    Narcoleptic.enableSerial();
    while(!Serial){};
    Serial.begin(4800);
    Serial.print("DATA,TIME,");
    Serial.println(millis());
    prevMultiple = currMultiple;
 }
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print((millis()-diff)/1000);      // display seconds elapsed since power-up (minus time spent asleep)
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
