#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define tempPin A1
#define DHT11 8
#define MQ2pin 7
#define buzzer 9

LiquidCrystal_I2C lcd(0x27,16,2);  
dht DHT;                           
void setup() {
  pinMode(buzzer, OUTPUT);
  
  lcd.init();             
  lcd.backlight();       

  lcd.print("MQ2 warming up!");
  delay(2000);          
  lcd.clear();          
}

void loop() {

  lcd.clear();

  int gasFlag = MQ2();

  float h = DHT11_Sensor();
  
  float temperatureC = Temp();

  Buzzer(gasFlag, temperatureC, h);

  delay(1000);
}

int MQ2(){
  int sensorValue = digitalRead(MQ2pin); 

  lcd.setCursor(8,1);
  lcd.print("Gas:");
  if(sensorValue){
    lcd.print("NO");
    return 0;
  }
  else{
    lcd.print("YES");
    return 1;
  }
}

float DHT11_Sensor(){
  int readData = DHT.read11(DHT11);
  
  float h = DHT.humidity;

  lcd.setCursor(11,0); 
  lcd.print("H:");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print("%");

  return h;
}

float Temp(){
  int reading = analogRead(tempPin);

  float voltage = reading * (5.0 / 1023.0);

  float temperatureC = voltage * 100;

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(2, 0);
  lcd.print(temperatureC);
  lcd.write((char)233);
  lcd.print("C");

  return temperatureC;
}

void Buzzer(int gasFlag, float temperatureC, float h){
  if(gasFlag == 1 || h > 60.0 || temperatureC > 40.0){
    tone(buzzer, 2000, 500);
    delay(1000);
    tone(buzzer, 2000, 500);
  }
  else{
    noTone(buzzer);
  }
}
