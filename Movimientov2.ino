// libreria de motor
#include <ESP_FlexyStepper.h>
// Librerias de memoria
#include "FS.h"
#include "SD.h"
#include "SPI.h"

String myString, timed; // timestap desde memoria
int id, stage_routine;  // id de sesion memoria y id stage_routine;

int countiman = 0;
int detect_iman;
boolean state_botellas[] = {0, 0, 0, 0};
const int iman = 16;
const int MOTOR_X_STEP_PIN = 33;
const int MOTOR_X_DIRECTION_PIN = 32;
const int LIMIT_X_SWITCH_PIN = 13;
const int ENABLE_X = 25;

const int MOTOR_F_STEP_PIN = 27;
const int MOTOR_F_DIRECTION_PIN = 14;
const int LIMIT_FUNNEL_SWITCH_PIN = 15;
const int ENABLE_F = 26;

const int PUMP_WATER=2;
ESP_FlexyStepper stepper_X;

ESP_FlexyStepper stepper_FUNNEL;

void setup()

{
  Serial.begin(115200);

  // Montando Memoria
  if (!SD.begin())
  {
    Serial.println("Sd no montada ");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No identificada");
    return;
  }

  pinMode(iman, INPUT_PULLUP);

  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_FUNNEL_SWITCH_PIN, INPUT_PULLUP);
  pinMode(ENABLE_X, OUTPUT);
  pinMode(ENABLE_F, OUTPUT);

  pinMode(PUMP_WATER,OUTPUT);
  digitalWrite(PUMP_WATER,HIGH);
  
  digitalWrite(ENABLE_X, LOW);
  digitalWrite(ENABLE_F, HIGH);

  stepper_FUNNEL.connectToPins(MOTOR_F_STEP_PIN, MOTOR_F_DIRECTION_PIN);
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);

  stepper_FUNNEL.setSpeedInStepsPerSecond(50);
  stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(80);

  // Leer y actualizar valores
  readFile(SD, "/datatemp.txt");
  id = id + 1;

  gohome();
}
void loop()
{

  SEARCH_BOTTLE(0);
  delay(1000);

  SEARCH_BOTTLE(1);
  delay(1000);
  SEARCH_BOTTLE(2);
  delay(1000);
  SEARCH_BOTTLE(3);
  delay(1000);
}

void SEARCH_BOTTLE(int id_bottle)
{
  stage_routine = 0;
  double movimiento = 172;
  for (int i = 0; i < id_bottle; i++)
  {
    movimiento = movimiento - 50;
    Serial.println(movimiento);
  }
  stepper_X.setSpeedInStepsPerSecond(180);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(200);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(700);
  stepper_X.setTargetPositionInSteps(-movimiento);
  Serial.println("Movimiento botella: " + String(id_bottle));
  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();
  }
  delay(2000);
  SEARCH_IMAN(id_bottle);
  delay(1000);
  stage_routine = 1;
  ROUTINE_REGISTRY(stage_routine);

  if (detect_iman == true && state_botellas[id_bottle] == 0)
  {
    MOVE_FUNNEL(15, id_bottle);
    FILL_WATER(0);
    delay(1000);
    FILL_WATER(1);
    stage_routine = 2;
    ROUTINE_REGISTRY(stage_routine);
    delay(1000);
    state_botellas[id_bottle] = 1;
    Serial.println("Estado de botella: " + String(state_botellas[id_bottle]));
    MOVE_FUNNEL(-15, id_bottle);
    stage_routine = 3;
    ROUTINE_REGISTRY(stage_routine);
  }
  if (state_botellas[id_bottle] == 1)
  {
    Serial.println("Botella: " + String(id_bottle) + " Completada");
  }
}

void SEARCH_IMAN(int botella)
{

  Serial.println("Buscando iman de: " + String(botella));
  if (digitalRead(iman) == HIGH)
  {
    Serial.println("iman Detectado, esperando embudo");
    detect_iman = true;
  }
  else
  {

    Serial.println("### Iman no encontrado ###");
    detect_iman = false;
    gohome();
    /*
    if (state_botellas[botella] == 0) {
      gohome();
    }
    */
  }
}
