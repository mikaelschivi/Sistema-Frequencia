#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <RTClib.h>

char t[32];

// RTC initializer
RTC_DS1307 RTC;

// Initializer for Matrix keypad
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {11,10,9,8}; 
byte colPins[COLS] = {7,6,5,4};
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Initializer for lcd i2c display
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Wire.begin();
  Serial.begin(9600);
  RTC.begin();

  lcd.backlight();    // turn on lcd blacklight
  lcd.init();         // turn on lcd
  
  lcd.setCursor(0, 0);                // Initial Setup
  lcd.print("Insira sua matricula");
  lcd.setCursor(0, 1);
  lcd.blink_on();
}


char id[8];
int index = 0;


void loop() {
  char key = keypad.getKey();
  
  if (key) {
      if( key == '*'){
      lcd.clear();                // Clear lcd
      lcd.setCursor(0, 0);        // Set cursor at (0,0) - Top left
      lcd.print("Bem-vindo :)");  // Print to lcd
      delay(1000);
      //tone(12, 2500, 125);        // Beep to audible response

      Serial.print('\n');
      Serial.println(id);

      //start the transmission
      Wire.beginTransmission(9); // Slave Arduino address
      Wire.write(id);
      Wire.endTransmission();

      memset(id, 0, sizeof(id));  // Set all elements of id[] to 0 which = NULL
      index = 0;                  // Set index=0 to remap counter

      lcd.clear();                
      lcd.setCursor(0, 0);      

      lcd.print("Aguarde..");
      delay(2200);

      Serial.print('\n');
                  
      lcd.clear();
      lcd.setCursor(0, 0);                // Set cursor at (0,0) - Top left
      lcd.print("Insira sua matricula");  // Print to lcd
      lcd.blink_on();                     // Cursor blink
    }

        // Typing routine
    else{
          id[index] = key;
          
          lcd.setCursor(index, 1);
          lcd.print(id[index]);
          Serial.print(id[index]);

          if( index <= sizeof(id) ){
            index++;
          }
    }
  }  
}