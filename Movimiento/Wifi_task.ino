void try_Connected()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
}
void try_Disconnected()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi OFF");
}
void tryOn_database()
{
    fbdo.setResponseSize(4096);
    config.token_status_callback = tokenStatusCallback;
    config.max_token_generation_retry = 5;
    Firebase.begin(&config, &auth);
    Serial.println("Getting User UID");
    while ((auth.token.uid) == "")
    {
        Serial.print('.');
        delay(1000);
    }
    uid = auth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.println(uid);
    databasePath = "/ColectorData/" + uid + "/Colector1";
}
void publish()
{

    parentPath = databasePath + "/" + String(epochTime);
    json.set(botella1.c_str(), String(statesbotellas[0]));
    json.set(botella2.c_str(), String(statesbotellas[1]));
    json.set(botella3.c_str(), String(statesbotellas[2]));
    json.set(botella4.c_str(), String(statesbotellas[3]));
    json.set(Bat.c_str(), String(voltaje));
    json.set(timePath, String(epochTime));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
}
void postData()
{
    try_Connected();
    tryOn_database();
    publish();
    try_Disconnected();
}