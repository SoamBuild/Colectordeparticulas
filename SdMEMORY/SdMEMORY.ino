#include "FS.h"
#include "SD.h"
#include "SPI.h"

int id;
String timed;
int stage;
int b1;
int b2;
int b3;
int b4;
String myString;
void setup() 
{
  Serial.begin(115200);
  if (!SD.begin()) {
    Serial.println("Sd no montada ");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No identificada");
    return;
  }
  readFile(SD, "/datatemp.txt");
  id = id + 1;
  writeFile(SD, "/Data.txt", "ID,Time,Stage,B1,B2,B3,B4\n");
}
void loop() 
{
  String tosave = String(id) + ",000000,3,0,1,0,2\n";
  delay(1000);
  appendFile(SD, "/Data.txt", tosave.c_str());
  Serial.println(String(id) + ",000000,3,0,1,0,2\n");
  writeFile(SD, "/dataTemp.txt", tosave.c_str());
}
