#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
#include<ESP8266HTTPUpdateServer.h>

const char* ssid = "Ngoc Nu";
const char* password = "03041997";
const char* updatePath = "/update"; // thiet lap duong dan
const char* updateUsername = "admin"; // thiet lap user de truy cap
const char* updatePassword = "03041997"; // thiet lap pass word


// khai bao web server
ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdate;


void setup()
{
  Serial.begin(115200);
  Serial.println("dang ket noi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while( WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(100);
    Serial.println(".");
  }
  
  Serial.println(WiFi.localIP());

  httpUpdate.setup(&webServer, updatePath, updateUsername, updatePassword); // dua vao web 
  webServer.begin();
}

void loop()
{
  webServer.handleClient();
}
