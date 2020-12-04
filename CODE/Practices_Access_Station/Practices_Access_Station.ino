// thu vien
#include<ESP8266WiFi.h>

// khai bao bien
char* ssid_ap = "IOT_LVT";
char* pass_ap = "10100101";

char* ssid_sta = "My ASUS";
char* pass_sta = "00011101";

// cau hinh
IPAddress ip(192, 168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet( 255,255,255,0);

void setup()
{
  WiFi.disconnect();
  WiFi.mode( WIFI_AP_STA);
  //access point
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP( ssid_ap, pass_ap);
  // station mode
  WiFi.begin( ssid_sta, pass_sta);
  
  Serial.begin(9600);
  Serial.println(" ESP AccessPoint_station operating mode!");
  delay(500);
  Serial.println("Connecting...");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("....");
  }
  Serial.println(WiFi.localIP());

  pinMode( D4, OUTPUT);
}

void loop()
{
  digitalWrite( D4, HIGH);
  delay(1000);
  digitalWrite( D4, LOW);
  delay(500);
}
