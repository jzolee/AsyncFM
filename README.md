# AsyncFM
simple web based interface for SPIFFS file management for esp32

### Accessing the web interface
go to `http://x.x.x.x/filemanager` in browser. ( where `x.x.x.x` is the IP of your ESP32 Module)

### Example
```
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncFM.h>

const char* ssid = "ssid";
const char* password = "password";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  AsyncFM.begin(&server);
  // AsyncFM.begin(&server, "username", "password"); // if you want security
  
  server.begin();
}

void loop() {
}
```
