#include <ESP_FlexyStepper.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TimeLib.h>
#include <IridiumSBD.h>

// SAT MODULE SETUP
#define IridiumSerial Serial2
#define RXD2 16
#define TXD2 17
IridiumSBD modem(IridiumSerial);
// DATA FOR SAT MODULE
int signalQuality = -1;
int err;
bool messageSent = false;
// SD ACTION
unsigned long previousmillis_SD = 0; // Variable para almacenar el tiempo anterior
unsigned long interval_SD = 300000;  // Intervalo de tiempo desea
// SD PINS
#define SCK 18
#define MISO 4
#define MOSI 23
#define CS 5
SPIClass spi = SPIClass(VSPI);
// DATA SD READ
String myString;
// SETUP AND OBJECTS STEPPER MOTORS
const int MOTOR_X_STEP_PIN = 27;
const int MOTOR_X_DIRECTION_PIN = 14;
const int LIMIT_X_SWITCH_PIN = 15;
const int MOTOR_F_STEP_PIN = 33;
const int MOTOR_F_DIRECTION_PIN = 32;
const int LIMIT_FUNNEL_SWITCH_PIN = 13;
const int ENABLE_MOTORS = 26;
ESP_FlexyStepper stepper_DISK;
ESP_FlexyStepper stepper_FUNNEL;
int counthomeerror = 0;
// ACTUATOR AND SENSOR
const int pumpwater = 2;
const int imanencoder = 0;
// BAT READ DATA
int batin = 12;
float voltaje = 0; // static x now
// Control global variables
boolean statesbotellas[] = {1, 1, 1, 1};
// ALARM TASK 1
// day(),hour(),minute(),second()
int task1[] = {31, 13, 30, 0};
int task2[] = {2, 12, 30, 0};
int task3[] = {4, 12, 30, 0};
int task4[] = {6, 12, 30, 0};

void setup()
{
  Serial.begin(115200);

  while (!Serial)
    ;
  // BEGIN AND CHECK SATMODULE
  IridiumSerial.begin(19200);
  Serial.println("Starting modem...");
  err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    Serial.print("Begin failed: error ");
    Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED)
      Serial.println("No modem detected: check wiring.");
    return;
  }
  // TASKCHECKSIGNAL
  checkSignal();
  // TASKGETTIMESAT
  gettimeSat();
  // BEGIN AND CHECK SD
  spi.begin(SCK, MISO, MOSI, CS);
  if (!SD.begin(CS, spi, 80000000))
  {
    Serial.println("Card Mount Failed");
    ESP.restart();
    return;
  }
  else
  {
    Serial.println("SD Mount OK");
  }
  // INIT INPUT AND OUTPUT
  pinMode(imanencoder, INPUT_PULLUP);
  pinMode(pumpwater, OUTPUT);
  pinMode(ENABLE_MOTORS, OUTPUT);
  digitalWrite(ENABLE_MOTORS, LOW);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_FUNNEL_SWITCH_PIN, INPUT_PULLUP);
  stepper_FUNNEL.connectToPins(MOTOR_F_STEP_PIN, MOTOR_F_DIRECTION_PIN);
  stepper_DISK.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  homex();
  movex(600);
  homefunnel();
  digitalWrite(ENABLE_MOTORS, HIGH);
}
void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousmillis_SD >= interval_SD)
  {
    previousmillis_SD = currentMillis; // Actualizar el tiempo anterior de la función cada 5 minutos
    int readbat_A = analogRead(batin);
    voltaje = ((readbat_A / 4095.0) * 3.3) * 4.92;
    ROUTINE_REGISTRY(voltaje);
  }

  // Resto del código del programa

  if (day() == task1[0] && hour() == task1[1] && minute() == task1[2] && second() == task1[3])
  {
    Serial.println("Ejecutando tarea 1");
    searchbottle(0);
  }
  if (day() == task2[0] && hour() == task2[1] && minute() == task2[2] && second() == task2[3])
  {
    Serial.println("Ejecutando tarea 2");
    searchbottle(2);
  }
  if (day() == task3[0] && hour() == task3[1] && minute() == task3[2] && second() == task3[3])
  {
    Serial.println("Ejecutando tarea 3");
    searchbottle(1);
  }
  if (day() == task4[0] && hour() == task4[1] && minute() == task4[2] && second() == task4[3])
  {
    Serial.println("Ejecutando tarea 4");
    searchbottle(3);
  }
}

void searchbottle(int idbotella)
{

  Serial.println("Botella: " + String(idbotella) + " Estado: " + String(statesbotellas[idbotella]));

  if (statesbotellas[idbotella] == true)
  {
    int localdistance = 2750;
    digitalWrite(ENABLE_MOTORS, LOW);
    for (int i = 0; i < idbotella; i++)
    {
      Serial.println(localdistance);
      localdistance = localdistance - 790;
    }
    homex();
    movex(localdistance);
    delay(1000);
    movefunnel(20);
    delay(1000);
    digitalWrite(ENABLE_MOTORS, HIGH);
    digitalWrite(pumpwater, HIGH);
    delay(8000);
    digitalWrite(pumpwater, LOW);
    delay(5000);
    digitalWrite(ENABLE_MOTORS, LOW);
    movefunnel(-600);
    digitalWrite(ENABLE_MOTORS, HIGH);
    statesbotellas[idbotella] = 0;
  }
  else
  {
    Serial.println("Botella " + String(idbotella) + " Completa");
  }
}