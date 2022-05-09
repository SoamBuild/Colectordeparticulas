//libreria de motor 
#include <ESP_FlexyStepper.h>
//Librerias de memoria 
#include "FS.h"
#include "SD.h"
#include "SPI.h"


String myString,timed; //timestap desde memoria 
int id, stage; //id de sesion memoria y id stage;

int countiman = 0;
int detect_iman;
boolean state_botellas[] = {0, 0, 0, 0};
const int iman = 12;
const int MOTOR_X_STEP_PIN = 16;
const int MOTOR_X_DIRECTION_PIN = 17;
const int LIMIT_X_SWITCH_PIN = 13;

const int MOTOR_F_STEP_PIN = 26;
const int MOTOR_F_DIRECTION_PIN = 27;
const int LIMIT_FUNNEL_SWITCH_PIN = 14;
ESP_FlexyStepper stepper_X;
ESP_FlexyStepper stepper_FUNNEL;

void setup()
{
  Serial.begin(115200);
  //Montando Memoria
  if (!SD.begin()) {
    Serial.println("Sd no montada ");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No identificada");
    return;
  }
  pinMode(iman, INPUT_PULLUP);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);

  stepper_FUNNEL.connectToPins(MOTOR_F_STEP_PIN, MOTOR_F_DIRECTION_PIN);
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);

  stepper_FUNNEL.setSpeedInStepsPerSecond(50);
  stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(80);
  
  //Leer y actualizar valores
  readFile(SD, "/datatemp.txt");
  id = id + 1;
  gohome();
}
void loop()
{
  botella(0);
  delay(1000);
  botella(1);
  delay(1000);
  botella(2);
  delay(1000);
  botella(3);
  delay(1000);
}

void botella(float numero)
{
  int change;
  change = (int)numero;
  Serial.println(state_botellas[change]);
  double movimiento = 172;
  for (int i = 0; i < numero; i++)
  {
    movimiento = movimiento - 50;
  }
  stepper_X.setSpeedInStepsPerSecond(180);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(200);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(700);
  stepper_X.setTargetPositionInSteps(-movimiento);
  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();
  }
  delay(2000);
  buscar_botella(numero);
  delay(1000);

  if (detect_iman == true && state_botellas[change] == 0)
  {
    stepper_FUNNEL.setSpeedInStepsPerSecond(100);
    stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(300);
    stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(800);
    stepper_FUNNEL.moveRelativeInMillimeters(-15);
    while (!stepper_FUNNEL.motionComplete())
    {
      stepper_FUNNEL.processMovement();
    }
    delay(1000);
    state_botellas[change] = 1;
    stepper_FUNNEL.moveRelativeInMillimeters(15);
    while (!stepper_FUNNEL.motionComplete())
    {
      stepper_FUNNEL.processMovement();
    }
  }
}

void buscar_botella(int botella)
{
  Serial.println("searchiman");
  if (digitalRead(iman) == HIGH)
  {
    Serial.println("iman detect");
    detect_iman = true;
  }
  else
  {
    Serial.println("not detect");
    detect_iman = false;
  }
}
void gohome()
{
  Serial.println("HOME BOTELLA ...");
  stepper_X.setSpeedInStepsPerSecond(50);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(80);

  if (stepper_X.moveToHomeInSteps(-1, 5, 200, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOME BOTELLA OK");
  }
  else
  {
    Serial.println("HOME ERROR!");
    gohome();
  }
}
