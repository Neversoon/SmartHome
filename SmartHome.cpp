#include "SmartHome.h"

void SmartHome::ProcessData(String data, Firebase firebase) {
  if (data.length() == 0) {
    return;
  }

  int namesCount = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == '|') {
      namesCount++;
    }
  }

  String dataNames[namesCount + 1];

  int from = 0;
  int to;
  int nameIndex = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == '|') {
      to = i;
      String name = data.substring(from, to);
      dataNames[nameIndex] = name;
      from = i + 1;
      nameIndex++;
    }
  }

  dataNames[nameIndex] = data.substring(from);

  DeviceInfo deviceInfo[namesCount + 1];

  for (int i = 0; i < namesCount + 1; i++) {
    from = 0;
    to = 0;
    nameIndex = 0;
    for (int t = 0; t < dataNames[i].length(); t++) {
      if (dataNames[i][t] == '.') {
        switch (nameIndex) {
          case 0:
            to = t;
            deviceInfo[i].name = dataNames[i].substring(from, to);
            from = to + 1;
            nameIndex++;
            break;
          case 1:
            to = t;
            deviceInfo[i].type = dataNames[i].substring(from, to);
            from = to + 1;
            nameIndex++;
            break;
          case 2:
            to = t;
            deviceInfo[i].pin = dataNames[i].substring(from, to).toInt();
            from = to + 1;
            nameIndex++;
            break;
          case 3:
            to = t;
            deviceInfo[i].params = dataNames[i].substring(from, to);
            from = to + 1;
            nameIndex++;
            break;
          default:
            break;
        }
      }
    }
    deviceInfo[i].params = dataNames[i].substring(from);

    Serial.println(deviceInfo[i].name + " " + deviceInfo[i].type + " " + String(deviceInfo[i].pin) + " " + deviceInfo[i].params);
  }
  ActionsByDevice(namesCount + 1, deviceInfo, firebase);
}

void SmartHome::ProcessData(String data) {
  if (data.length() == 0) {
    return;
  }

  int namesCount = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == '|') {
      namesCount++;
    }
  }

  String dataNames[namesCount + 1];

  int from = 0;
  int to;
  int nameIndex = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == '|') {
      to = i;
      String name = data.substring(from, to);
      dataNames[nameIndex] = name;
      from = i + 1;
      nameIndex++;
    }
  }

  dataNames[nameIndex] = data.substring(from);

  DeviceInfo deviceInfo[namesCount + 1];

  for (int i = 0; i < namesCount + 1; i++) {
    from = 0;
    to = 0;
    nameIndex = 0;
    for (int t = 0; t < dataNames[i].length(); t++) {
      if (dataNames[i][t] == '.') {
        switch (nameIndex) {
          case 0:
            to = t;
            deviceInfo[i].name = dataNames[i].substring(from, to);
            from = to + 1;
            nameIndex++;
            break;
          case 1:
            to = t;
            deviceInfo[i].type = dataNames[i].substring(from, to);
            from = to + 1;
            nameIndex++;
            break;
          case 2:
            to = t;
            deviceInfo[i].pin = dataNames[i].substring(from, to).toInt();
            from = to + 1;
            nameIndex++;
            break;
          case 3:
            to = t;
            deviceInfo[i].params = dataNames[i].substring(from, to);
            from = to + 1;
            nameIndex++;
            break;
          default:
            break;
        }
      }
    }
    deviceInfo[i].params = dataNames[i].substring(from);

    Serial.println(deviceInfo[i].name + " " + deviceInfo[i].type + " " + String(deviceInfo[i].pin) + " " + deviceInfo[i].params);
  }
  ActionsByDevice(namesCount + 1, deviceInfo);
}

