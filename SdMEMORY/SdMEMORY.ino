/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
void setup(){
  
  

    Serial.begin(115200);
    if(!SD.begin()){
        Serial.println("Sd no montada ");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No identificada");
        return;
    }
    createDir(SD, "/test");
    writeFile(SD, "/Data.txt", "ID,Time,Stage,B1,B2,B3,B4");
    
}


void loop(){
 

}
