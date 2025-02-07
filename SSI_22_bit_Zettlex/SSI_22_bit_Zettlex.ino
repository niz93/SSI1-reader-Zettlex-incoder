#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x3F, 20, 4);

const int CLK = 7;  // Clock signal
const int DO = 8;   // Digital Output from the encoder which delivers me a 0 or 1, depending on the bar angle..
unsigned int time = 250;


unsigned long sensorWaarde = 0;
unsigned long count = 0;


void setup() {

  Serial.begin(115200);
  Serial.println("SSI reader Zettlex 22 bit");  //send sample text to monitor for debug

  lcd.init();  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("SSI reader");
  lcd.setCursor(3, 1);
  lcd.print("Zettlex 22 bit");
  lcd.setCursor(2, 2);
  lcd.print("UART 115200 BAUD");
  lcd.setCursor(2, 3);
  lcd.print("rev1 06.02.2025");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEC");
  lcd.setCursor(0, 1);
  lcd.print("DEG");
  lcd.setCursor(0, 2);
  lcd.print("RAD");
  lcd.setCursor(0, 3);
  lcd.print("mRAD");

  pinMode(CLK, OUTPUT);
  pinMode(DO, INPUT);

  digitalWrite(CLK, HIGH);
}


void loop() {

  sensorWaarde = readSensor();
  delay(time);  //Tcs waiting for another read in
}

unsigned long readSensor() {
  unsigned long dataOut = 0;
  float dataOutDeg = 0;
  float dataOutRad = 0;
  float dataOutmRad = 0;


  delayMicroseconds(1);  //Waiting for Tclkfe

  //Passing 12 times, from 0 to 1
  digitalWrite(CLK, LOW);
  delayMicroseconds(1);  //Tclk/2
  digitalWrite(CLK, HIGH);
  delayMicroseconds(1);  //Tdo valid, like Tclk/2
  digitalWrite(CLK, LOW);
  delayMicroseconds(1);  //Tclk/2
  digitalWrite(CLK, HIGH);
  delayMicroseconds(1);  //Tdo valid, like Tclk/2
  for (int x = 0; x < 22; x++) {
    digitalWrite(CLK, LOW);
    delayMicroseconds(1);  //Tclk/2
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);                        //Tdo valid, like Tclk/2
    dataOut = (dataOut << 1) | digitalRead(DO);  //shift all the entering data to the left and past the pin state to it. 1e bit is MSB
    dataOutDeg = dataOut / 11650.8444444444;
    dataOutRad = dataOut / 667544.214430109;
    dataOutmRad = dataOut / 667.544214430109;
    
  }

Serial.println(count);
  Serial.print("DEC  ");
  Serial.println(dataOut);
  Serial.print("DEG  ");
  Serial.println(dataOutDeg, 7);
  Serial.print("RAD  ");
  Serial.println(dataOutRad, 7);
  Serial.print("mRAD ");
  Serial.println(dataOutmRad, 7);
  Serial.println();




  lcd.setCursor(6, 0);
  lcd.print("      ");
  lcd.setCursor(5, 0);
  lcd.print(dataOut);
 
 lcd.setCursor(14, 1);
  lcd.print("  ");
  lcd.setCursor(5, 1);
  lcd.print(dataOutDeg, 7);
 

  lcd.setCursor(5, 2);
  lcd.print(dataOutRad, 7);

 lcd.setCursor(14, 3);
  lcd.print("   ");
  lcd.setCursor(5, 3);
  lcd.print(dataOutmRad, 7);
  count = count + 1; 

  return dataOut;
}