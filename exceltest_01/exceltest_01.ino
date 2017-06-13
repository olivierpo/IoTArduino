unsigned long int milli_time;    //variable to hold the time
float voltage;         //variable to hold the voltage form A0
void setup() {
  Serial.begin(9600);               //Fastest baudrate
  Serial.println("CLEARDATA");        //This string is defined as a 
                                      // commmand for the Excel VBA 
                                      // to clear all the rows and columns
  Serial.println("LABEL,Computer Time,Time (Milli Sec.),Volt, Volt1, Volt2, Volt3, Volt4, Volt5, Volt6");  
                                      //LABEL command creates label for 
                                      // columns in the first row with bold font
}
void loop() {
  milli_time = millis();
  voltage = 5.0 * analogRead(A0) / 1024.0;
  Serial.print("DATA,TIME,");
  Serial.print(milli_time);
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
