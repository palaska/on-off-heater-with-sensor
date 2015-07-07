#include <LiquidCrystal.h>
#include <OneWire.h>

#define POTENTIOMETER A1

#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
OneWire ds(A0);

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

int backLight = 13;    // pin 13 will control the backlight
int i;
void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
  pinMode(6, OUTPUT);
  //  pinMode(POTENTIOMETER, INPUT);

  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
  lcd.begin(16, 2);             // columns, rows.  use 16,2 for a 16x2 LCD, etc.
  lcd.clear();
}

void loop(void) {
  float pot = 0.0;
  float temperature = 100;

  // calculating temperature
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  //find a device
  if (!ds.search(addr)) {
    ds.reset_search();
    //return false;
  }
  if (OneWire::crc8( addr, 7) != addr[7]) {
    //return false;
  }
  if (addr[0] != DS18S20_ID && addr[0] != DS18B20_ID) {
    //return false;
  }
  ds.reset();
  ds.select(addr);
  // Start conversion
  ds.write(0x44, 1);
  // Wait some time...
  delay(1850);
  present = ds.reset();
  ds.select(addr);
  // Issue Read scratchpad command
  ds.write(0xBE);
  // Receive 9 bytes
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  // Calculate temperature value
  temperature = ( (data[1] << 8) + data[0] ) * 0.0625;

  pot = analogRead(POTENTIOMETER) / 30.0 + 20;


  lcd.setCursor(0, 0);
  lcd.print("Sns: ");
  lcd.print(temperature);
  lcd.print("'C");

  lcd.setCursor(0, 1);
  lcd.print("Tar: ");
  lcd.print(pot);
  lcd.print("'C");

  Serial.print("The sensor temperature is: ");
  Serial.print(temperature);
  Serial.print(" *C --- Target temperature is: ");
  Serial.print(pot);
  Serial.print(" *C --- Heater: ");
  if ((pot - temperature) > 0.5 ) {
    digitalWrite(6, HIGH);
    Serial.println("ON");
    lcd.print("--ON");
  } else {
    digitalWrite(6, LOW);
    Serial.println("OFF");
    lcd.print("-OFF");
  }

}
