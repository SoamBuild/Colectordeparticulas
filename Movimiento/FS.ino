void readFile(fs::FS &fs, const char *path, int id)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        statesbotellas[id] = 0;
        return;
    }

    // Serial.print("Read from file: ");
    while (file.available())
    {
        myString = file.readStringUntil('\n');
        statesbotellas[id] = getValor(myString, ',', 0).toInt();
    }

    // id = getValor(myString, ',', 0).toInt();

    // Serial.println(id);
    // file.close();
}
String getValor(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

// sessionID,time,pirID,batteryValue
void ROUTINE_REGISTRY(int idPir)
{

    // Battery_check(0);
    // now = rtc.now();
    // Serial.println("Guardando alerta PIR: " + String(idPir));
    // String tosave = String(id) + "," + String(now.hour(), DEC) + ":" + String(now.minute(), DEC) + ":" + String(now.second(), DEC) + "," + String(idPir) + "," + String(voltage) + "\n";
    // appendFile(SD, "/ActivePuma_DB.txt", tosave.c_str());
    // writeFile(SD, "/ActivePuma.txt", tosave.c_str());
    // sdregistry = true;
}