{{
 ***************** Simple PLX-DAQ Example *****************
  Simple PLX-DAQ routine for data.
  Plots the sine of angle from 0 to 360 degrees.
  Program will send data on serial port to be
  accepted and placed into an Excel worksheet.

  This example also demonstrates reading the current ROW from Excel,
  and starting over again once it exceeds 300.

  Check "Reset on Connect" to catch configuration data.

  January, 2007
  By Martin Hebel
  SelmaWare Solutions - www.selmaware.com
  Southern Illinois University Carbondale - www.siu.edu/~isat/est

 USE F11 WHEN DOWLOADING TO SAVE TO EEPROM AS PLX-DAQ WILL CYCLE DTR

 The PLX-DAQ Library must be created and started
                ┌────────────────────────────────────────────────┐   
                │ OBJ                                            │
                │  PDAQ : "PLX_DAQ"                              │
                │                                                │
                │ Pub Start | Angle, Row                         │
                │  PDAQ.start(31,30,0,9600) ' Rx,Tx, Mode, Baud  │
                └────────────────────────────────────────────────┘
                 
 ***************** PLX-DAQ data structures used ****************

  PLX-DAQ directives used and how called:
  DATA          Places data into the next row of Excel spreadsheet
                Up to 26 comma-separated values may be stored.
                Each value following "DATA," must be separated by comma-strings ","
                PLX-DAQ String: DATA,val1,val2,val3...Val26
                PLX-DAQ String: DATA,TIME,TIMER, decimal value of X, Signed-Decimal value of X

                The Object library allows sending data as values or as text.
                When 1st called, if TimeStamp is enabled, "TIME,TIMER" will
                be sent before data is sent.  A complete data set must end
                with a .CR call.
               ┌────────────────────────────────────────────────┐   
               │ PDAQ.Data(value1)           ' Value            │
               │ PDAQ.DataDiv(Value2,1000)   ' Value/1000       │
               │ PDAQ.DataText(string("On"))                    │
               │ PDAQ.CR                                        │
               └────────────────────────────────────────────────┘
  
  TIME          Replaced by PLX-DAQ with real system time (DATE may also be used)
                in DATA directive.  Column cells should be formatted for time.
               ┌────────────────────────────────────────┐   
               │ PDAQ.Label(String("TIME,ext"))         │
               │ PDAQ.DataText(String("TIME"))          │
               └────────────────────────────────────────┘
               When writing Data, if TimeStamp is enabled, "TIME,TIMER" will
               automatically be included when data is sent. It is true by default.
               ┌────────────────────────────────────────┐   
               │ PDAQ.TimeStamp(True)                   │
               └────────────────────────────────────────┘                    

  TIMER         Replaced by PLX-DAQ with the seconds since last may also be used
                to show time since last RESETTIMER.
                Column should NOT be formatted for Time.
               ┌────────────────────────────────────────┐   
               │ PDAQ.Label(String("TIMER,ext"))        │
               │ PDAQ.DataText(String("TIMER"))         │
               └────────────────────────────────────────┘
               When writing Data, if TimeStamp is enabled, "TIME,TIMER" will
               automatically be included when data is sent. It is true by default.
               ┌────────────────────────────────────────┐   
               │ PDAQ.TimeStamp(True)                   │
               └────────────────────────────────────────┘          
                
  LABEL         Places headings on the columns for rows A-J using up to
                26 comma-separated labels.
                PLX-DAQ String: LABEL,label1,label2,...label10
               ┌────────────────────────────────────────┐ 
               │ PDAQ.Label(string("label1,label2,etc"))│
               └────────────────────────────────────────┘
               
  CLEARDATA     Clears columns A-J, rows 2 and on. (labels remain).
                PLX-DAQ String: CLEARDATA
               ┌────────────────────────────────────────┐ 
               │ PDAQ.ClearData                         │
               └────────────────────────────────────────┘
  
  RESETTIMER    Resets the timer to 0.
                PLX-DAQ String: CLEARDATA  
               ┌────────────────────────────────────────┐   
               │ PDAQ.ResetTimer                        │
               └────────────────────────────────────────┘
 
  ROW,GET       Requests the last row data went into.
                PLX-DAQ String: ROW,GET
               ┌────────────────────────────────────────┐   
               │ Row := PDAQ.RowGet                     │
               └────────────────────────────────────────┘

  ROW,SET,val   Sets the row the next data set will use.
                PLX-DAQ String: ROW,SET,2  
               ┌────────────────────────────────────────┐     
               │ PDAQ.RowSet(2)                         │
               └────────────────────────────────────────┘

  MSG           Sets a text message in the PLX-DAQ control
                PLX-DAQ String: MSG,hello
               ┌────────────────────────────────────────────────┐                                   
               │ PDAQ.Msg(string("Hello"))                      │
               └────────────────────────────────────────────────┘
 **************************************************************
}}
CON
    _clkmode = xtal1 + pll16x
    _xinfreq = 5_000_000

    CR = 13
                       
OBJ
   PDAQ : "PLX-DAQ"                

Pub Start  | Angle, Row
  PDAQ.start(31,30,0,9600)                              ' Rx,Tx, Mode, Baud  
  PDAQ.Label(string("Time,Timer,X,SIN X"))              ' Label the spreadsheet columns
  PDAQ.ClearData                                        ' Clear present data
  PDAQ.ResetTimer                                       ' Reset timer for seconds interval
 
  repeat
    repeat Angle from 0 to 359                          ' Count from 0 to 359
      PDAQ.DataText(string("TIME,TIMER"))               ' Place current time and time since reset
      PDAQ.Data(Angle)                                  ' Send data of angle
      PDAQ.DataDiv(Sin(Angle),1000)                     ' Send data of sin of angle / 1000
      PDAQ.CR                                           ' End of data for row
      Row:=PDAQ.RowGet                                  ' Read current row
      If Row => 300                                     ' Greater than 300?
         PDAQ.RowSet(2)                                     ' back to row 2
         PDAQ.Msg(string("Restarting Data"))                ' Post message to control            
      Pause(100)                                        ' 100mSec Pause


     
PUB Sin(angle)                  ' SIN angle is 13-bit ; Returns a 16-bit signed value
                                ' Code adapted from Parallax forums to use internal SIN tables
    Angle := (Angle * 1024)/45
    Result := angle << 1 & $FFE
    if angle & $800
       Result := word[$F000 - Result]
    else
       Result := word[$E000 + Result]
    if angle & $1000
       -Result
    Result := (Result * 1000/65535)

PUB Pause(mS)
  waitcnt(clkFreq/1000 * ms + cnt)