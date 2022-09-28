void gohome()
{

  Serial.println("HOME BOTELLA ...");
  stepper_X.setSpeedInStepsPerSecond(90);
  stepper_X.setAccelerationInStepsPerSecondPerSecond(100);
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
void MOVE_FUNNEL(int distance, int botella)
{

  digitalWrite(ENABLE_F, LOW); // Active motor
  Serial.println("aaaaaaaaaaaaaaaa Bajando embudo en botella: " + String(botella));
  stepper_FUNNEL.setSpeedInStepsPerSecond(150);
  stepper_FUNNEL.setAccelerationInStepsPerSecondPerSecond(300);
  stepper_FUNNEL.setDecelerationInStepsPerSecondPerSecond(800);
  stepper_FUNNEL.moveRelativeInMillimeters(distance);
  while (!stepper_FUNNEL.motionComplete())
  {
    stepper_FUNNEL.processMovement();
  }
  delay(1000);
  digitalWrite(ENABLE_F, HIGH); // Active motor
}
void FILL_WATER(int state){
  digitalWrite(PUMP_WATER,state);
}
