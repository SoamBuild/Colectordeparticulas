#include <ESP_FlexyStepper.h>

int countiman = 0;
int detect_iman;

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
  pinMode(iman, INPUT_PULLUP);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);

  stepper_FUNNEL.connectToPins(MOTOR_F_STEP_PIN, MOTOR_F_DIRECTION_PIN);
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);

  stepper_FUNNEL.setSpeedInStepsPerSecond(50);
  stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(80);

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
  // Serial.println(numero);
  //  LA DISTANCIA ENTRE CADA BOTELLA SON 50 PASOS
  //  EL CICLO FOR RESTA 5 Y REPITE EL NUMERO DE LA FUNCION SUPERIOR
  //  ASI SE DETERMINA LA POSICION DE FORMA AUTOMATICA
  double movimiento = 171;
  for (int i = 0; i < numero; i++)
  {
    movimiento = movimiento - 50;
    // Serial.println(movimiento);
  }
  stepper_X.setSpeedInStepsPerSecond(200);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(300);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(800);
  stepper_X.setTargetPositionInSteps(-movimiento);
  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();
  }
  // detect_iman=false;
  delay(2000);
  buscar_botella(numero);
  delay(1000);

  if (detect_iman == true)
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
    detect_iman=false;
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