void SmartHome::ActionsByDevice(int deviceCount, DeviceInfo devicesInfo[]) {
  String updatedData = "";
  bool firstDevice = true;

  for (int i = 0; i < deviceCount; i++) {
    DeviceInfo deviceInfo = devicesInfo[i];

    int paramsCount = 1;
    for (int j = 0; j < deviceInfo.params.length(); j++) {
      if (deviceInfo.params[j] == ',') {
        paramsCount++;
      }
    }

    String params[paramsCount];
    Serial.println(paramsCount);
    int from = 0;
    int to;
    int paramIndex = 0;

    for (int j = 0; j < deviceInfo.params.length(); j++) {
      if (deviceInfo.params[j] == ',') {
        to = j;
        String param = deviceInfo.params.substring(from, to);
        params[paramIndex] = param;
        from = j + 1;
        paramIndex++;
      }
    }
    if (paramIndex != 0) {
      params[paramIndex] = deviceInfo.params.substring(from, deviceInfo.params.length());
    }
    bool activeStatus;

    if (paramIndex == 0) {
      activeStatus = deviceInfo.params == "true";
    } else {
      activeStatus = params[0] == "true";
    }

    if (deviceInfo.type == "Relay") {
      Serial.println("It is relay ");
      if (digitalRead(deviceInfo.pin) != activeStatus) {
        pinMode(deviceInfo.pin, 1);
        Serial.println("On/off " + String(activeStatus));
        digitalWrite(deviceInfo.pin, activeStatus ? HIGH : LOW);
      }
      String relayType = params[1];
    } else if (deviceInfo.type == "Sensor") {
      int sensorValue;
      if (params[1] == "analog") {
        sensorValue = analogRead(deviceInfo.pin);
      } else {
        sensorValue = digitalRead(deviceInfo.pin);
      }
    } else if (deviceInfo.type == "Led") {
      Serial.println("It is led ");
      if (digitalRead(deviceInfo.pin) != activeStatus) {
        Serial.println("On/off " + String(activeStatus));
        digitalWrite(deviceInfo.pin, activeStatus ? HIGH : LOW);
      }
    }
  }
}

void SmartHome::ActionsByDevice(int deviceCount, DeviceInfo devicesInfo[], Firebase firebase) {
  String updatedData = "";
  bool firstDevice = true;

  for (int i = 0; i < deviceCount; i++) {
    DeviceInfo deviceInfo = devicesInfo[i];

    int paramsCount = 1;
    for (int j = 0; j < deviceInfo.params.length(); j++) {
      if (deviceInfo.params[j] == ',') {
        paramsCount++;
      }
    }

    String params[paramsCount];
    Serial.println(paramsCount);
    int from = 0;
    int to;
    int paramIndex = 0;

    for (int j = 0; j < deviceInfo.params.length(); j++) {
      if (deviceInfo.params[j] == ',') {
        to = j;
        String param = deviceInfo.params.substring(from, to);
        params[paramIndex] = param;
        from = j + 1;
        paramIndex++;
      }
    }
    if (paramIndex != 0) {
      params[paramIndex] = deviceInfo.params.substring(from, deviceInfo.params.length());
    }
    bool activeStatus;

    if (paramIndex == 0) {
      activeStatus = deviceInfo.params == "true";
    } else {
      activeStatus = params[0] == "true";
    }

    Serial.println("Active status " + String(activeStatus));

    String updatedParams;
    String activeStatusStr = activeStatus ? "true" : "false";

    if (deviceInfo.type == "Relay") {
      Serial.println("It is relay ");
      if (digitalRead(deviceInfo.pin) != activeStatus) {
        pinMode(deviceInfo.pin, 1);
        Serial.println("On/off " + String(activeStatus));
        digitalWrite(deviceInfo.pin, activeStatus ? HIGH : LOW);
      }
      String relayType = params[1];
      updatedParams = activeStatusStr + "," + relayType;
    } else if (deviceInfo.type == "Sensor") {
      int sensorValue;
      if (params[1] == "analog") {
        sensorValue = analogRead(deviceInfo.pin);
      } else {
        sensorValue = digitalRead(deviceInfo.pin);
      }
      updatedParams = String(sensorValue) + "," + params[1];
    } else if (deviceInfo.type == "Led") {
      Serial.println("It is led ");
      if (digitalRead(deviceInfo.pin) != activeStatus) {
        Serial.println("On/off " + String(activeStatus));
        pinMode(deviceInfo.pin, 1);
        digitalWrite(deviceInfo.pin, activeStatus ? HIGH : LOW);
      }
      updatedParams = activeStatusStr;
    }

    if (!firstDevice) {
      updatedData += "|";
    }

    updatedData += deviceInfo.name + "." + deviceInfo.type + "." + deviceInfo.pin + "." + updatedParams;
    firstDevice = false;
  }

  Serial.println("New data: " + updatedData);

  if (firebase.setString("test", updatedData)) {
    Serial.println("Data successfully updated in Firebase.");
  } else {
    Serial.println("Error updating data in Firebase.");
  }
}
void SmartHome::SaveData(String data) {
  int length = data.length();

  for (int i = 0; i < length; ++i) {
    EEPROM.write(i, data[i]);
  }

  EEPROM.commit();
}

String SmartHome::LoadData() {
  String data;
  char charRead;
  int addr = 0;

  while (true) {
    charRead = EEPROM.read(addr++);
    if (charRead == '\0' || addr >= EEPROM.length()) {
      break;
    }
    data += charRead;
  }

  return data;
}
