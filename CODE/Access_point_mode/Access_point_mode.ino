// thu vien
#include<ESP8266WiFi.h>

// khai bao bien
char* ssid = "IOT";
char* pass = "10100101";

// cau hinh
IPAddress ip(192, 168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet( 255,255,255,0);

void setup()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP( ssid, pass);

  Serial.begin(9600);
  Serial.println(" ESP AccessPoint operating mode!");
}
