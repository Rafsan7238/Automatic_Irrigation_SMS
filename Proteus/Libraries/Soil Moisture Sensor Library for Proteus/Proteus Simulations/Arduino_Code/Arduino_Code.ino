#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int SensorPin = A0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(1,0);
  lcd.print("www.TheEngineering");
  lcd.setCursor(4,1);
  lcd.print("Projects.com");
  lcd.setCursor(0,2);
  lcd.print("Analog Value: ");
  lcd.setCursor(0,3);
  lcd.print("Voltage: ");
  
}

void loop() {

  int SensorValue = analogRead(SensorPin);   
  float SensorVolts = analogRead(SensorPin)*0.0048828125;   
  
  lcd.setCursor(14, 2);  
  lcd.print(SensorValue);
  lcd.setCursor(9, 3);  
  lcd.print(SensorVolts);     
  lcd.print(" V");
  delay(1000);

//  sensorValue = analogRead(sensorPin);
//  lcd.setCursor(4,2);
//  lcd.print(sensorValue);
//  delay(1000);
  
}
