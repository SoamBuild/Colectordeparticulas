#include <ESP_FlexyStepper.h>

bool count_active_iman = true;
int count_iman = 0;

const int iman = 12;                  // pin para detector
const int MOTOR_X_ENABLE = 18;        // apagar y prender motor
const int MOTOR_X_STEP_PIN = 16;      // step pin para motor
const int MOTOR_X_DIRECTION_PIN = 17; // direccion para motor
const int LIMIT_X_SWITCH_PIN = 13;    // final de carrera motor
ESP_FlexyStepper stepper_X;           // obj motor

void setup()
{
  Serial.begin(115200); // serial begin
  // pinMode para iman, motor y final de carrera
  pinMode(iman, INPUT_PULLUP);
  pinMode(MOTOR_X_ENABLE, OUTPUT);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);

  // estado inicial de motor
  digitalWrite(MOTOR_X_ENABLE, LOW);

  // GO HOME
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
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
}
void sbotella(int numero_botella)
{
  // setup vel,aceleracion y desaceleracion
  stepper_X.setSpeedInStepsPerSecond(80);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(100);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(400);
  // setup distancia segun target
  if (numero_botella >= 3)
  {
    stepper_X.setTargetPositionInSteps(-200);
  }
  else
  {
    stepper_X.setTargetPositionInSteps(-100);
  }
  // procesar movimiento
  while (!stepper_X.motionComplete())
  {
    stepper_X.processMovement();    // process
    buscar_botella(numero_botella); // buscar botella con iman
    stopmotor(numero_botella);      // detener motor segun iman
    Serial.println(count_iman);     // imprimir contador de imanes
  }
  // si el movimiento se completo
  if (stepper_X.motionComplete())
  {
    count_iman = 0; // reiniciar contador de imanes
    gohome();       // volver a inicio
  }
}
// ir a inicio
void gohome()
{
  // setup vel, aceleracione, desaceleracion
  stepper_X.setSpeedInStepsPerSecond(30);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(50);
  stepper_X.setDecelerationInStepsPerSecondPerSecond(50);
  // gohome
  if (stepper_X.moveToHomeInSteps(-1, 5, 200, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOME OK");
  }
  else
  {
    Serial.println("HOME ERROR");
  }
}
// detectar botellas con iman
void buscar_botella(int botella)
{
  // si el iman esta
  if (digitalRead(iman) == HIGH)
  {
    if (count_active_iman == true) // y el contador esta activo
    {
      count_iman = count_iman + 1; // el contador mas 1

      count_active_iman = false; // desactivar contador
    }
  }
  else
  {
    count_active_iman = true; // si no hay iman se reactiva el iman
  }
}

// detener motor
void stopmotor(int target)
{
  // alcanzar objetivo
  if (target == count_iman)
  {
    delay(2000);
    stepper_X.emergencyStop(); // detener motor
    delay(2000);
    digitalWrite(MOTOR_X_ENABLE, HIGH); // apagar motor
    Serial.println("Botella encontrada");
    delay(3000);
    digitalWrite(MOTOR_X_ENABLE, LOW); // activar motor para volver al inicio
    
  }
}
