#pragma once

#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

struct DeviceInfo {
  String name;
  String type;
  uint8_t pin;
  String params;
};

class SmartHome {
public:
  void ProcessData(String data, Firebase firebase);
  void ProcessData(String data);
  String LoadData();
private:
  void ActionsByDevice(int deviceCount, DeviceInfo devicesInfo[], Firebase firebase);
  void ActionsByDevice(int deviceCount, DeviceInfo devicesInfo[]);
  void SaveData(String data);
};