//libreria de motor
#include <ESP_FlexyStepper.h>
//Librerias de memoria
#include "FS.h"
#include "SD.h"
#include "SPI.h"


String myString, timed; //timestap desde memoria
int id, stage; //id de sesion memoria y id stage;

int countiman = 0;
int detect_iman;
boolean state_botellas[] = {0, 0, 0, 0};
const int iman = 16;
const int MOTOR_X_STEP_PIN = 27;
const int MOTOR_X_DIRECTION_PIN = 14;
const int LIMIT_X_SWITCH_PIN = 13;

const int MOTOR_F_STEP_PIN = 33;
const int MOTOR_F_DIRECTION_PIN = 32;
const int LIMIT_FUNNEL_SWITCH_PIN = 15;
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

void botella(int numero)
{
  stage = 0;
  double movimiento = 172;
  for (int i = 0; i < numero; i++)
  {
    movimiento = movimiento - 50;
  }
  stepper_X.setSpeedInStepsPerSecond(180);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(200);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(700);
  stepper_X.setTargetPositionInSteps(-movimiento);
  Serial.println("Movimiento botella: " + String(numero));
  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();
  }
  delay(2000);
  buscar_botella(numero);
  delay(1000);
  stage = 1;
  toregistry(stage);

  if (detect_iman == true && state_botellas[numero] == 0)
  {
    Serial.println("Bajando embudo en botella:" + String(numero));
    stepper_FUNNEL.setSpeedInStepsPerSecond(150);
    stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(300);
    stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(800);
    stepper_FUNNEL.moveRelativeInMillimeters(-15);
    while (!stepper_FUNNEL.motionComplete())
    {
      stepper_FUNNEL.processMovement();
    }
    stage = 2;
    toregistry(stage);
    delay(1000);
    state_botellas[numero] = 1;
    Serial.println("Estado de botella: "+ String(state_botellas[numero]));
    stepper_FUNNEL.moveRelativeInMillimeters(15);
    while (!stepper_FUNNEL.motionComplete())
    {
      stepper_FUNNEL.processMovement();
    }
    stage = 3;
    toregistry(stage);
  }
}

void buscar_botella(int botella)
{
  Serial.println("Buscando iman de:" + String(botella));
  if (digitalRead(iman) == HIGH)
  {
    //Serial.println("iman detect");
    detect_iman = true;
  }
  else
  {

    Serial.println("### Iman no encontrado ###");
    detect_iman = false;
    if (state_botellas[botella] == 0) {
      gohome();
    }
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
    //Serial.println("HOME BOTELLA OK");
  }
  else
  {
    Serial.println("HOME ERROR!");
    gohome();
  }
}
void toregistry(int stage) {
  Serial.println("Guardando etapa: " + String(stage));
  String tosave = String(id) + ",000000," + String(stage)+","+String(state_botellas[1])+","+String(state_botellas[2])+","+String(state_botellas[3])+","+String(state_botellas[4])+"\n";
  appendFile(SD, "/Data.txt", tosave.c_str());
  writeFile(SD, "/dataTemp.txt", tosave.c_str());



}
