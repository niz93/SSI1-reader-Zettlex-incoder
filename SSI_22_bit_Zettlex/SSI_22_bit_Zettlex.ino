#include <FastGPIO.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x3F, 20, 4);

const int CLK = 7;  // Clock signal
const int DO = 8;   // Digital Output from the encoder which delivers me a 0 or 1, depending on the bar angle..


unsigned time_half_period = 5;  // 1/2T Clock signal
unsigned int time = 400;

byte k = 100;
unsigned long sensorWaarde = 0;

float sensorWaardeDeg = 0;
float sensorWaardeRad = 0;
float sensorWaardemRad = 0;
unsigned long count = 0;


void setup() {

  Serial.begin(115200);
  Serial.println("SSI reader Zettlex 22 bit");  //send sample text to monitor for debug

  lcd.init();  // initialize the lcd
  lcd.init();  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("SSI reader");
  lcd.setCursor(3, 1);
  lcd.print("Zettlex 22 bit");
  lcd.setCursor(2, 2);
  lcd.print("UART 115200 BAUD");
  lcd.setCursor(2, 3);
  lcd.print("rev2 06.03.2025");

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


  FastGPIO::Pin<CLK>::setOutput(HIGH);
  FastGPIO::Pin<DO>::setInput();
}


void loop() {


  sensorWaarde = readSensor();
  delay(time);  //Tcs waiting for another read in

  /*  
unsigned long sensorWaardeSUM = 0;
 for (int x = 0; x < k; x++) {
    sensorWaardeSUM = sensorWaardeSUM + readSensor();
    delay(time);
  }
  sensorWaarde = sensorWaardeSUM / k;
 */
  sensorWaardeDeg = sensorWaarde / 11650.8444444444;
  sensorWaardeRad = sensorWaarde / 667544.214430109;
  sensorWaardemRad = sensorWaarde / 667.544214430109;

  Serial.println(count);
  Serial.print("DEC  ");
  Serial.println(sensorWaarde);
  Serial.print("DEG  ");
  Serial.println(sensorWaardeDeg, 7);
  Serial.print("RAD  ");
  Serial.println(sensorWaardeRad, 7);
  Serial.print("mRAD ");
  Serial.println(sensorWaardemRad, 7);
  Serial.println();
  Serial.flush();


  if (sensorWaarde < 10) {
    lcd.setCursor(6, 0);
    lcd.print(" ");
  }
  if (sensorWaarde < 100) {
    lcd.setCursor(7, 0);
    lcd.print(" ");
  }
  if (sensorWaarde < 1000) {
    lcd.setCursor(8, 0);
    lcd.print(" ");
  }
  if (sensorWaarde < 10000) {
    lcd.setCursor(9, 0);
    lcd.print(" ");
  }
  if (sensorWaarde < 100000) {
    lcd.setCursor(10, 0);
    lcd.print(" ");
  }
  if (sensorWaarde < 1000000) {
    lcd.setCursor(11, 0);
    lcd.print(" ");
  }

  lcd.setCursor(5, 0);
  lcd.print(sensorWaarde);



  if (sensorWaardeDeg < 10) {
    lcd.setCursor(14, 1);
    lcd.print(" ");
  }
  if (sensorWaardeDeg < 100) {
    lcd.setCursor(15, 1);
    lcd.print(" ");
  }
  lcd.setCursor(5, 1);
  lcd.print(sensorWaardeDeg, 7);


  lcd.setCursor(5, 2);
  lcd.print(sensorWaardeRad, 7);




  if (sensorWaardemRad < 10) {
    lcd.setCursor(14, 3);
    lcd.print(" ");
  }
  if (sensorWaardemRad < 100) {
    lcd.setCursor(15, 3);
    lcd.print(" ");
  }
  if (sensorWaardemRad < 1000) {
    lcd.setCursor(16, 3);
    lcd.print(" ");
  }
  lcd.setCursor(5, 3);
  lcd.print(sensorWaardemRad, 7);

  count = count + 1;
}

unsigned long readSensor() {
  unsigned long dataOut = 0;
  noInterrupts();

  for (int x = 0; x < 2; x++) { // Pass first BIT
    FastGPIO::Pin<CLK>::setOutputValueLow();
    delayMicroseconds(time_half_period);  //Tclk/2
    FastGPIO::Pin<CLK>::setOutputValueHigh();
    delayMicroseconds(time_half_period);  //Tdo valid, like Tclk/2
  }
  for (int x = 0; x < 22; x++) { // Read Data BIT
    FastGPIO::Pin<CLK>::setOutputValueLow();
    delayMicroseconds(time_half_period);  //Tclk/2
    FastGPIO::Pin<CLK>::setOutputValueHigh();
    delayMicroseconds(time_half_period);                          //Tdo valid, like Tclk/2
    dataOut = (dataOut << 1) | FastGPIO::Pin<DO>::isInputHigh();  //shift all the entering data to the left and past the pin state to it. 1e bit is MSB
  }
  interrupts();

  return dataOut;
}