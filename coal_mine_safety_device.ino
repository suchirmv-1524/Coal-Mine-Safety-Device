#include <LiquidCrystal_I2C.h>
#include <dht.h>

// Define the analog pins and digital pins used to read the sensor readings from the arduino board
#define tempPin A1
#define DHT11 8
#define MQ2pin 7
#define buzzer 9

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
dht DHT;                           // Create a DHT object

void setup() {
  // Configure buzzer pin to be an output pin
  pinMode(buzzer, OUTPUT);
  
  lcd.init();             // Initialize LCD
  lcd.backlight();        // Make sure backlight is on

  lcd.print("MQ2 warming up!");
  delay(2000);          // allow the MQ2 to warm up
  lcd.clear();           // Clear the LCD
}

void loop() {

  lcd.clear();

// Gas sensor readings
  int gasFlag = MQ2();

// DHT11 sensor readings
  float h = DHT11_Sensor();
  
// Temperature sensor
  float temperatureC = Temp();

// Buzzer
  Buzzer(gasFlag, temperatureC, h);

  delay(1000);
}

/*
`* Function that reads the output of the MQ2 sensor and returns true(1) if gas is present or returns false(0) if gas is absent.
 * Prints the value on the LCD.
 * Return type is int.
*/
int MQ2(){
  //MQ2 gas sensor reading
  int sensorValue = digitalRead(MQ2pin); // read digital output pin

  // Print the MQ2 gas sensor reading on the LCD
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

/*
`* Function that reads the output of the DHT11 sensor and returns the humidity in the atmosphere.
 * Prints the value on the LCD.
 * Return type is float.
*/
float DHT11_Sensor(){
  //DHT11 reading
  int readData = DHT.read11(DHT11);
  
  float h = DHT.humidity;

  // Print the humidity on the LCD
  lcd.setCursor(11,0);   //11,0 represent 11th row and 0th column
  lcd.print("H:");
  lcd.setCursor(0,1);
  lcd.print(h);
  lcd.print("%");

  return h;
}

/*
`* Function that reads the output of the LM35 sensor and returns the temperature of the atmosphere.
 * Prints the value on the LCD.
 * Return type is float.
*/
float Temp(){
  // LM35 reading
  int reading = analogRead(tempPin);

  // Convert that reading into voltage
  float voltage = reading * (5.0 / 1023.0);

  // Convert the voltage into the temperature in Celsius
  float temperatureC = voltage * 100;

  // Print the temperature on the LCD;
  lcd.setCursor(0, 0);     //0,0 represent the 0th row and 0th column
  lcd.print("T:");
  lcd.setCursor(2, 0);
  lcd.print(temperatureC);
  lcd.write((char)233);
  lcd.print("C");

  return temperatureC;
}

/*
 * Function that checks the temp, humidity and gas sensor values to see if they have limit the cross and then sound the alarm if they have.
 * Return type is void like your heart.
*/
void Buzzer(int gasFlag, float temperatureC, float h){
// To check if the thresholds have limit the cross 
  if(gasFlag == 1 || h > 60.0 || temperatureC > 40.0){
    tone(buzzer, 2000, 500);
    delay(1000);
    tone(buzzer, 2000, 500);
  }
  else{
    noTone(buzzer);
  }
}