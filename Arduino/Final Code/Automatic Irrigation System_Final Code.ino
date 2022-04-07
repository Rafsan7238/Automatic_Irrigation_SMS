
#include <SHT21.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <Wire.h>
#define echo 9
#define trigger 10
#define tank_pump 4
#define watering_pump 13
#define moisture_sensor A0

long duration;
int distance;
int distance_percent;
int water_level;
int moisture_value;
int moist_percent;
bool waterPumpOn = false;
bool tankPumpOn = false;
int temp;
int humid;

SoftwareSerial SIM900(2, 3);
LiquidCrystal lcd(12, 11, 8, 7, 6, 5);
SHT21 sht;

void setup () {

  lcd.begin(20, 4);
  SIM900.begin(9600);
  Serial.begin(9600);

  pinMode(echo, INPUT);
  pinMode(moisture_sensor, INPUT);
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(watering_pump, OUTPUT);
  pinMode(tank_pump, OUTPUT);

  digitalWrite(watering_pump, LOW);
  digitalWrite(tank_pump, LOW);

  lcd.setCursor(0, 1);
  lcd.print("SMART IRRIGATION");
  lcd.setCursor(0, 2);
  lcd.print("CSE360");
  lcd.setCursor(0, 3);
  lcd.print("GROUP 1");
  Wire.begin();
  delay(100);
  lcd.clear();
}


void loop() {

  //W. LEVEL SENSOR
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration * 0.017; //in cm
  distance_percent = map(distance, 0, 1023, 0, 100);
  water_level = 100 - distance_percent;

  //M. LEVEL SENSOR
  moisture_value = analogRead(moisture_sensor);
  moist_percent = map(moisture_value, 0, 1023, 0, 100);
  condition();
  delay(50);
}


//SHT SENSOR
void SHT_S() {

  temp = sht.getTemperature();
  sht.reset(); //reset to get correct output on the next reading
  humid = sht.getHumidity();
  sht.reset();

  Serial.println(temp);
  Serial.println(humid);
}

//SMS
void sms() {
  if (waterPumpOn) {
waterPumpOn = false;
SIM900.println("AT+CMGF=1");
SIM900.println("AT+CMGS=\"+8801234567890\"");
SIM900.println("WATERING PUMP IS OFF");
delay(100);
}

}

void sms1() {
  if (tankPumpOn) {
tankPumpOn = false;
SIM900.println("AT+CMGF=1");
SIM900.println("AT+CMGS=\"+8801234567890\"");
SIM900.println("TANK PUMP IS OFF");
delay(100);
}

}

void sms2() {
  if (!waterPumpOn) {
waterPumpOn = true;
SIM900.println("AT+CMGF=1");
SIM900.println("AT+CMGS=\"+8801234567890\"");
SIM900.println("WATER PUMP IS ON");
delay(100);
}
 
}

void sms3() {
  if (!tankPumpOn) {
tankPumpOn = true;
SIM900.println("AT+CMGF=1");
SIM900.println("AT+CMGS=\"+8801234567890\"");
SIM900.println("TANK PUMP IS ON");
delay(100);
}
  
}

//Conditions for Pump
void condition() {

  if (water_level > 65 && moist_percent < 50) {

    digitalWrite(tank_pump, LOW);
    digitalWrite(watering_pump, HIGH);
    SHT_S();

    Serial.println(water_level);
    Serial.println("OFF"); // message to send TANK PUMP IS OFF
    sms1();
    

    Serial.println(moist_percent);
    Serial.println("ON"); // message to send WATERING PUMP IS ON
    sms2();
  }

  else if (water_level < 65 && moist_percent > 50) {

    digitalWrite(tank_pump, HIGH);
    digitalWrite(watering_pump, LOW);
    SHT_S();

    Serial.println(water_level);
    Serial.println("ON"); // message to send TANK PUMP IS ON
    sms3();

    Serial.println(moist_percent);
    Serial.println("OFF"); // message to send WATERING PUMP IS OFF
    sms();
  }

  else if (water_level > 65 && moist_percent > 50) {

    digitalWrite(tank_pump, LOW);
    digitalWrite(watering_pump, LOW);
    SHT_S();

    Serial.println(water_level);
    Serial.println("OFF"); // message to send TANK PUMP IS OFF
    sms1();

    Serial.println(moist_percent);
    Serial.println("OFF"); // message to send WATERING PUMP IS OFF
    sms();
  }

  else if (water_level < 65 && moist_percent < 50) {

    digitalWrite(tank_pump, HIGH);
    digitalWrite(watering_pump, HIGH);
    SHT_S();

    Serial.println(water_level);
    Serial.println("ON"); // message to send TANK PUMP IS ON
    sms3();

    Serial.println(moist_percent);
    Serial.println("ON"); // message to send WATERING PUMP IS ON
    sms2();
  }

  LCD();
}



void LCD() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TANK LEVEL=  ");
  lcd.print(water_level);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("MOIST LEVEL= ");
  lcd.print(moist_percent);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("TEMP=  ");
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0, 3);
  lcd.print("HUMID=  ");
  lcd.print(humid);
  lcd.print("%");
}
