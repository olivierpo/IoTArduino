byte transmit = 7; //define our transmit pin
byte data = 170; //value to transmit, binary 10101010
byte mask = 1; //our bitmask
byte bitDelay = 10000;

void setup()
{
Serial.begin(9600);
}

void printBits(byte myByte){
 for(byte mask = 0x80; mask; mask >>= 1){
   if(mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }
}

void loop()
{
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
    if (data & mask){ // if bitwise AND resolves to true
      printBits(mask);
      Serial.print(" --> ");
      Serial.println("true"); // send 1
    }
    else{ //if bitwise and resolves to false
      printBits(mask);
      Serial.print(" --> ");
      Serial.println("false"); // send 0
    }
    delay(1000); //delay
  }
}
