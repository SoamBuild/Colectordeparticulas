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

  Serial.print("Read from file: ");
  while (file.available()) {
    char hola = file.read();
    myString = String(hola);
  }


  file.close();
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
