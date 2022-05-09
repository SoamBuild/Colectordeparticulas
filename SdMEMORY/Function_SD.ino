void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
void readFile(fs::FS &fs, const char * path) {
  int i = 0;
  //char str[1];
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Read from file: ");
  while (file.available()) {
    myString = file.readStringUntil('\n');

  }

  id = getValor(myString, ',', 0).toInt();
  timed = getValor(myString, ',', 1);
  stage = getValor(myString, ',', 2).toInt();
  b1 = getValor(myString, ',', 3).toInt();
  b2 = getValor(myString, ',', 4).toInt();
  b3 = getValor(myString, ',', 5).toInt();
  b4 = getValor(myString, ',', 6).toInt();

  Serial.println(id);
  Serial.println(timed);
  Serial.println(stage);
  Serial.println(b1);
  Serial.println(b2);
  Serial.println(b3);
  Serial.println(b4);
  
  file.close();
}

String getValor(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/*
  char str[] = "uno;dos;tres;cuatro;";
  char delimitadores[] = ";";
  char *resultado = NULL;
  resultado = strtok( str, delimitadores );


  /*
  void fixdata(int tag, String values, int ndatos)
  {
  int data[ndatos];

  for (int i = 0; i < ndatos; i++)
  {
    int index = values.indexOf(separator);
    data[i] = values.substring(0, index).toInt();
    values = values.substring(index + 1);
    Serial.println(data[i]);
  }
  /*
    void readFile(fs::FS &fs, const char * path) {
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file) {
      Serial.println("Failed to open file for reading");
      return;
    }

    Serial.print("Read from file: ");
    while (file.available()) {
      Serial.write(file.read());
      /*
        char hola = file.read();
        String myString = String(hola);
        char Buf[6];

        myString.toCharArray(Buf, 6);
        Serial.print(myString.length());

    }
    file.close();
    }
*/
