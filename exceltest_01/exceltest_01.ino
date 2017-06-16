unsigned long int milli_time;    //variable to hold the time
float voltage;         //variable to hold the voltage form A0
void setup() {
  Serial.begin(128000);               //Fastest baudrate
  Serial.println("CLEARDATA");        //This string is defined as a 
                                      // commmand for the Excel VBA 
                                      // to clear all the rows and columns
  Serial.println("LABEL,Computer Time, Sinus (Wave),Volt, Volt1, Volt2, Volt3, Volt4, Volt5, Volt6");  
                                      //LABEL command creates label for 
                                      // columns in the first row with bold font
}

int x = 0;
int y = 0;
void loop() {
  x++;
  y = 100*sin(50*x);
  voltage = 5.0 * analogRead(A0) / 1024.0;
  Serial.print("DATA,TIME,");
  Serial.print(y);
  Serial.print(",");
  Serial.print(voltage);
  Serial.print(",");
  Serial.print(voltage * millis());
  Serial.print(",");
  Serial.print((voltage-millis())*23);
  Serial.print(",");
  Serial.print((voltage-millis())/23);
  Serial.print(",");
  Serial.print(voltage-millis()*23);
  Serial.print(",");
  Serial.print(voltage-millis()*23);
  Serial.print(",");
  Serial.println(voltage-millis()*voltage);
    
  delay(500);                    //Take samples every one second
}
