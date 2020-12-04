#include<ESP8266WiFi.h>

char* ssid = "Ngoc Nu";
char* pass = "03041997";

void setup()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("....");
    
  }
  Serial.println(WiFi.localIP());
}


void loop()
{
  
}
