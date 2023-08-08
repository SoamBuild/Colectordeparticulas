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
        counthomeerror = 0;
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