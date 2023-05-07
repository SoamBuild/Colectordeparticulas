void getTime()
{
    epochTime = gettimeUnix();
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    // time = timeinfo, "%A, %B %d %Y %H:%M:%S";

    day = timeinfo.tm_mday;
    hour = timeinfo.tm_hour;
    minute = timeinfo.tm_min;
    second = timeinfo.tm_sec;
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
unsigned long gettimeUnix()
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        // Serial.println("Failed to obtain time");
        return (0);
    }
    time(&now);
    return now;
}
