// Test wifi para probar rutinas basadas en eventos controlados por un servidor ntp
#include <WiFi.h>
#include "time.h"
#include <ESP_FlexyStepper.h>
#include "config.h"
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <SPIFFS.h>
// Firebase object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;
String databasePath;
// Setup data x json
String botella1 = "/B1";
String botella2 = "/B2";
String botella3 = "/B3";
String botella4 = "/B4";
String Bat = "/Batlevel";
String timePath = "/Timestamp";
String parentPath;
FirebaseJson json;
// setup NTP SERVER
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -14400;
const int daylightOffset_sec = 0;
unsigned long epochTime;
// Millis para enviar los datos cada xtiempo
unsigned long previousMillis = 0;
const long interval = 3600000; // 3600000; // Intervalo de 5 minutos en milisegundos
// Variables para leer datos desde la memoria interna
String myString;
// setup and objects stepper motors
const int MOTOR_X_STEP_PIN = 27;
const int MOTOR_X_DIRECTION_PIN = 14;
const int LIMIT_X_SWITCH_PIN = 15;
const int MOTOR_F_STEP_PIN = 33;      // 33
const int MOTOR_F_DIRECTION_PIN = 32; // 32
const int LIMIT_FUNNEL_SWITCH_PIN = 13;
const int ENABLE_MOTORS = 26;
ESP_FlexyStepper stepper_DISK;
ESP_FlexyStepper stepper_FUNNEL;
int counthomeerror = 0;
// Sensors and actuator pins
int pumpwater = 2;
int imanencoder = 0;
// Control global variables
boolean statesbotellas[] = {0, 0, 0, 0};
// Voltaje Colector Data
float voltaje = 12.2; // static x now
// Alarmas segun hora
int task1[] = {10, 17, 13, 0};
int task2[] = {11, 2, 10, 0};
int task3[] = {11, 4, 10, 0};
int task4[] = {11, 6, 10, 0};
int day, hour, minute, second;

void setup()
{
  Serial.begin(115200);

  //  readFile(SPIFFS, "/b2.txt", 1); // leer el archivo de spiff de las botellas
  Serial.println(statesbotellas[0]);
  // delay(5000);

  try_Connected();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getTime();
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  tryOn_database();
  delay(5000);
  try_Disconnected();
  // Setup OUTPUTS solo para prueba de alarmas
  pinMode(imanencoder, INPUT_PULLUP);
  pinMode(pumpwater, OUTPUT);
  pinMode(ENABLE_MOTORS, OUTPUT);
  digitalWrite(ENABLE_MOTORS, LOW);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_FUNNEL_SWITCH_PIN, INPUT_PULLUP);
  stepper_FUNNEL.connectToPins(MOTOR_F_STEP_PIN, MOTOR_F_DIRECTION_PIN);
  stepper_DISK.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  homex();
  homefunnel();
  digitalWrite(ENABLE_MOTORS, HIGH);
}
void loop()
{

  getTime();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    postData();
  }

  if (day == task1[0] && hour == task1[1] && minute == task1[2] && second == task1[3])
  {
    Serial.println("Ejecutando tarea 1");
    searchbottle(0);
  }
  if (day == task2[0] && hour == task2[1] && minute == task2[2] && second == task2[3])
  {
    Serial.println("Ejecutando tarea 2");
    searchbottle(1);
  }
  if (day == task3[0] && hour == task3[1] && minute == task3[2] && second == task3[3])
  {
    Serial.println("Ejecutando tarea 3");
    searchbottle(2);
  }
  if (day == task4[0] && hour == task4[1] && minute == task4[2] && second == task4[3])
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