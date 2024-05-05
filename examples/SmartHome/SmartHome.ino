#include "SmartHome.h"


#define _SSID "" // WIFI ssid 
#define _PASSWORD "" // WIFI password
#define REFERENCE_URL "" // Database URL
#define BRANCH "test" // Your data branch

Firebase firebase(REFERENCE_URL);
SmartHome smartHome;

String dataOffline;
bool offlineData = false;

void setup() {
  Serial.begin(921600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  String data;
  if (WiFi.status() != WL_CONNECTED) {
    if (!offlineData) {
      offlineData = true;
      dataOffline = smartHome.LoadData();
    }
    smartHome.ProcessData(dataOffline);
    reconnectWiFi();
  } else {
    offlineData = false;
    data = firebase.getString(BRANCH);
    smartHome.ProcessData(data, firebase);
    dataOffline = data;
  }
}

void reconnectWiFi() {
  WiFi.begin(_SSID, _PASSWORD);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 5) {
    delay(500);
    Serial.print(".");
    attempt++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("Reconnected to Wi-Fi");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to reconnect to Wi-Fi");
  }
}
