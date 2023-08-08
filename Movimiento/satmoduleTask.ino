void checkSignal()
{
    while (signalQuality < 3)
    {
        err = modem.getSignalQuality(signalQuality);
        if (err != ISBD_SUCCESS)
        {
            Serial.print("SignalQuality failed: error ");
            Serial.println(err);
            return;
        }
        Serial.print("On a scale of 0 to 5, signal quality is currently ");
        Serial.print(signalQuality);
        Serial.println(".");
        delay(10000);
    }
}
void setmessage(String message)
{
    checkSignal();
    const char *localmessage = message.c_str();
    Serial.print("Trying to send the message.  This might take several minutes.\r\n");
    if (!messageSent)
    {
        err = modem.sendSBDText(localmessage);
        if (err != ISBD_SUCCESS)
        {
            Serial.print("sendSBDText failed: error ");
            Serial.println(err);
            if (err == ISBD_SENDRECEIVE_TIMEOUT)
                Serial.println("Try again with a better view of the sky.");
        }

        else
        {
            Serial.println("Hey, it worked!");
            messageSent = true;
        }
    }
}
void gettimeSat()
{
    checkSignal();
    struct tm t;
    err = modem.getSystemTime(t);
    if (err == ISBD_SUCCESS)
    {
        char buf[32];
        sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d",
                t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
        Serial.print("Iridium time/date is ");
        Serial.println(buf);
        setTime(t.tm_hour - 4, t.tm_min, t.tm_sec, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
    }
    else if (err == ISBD_NO_NETWORK)
    {
        Serial.println("No network detected.  Waiting 10 seconds.");
    }
}