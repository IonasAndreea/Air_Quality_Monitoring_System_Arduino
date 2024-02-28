#include <SimpleDHT.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#define GAS_SENSOR      A1
#define LDR_SENSOR      A2
#define HT_SENSOR       A5

//leds(pins:5)
#define T_LED          13
#define H_LED          10
#define BUZZER          11
#define GAS_LED         9
#define GAS_LEDR         8
#define LDR_LED         12

void init_adc()
{
    ADMUX = 0b01000101;
    ADCSRA= 0b10000111;
}
int adc_l,adc_h;

int readADC (char ch){
     ADMUX &= 0b11100000;
     ADMUX |= ch;  
    
    ADCSRA |= (1<<6);
    while(ADCSRA & (1<<6));

      adc_l = ADCL;
      adc_h = ADCH;
      return ((adc_h << 8) | adc_l);
}

int val,val_ref=500;

void gas_Readings() {
  val = readADC(4);
  Serial.print("Gas Sensor Reading: ");
  Serial.println(val);
  if (val > val_ref) {
    digitalWrite(GAS_LED, LOW);   // Turn off the GREEN LED
    digitalWrite(GAS_LEDR, HIGH);   // Turn on the RED LED
    tone(BUZZER, 20);
    lcd.setCursor(4, 0);
    lcd.print("BAD");
    Serial.print("GAS: ");
    Serial.print("BAD.");
  } else {
    digitalWrite(GAS_LED, HIGH);    // Turn on the GREEN LED
    digitalWrite(GAS_LEDR, LOW);   // Turn off the RED LED
    lcd.setCursor(4, 0);
    lcd.print("OK.");
    Serial.print("GAS: ");
    Serial.print("OK.");
    noTone(BUZZER);
  }
  delay(2);
}


SimpleDHT11 dht11(HT_SENSOR);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  // set sensor pins as inputs

    pinMode( HT_SENSOR , INPUT);
    pinMode(LDR_SENSOR , INPUT);
    pinMode( GAS_SENSOR , INPUT);
     pinMode( BUZZER , OUTPUT);
      pinMode( H_LED , OUTPUT);
      pinMode( T_LED , OUTPUT);
     pinMode( GAS_LED , OUTPUT);
       pinMode( GAS_LEDR , OUTPUT);
     pinMode( LDR_LED , OUTPUT);
    
    


  // print sensor name on the LCD
  lcd.setCursor(0, 0);
  lcd.print("GAS:   ");//Gas Sensor data will be displayed at(14,0)
  lcd.setCursor(8, 0);
  lcd.print("LDR: ");//LDR Sensor data will be displayed at(15,0)
  lcd.setCursor(0,1);// Move cursor to the next line of LCD
  lcd.print("T:   ,   ");//Humidity & temperature Sensor data will be displayed at(8,1) &(12,1) respectivel
  lcd.setCursor(8, 1);
  lcd.print("H:   ");

}
/*
//uncomment when using GAS Sensor
void gas_Readings() {
  int x = readADC(4);
  lcd.setCursor(3, 0);
  lcd.print(x);
  Serial.print("GS=");
  Serial.print(x);
 // Serial.print("\t");

   //Gas Sensor input condition for Led to glow
  if (x >= 600){
    digitalWrite(9, HIGH);
     tone(BUZZER, 2000);}
  else{
    digitalWrite(13, HIGH);
    noTone(BUZZER);}
   delay(10);

  }
*/
//uncomment when using LDR Sensor
void LDR_Readings() {
  int x = map(analogRead(LDR_SENSOR), 0, 1024, 0, 99);
  lcd.setCursor(12, 0);
  lcd.print(x);
  lcd.print("%");
  Serial.print("LUM:");
  Serial.print(x);
  Serial.print("%");
  Serial.print("\t");

  //LDR Sensor input condition for Led to glow
  if (x >= 50) {
    digitalWrite(LDR_LED, LOW);
    delay (100);
    digitalWrite(LDR_LED, HIGH);
    delay (100);
  }
  else {
    digitalWrite(LDR_LED, HIGH);
  }
  delay(2000);

}


void ht_Readings() {
  byte temperature = 0;
  byte humidity = 0;

  dht11.read(&temperature, &humidity, NULL);// <simpledht> library function to read temperature and humidity

   lcd.setCursor(2, 1);
  lcd.print((int)temperature);
  lcd.print(" C");//temperature in degree
  lcd.setCursor(10, 1);
  lcd.print((int)humidity);
  lcd.print("%");
  Serial.print("T:");
  Serial.print(temperature);
  Serial.print(" C");
  Serial.print("H:");
  Serial.print(humidity);
  Serial.print("%");
  Serial.println();


  //Temperature and humidity conditions for Led to glow
 if (temperature > 0 && temperature<22)
    digitalWrite(T_LED, HIGH);
  else
    digitalWrite(T_LED, LOW);
    delay(100);
    digitalWrite(T_LED, HIGH);
    delay (100);
    
  if (humidity > 35 && humidity < 65)
      digitalWrite(H_LED, HIGH);
  else if (temperature > 0 && temperature<30)
    digitalWrite(T_LED, HIGH);
  else
    digitalWrite(T_LED, LOW);
    delay (100);
    digitalWrite(T_LED, HIGH);
    delay (100);
    
    if (humidity > 35 && humidity<65)
      digitalWrite(H_LED, HIGH);
  else
    digitalWrite(H_LED, LOW);
    delay (100);
    digitalWrite(H_LED, HIGH);
    delay(100);
  delay (500);

}

void loop() {
  /*
     read continously from each sensor
  */
  //read values from gas sensor
 
  init_adc();
  //read values from LDR sensor
  LDR_Readings();
  gas_Readings();
  // read temp and humidity from ht sensor
  ht_Readings();
}
