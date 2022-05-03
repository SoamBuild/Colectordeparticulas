/*
   Connect the SD card to the following pins:

   SD Card | ESP32
      D2       -
      D3       SS
      CMD      MOSI
      VSS      GND
      VDD      3.3V
      CLK      SCK
      VSS      GND
      D0       MISO
      D1       -
*/
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//#include <Function_SD.ino>
int id = 0;
int data[7];
String myString ="";
void setup() {



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
 // writeFile(SD, "/Data.txt", "ID,Time,Stage,B1,B2,B3,B4\n");

}


void loop() {
 /*
  id = id + 1;
  String tosave = String(id) + ",000000,3,0,1,0,2\n";
  delay(1000); 
  appendFile(SD, "/Data.txt", tosave.c_str());
  Serial.println(String(id) + ",000000,3,0,1,0,2\n");
  writeFile(SD, "/dataTemp.txt", tosave.c_str());
  */
}
