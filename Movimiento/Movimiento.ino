#include <ESP_FlexyStepper.h>

bool counter = true;
const int iman = 12;
int count_iman = 0;
const int MOTOR_X_ENABLE = 18;
const int MOTOR_X_STEP_PIN = 16;
const int MOTOR_X_DIRECTION_PIN = 17;
const int LIMIT_X_SWITCH_PIN = 13;
ESP_FlexyStepper stepper_X;

void setup()
{
  Serial.begin(115200);
  pinMode(iman, INPUT_PULLUP);
  pinMode(MOTOR_X_ENABLE, OUTPUT);
  digitalWrite(MOTOR_X_ENABLE, LOW);

  // HOME
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  stepper_X.setSpeedInStepsPerSecond(50);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(80);

  if (stepper_X.moveToHomeInSteps(-1, 5, 200, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOME OK");
  }
  else
  {
    Serial.println("HOME ERROR");
  }
}
void loop()
{
  sbotella(4);
  delay(2000);
  sbotella(3);
  delay(2000);
  sbotella(2);
  delay(2000);
  sbotella(1);

  /*

    delay(5000);
    botella(1);
    delay(5000);
    botella(2);
    delay(5000);
    botella(3);
    delay(5000);
  */
}
void sbotella(int numero_botella)
{

  stepper_X.setSpeedInStepsPerSecond(80);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(200);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(500);
  if(numero_botella>=3){
     stepper_X.setTargetPositionInSteps(-200);
  }else
  {
    stepper_X.setTargetPositionInSteps(-100);
  }

  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();
    buscar_botella(numero_botella);
    stopmotor(numero_botella);
    Serial.println(count_iman);
  }
  if (stepper_X.motionComplete())
  {
    count_iman = 0;
    gohome();
  }
}

void gohome()
{
  stepper_X.setSpeedInStepsPerSecond(50);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(80);

  if (stepper_X.moveToHomeInSteps(-1, 5, 200, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOME OK");
  }
  else
  {
    Serial.println("HOME ERROR");
  }
}

void botella(float numero)
{
  // LA DISTANCIA ENTRE CADA BOTELLA SON 50 PASOS
  // EL CICLO FOR RESTA 5 Y REPITE EL NUMERO DE LA FUNCION SUPERIOR
  // ASI SE DETERMINA LA POSICION DE FORMA AUTOMATICA
  double movimiento = 200;
  for (int i = 0; i < numero; i++)
  {
    movimiento = movimiento - 50;
    // Serial.println(movimiento);
  }
  stepper_X.setSpeedInStepsPerSecond(350);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(600);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(1000);
  stepper_X.setTargetPositionInSteps(-movimiento);
  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();
    buscar_botella(numero);
    Serial.println(count_iman);
  }
}

void buscar_botella(int botella)
{
  if (digitalRead(iman) == HIGH)
  {
    if (counter == true)
    {
      count_iman = count_iman + 1;

      counter = false;
    }
  }
  else
  {
    counter = true;
  }
}

void stopmotor(int target)
{
  Serial.println("target");
  if (target == count_iman)
  {
    delay(2000);
    stepper_X.emergencyStop();
    delay(2000);
    digitalWrite(MOTOR_X_ENABLE, HIGH);
    Serial.println("Botella encontrada");
    delay(3000);
    digitalWrite(MOTOR_X_ENABLE, LOW);
    // gohome();
  }
}
