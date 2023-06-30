/*
  Codigo responsavel pelo Arduino Interface
  Componentes que fazem parte:
  ---------------------------
  Keypad Matricial 4x4
  RTC 3231
  Fingerprint Scanner (?)
  Buzzer
  LCD 1602 I2C
  ---------------------------
*/

/* incluir bibliotecas */
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h> 

// inicializador RTC
RTC_DS3231 rtc;

// inicializador do Keypad
const byte ROWS = 4; 
const byte COLS = 4; 

/* mapeamento do Keypad */
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {11,10,9,8}; 
byte colPins[COLS] = {7,6,5,4};
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// inicializador lcd i2c
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  /* inicializacao dos componentes */
  Serial.begin(9600);
  while (!Serial){;}
  Serial.println("Master -> Serial pronto");
  
  rtc.begin();
  Wire.begin();
  lcd.init();
  lcd.backlight();
  
  if (!rtc.begin()) {
    Serial.println("Master -> RTC 3231 nao encontrado");
    // while (1);
  }
  if (rtc.lostPower()) {                                
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //
    Serial.println("Master -> RTC DS3231 sync");
    // textToFile("RTC - Sincronizou", LOG);
  }
  
  lcd.setCursor(5, 0);
  lcd.print("Aguarde..");
  delay(1000);
  lcd.setCursor(5, 1):
  lcd.print("Ok!");
  delay(1000);
  lcd.clear();
}

char id[8];
int index = 0;
void loop() {
  DateTime now = rtc.now();
  char key = keypad.getKey();
  lcd.setCursor(0, 0);
  lcd.print("Matricula: ");

  if (key) {
      if( key == '*'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Bem-vindo :)");
      delay(1000);

      Serial.println(id);

      memset(id, 0, sizeof(id));
      index = 0;

      lcd.clear();                
      lcd.setCursor(0, 0);      
      lcd.print("Aguarde..");
      delay(2200);
                  
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Insira sua matricula");
      lcd.blink_on();
    } else {
      /*
        logica pra printa na tela ao mesmo tempo
        em que a informacao e recebida pelo keypad
      */
      id[index] = key;          
      lcd.setCursor( index, 1 );
      lcd.print( id[index] );
      Serial.print( id[index] );

      if( index <= sizeof(id) ){
        index++;
      }
    }
  }  
}

/* 
  simples funcao para abrir e fechar
  transimissao com o Arduino Slave
  - pra facilita a vida de Write
*/
void SendMessageToSlave( int byte Message, int addr) {
  Wire.beginTransmission(addr);
  Wire.write( Message );
  Wire.endTransmission();
}

/*
  facilitar a vida de digitar toda hora
  lcd.clear ou lcd.setCursor/lcd.print    :d
*/
void WriteTextToLcd( int col, int row, char message bool clearDisplay) {
  if (clearDisplay ){ 
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(message);
}