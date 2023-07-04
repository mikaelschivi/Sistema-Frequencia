/*
  SLAVE
  ---------------------------
  - Buzzer
  - SD Module
  - RTC
  ---------------------------
*/

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

static int CHIPSELECTADDR = 10;
static int MSADDR = 4;
static int BUZZER = 4;

bool NEWID;
char eventData[7];

File file;
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // Checagem da inicializacao dos componentes
  if (!SD.begin(CHIPSELECTADDR)) {
    Serial.println("--> Error - Inicializacao do SD card falhou!");
    while (1)
      ;
  }
  if (!SD.exists("data.txt")) {
    Serial.println("--> Error - Lista de presenca nao encontrada em SD card!");
    Serial.println("    caso o arquivo nao exista, crie-o!");
    while (1)
      ;
  }
  Serial.println("SLAVE SETUP OK");

  rtc.begin();
  delay(10);
  if (!rtc.begin()) {
    Serial.println("--> ERROR - Modulo RTC nao encontrado!");
    while (1)
      ;
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //
    Serial.println("Master -> RTC DS3231 sync");
    // textToFile("RTC - Sincronizou", LOG);
  }  
  
  pinMode(BUZZER, OUTPUT);

  /*
    setup conexao I2C com o Arduino Master
  */
  Wire.begin(MSADDR);
  Wire.onReceive(receiveEvent);
}

void loop() {
  DateTime now = rtc.now();
  delay(500);

  if (NEWID == true && eventData != NULL) {
    file = SD.open("data.txt", FILE_WRITE);
    if (file) {
      file.print(eventData);
      file.print("-");
      file.print(now.day());
      file.print("/");
      file.print(now.month());
      file.print("-");
      file.print(now.hour());
      file.print(":");
      file.print(now.minute());
      file.println("\n");

      Serial.print(eventData);
      Serial.print("-");
      Serial.print(now.day());
      Serial.print("/");
      Serial.print(now.month());
      Serial.print("-");
      Serial.print(now.hour());
      Serial.print(":");
      Serial.print(now.minute());
      Serial.println("\n");
    }
    file.close();
    NEWID = false;
    memset(eventData, NULL, sizeof(eventData));

    delay(100);
    tone(BUZZER, 1000);  // 1KHz
    delay(300);          // ... 0.3 seconds
    noTone(BUZZER);
  }
}

/*
  Funcao base da lib Wire.h
  recebe event do master
*/
void receiveEvent(int howMany) {
  int i = 0;
  while (Wire.available() && i < 6) {
    eventData[i] = Wire.read();
    i++;
  }
  NEWID = true;
}
