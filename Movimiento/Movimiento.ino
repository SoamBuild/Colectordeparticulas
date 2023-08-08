#include <ESP_FlexyStepper.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TimeLib.h>
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
// DAY,HOUR,MINUTE,SECOND
int task1[] = {31, 13, 30, 0};
int task2[] = {2, 12, 30, 0};
int task3[] = {4, 12, 30, 0};
int task4[] = {6, 12, 30, 0};

void setup()
{

  Serial.begin(115200);
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
}
void loop()
{
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
void movefunnel(int distance)
{
  stepper_FUNNEL.setSpeedInStepsPerSecond(300);
  stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(200);
  stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(700);
  stepper_FUNNEL.setTargetPositionInSteps(distance);
  Serial.println("Movimiento embudo en mm: " + String(distance));
  while (!stepper_FUNNEL.motionComplete())
  {
    stepper_FUNNEL.processMovement();
  }
}
void homefunnel()
{
  // digitalWrite(ENABLE_MOTORS, LOW);
  stepper_FUNNEL.setSpeedInStepsPerSecond(100);
  stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(250);
  stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(250);
  if (stepper_FUNNEL.moveToHomeInSteps(-1, 5, 800, LIMIT_FUNNEL_SWITCH_PIN) == true)
  {
    Serial.println("HOME EMBUDO OK");
    movefunnel(-600);
    counthomeerror = 0;
    // digitalWrite(ENABLE_MOTORS, HIGH);
  }
  else
  {
    Serial.println("HOME ERROR!" + String(counthomeerror));
    counthomeerror = counthomeerror + 1;
    if (counthomeerror == 3)
    {
      ESP.restart();
    }
    else
    {
      homefunnel();
    }
  }
}
void movex(double distance)
{
  // digitalWrite(ENABLE_MOTORS, LOW);
  stepper_DISK.setSpeedInStepsPerSecond(400);
  stepper_DISK.setAccelerationInStepsPerSecondPerSecond(200);
  stepper_DISK.setDecelerationInStepsPerSecondPerSecond(3000);
  stepper_DISK.setTargetPositionInSteps(distance);
  Serial.println("Movimiento de disco: " + String(distance));
  while (!stepper_DISK.motionComplete())
  {
    stepper_DISK.processMovement();
  }
}
void homex()
{
  stepper_DISK.setSpeedInStepsPerSecond(200);
  stepper_DISK.setAccelerationInStepsPerSecondPerSecond(1200);
  stepper_DISK.setDecelerationInStepsPerSecondPerSecond(10000);

  if (stepper_DISK.moveToHomeInSteps(-1, 5, -3000, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOME BOTELLA OK");
    delay(3000);
    // movex(400);
    counthomeerror = 0;
    // digitalWrite(ENABLE_MOTORS, HIGH);
  }
  else
  {
    Serial.println("HOME ERROR!" + String(counthomeerror));
    counthomeerror = counthomeerror + 1;
    if (counthomeerror == 3)
    {
      ESP.restart();
    }
    else
    {
      homex();
    }
  }
}