#include <NTPClient.h>
#include<ESP8266WiFi.h>
#include<WiFiUdp.h>


WiFiUDP u;
NTPClient n(u, "2.asia.pool.ntp.org", 7*3600);

int h, mins;

void setup()
{
  Serial.begin(115200);
  
  WiFi.disconnect();
  WiFi.begin("Ngoc Nu", "03041997");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("da vao duoc wifi");

  n.begin();
}

void loop()
{
  n.update();
  delay(1000);
  h = n.getHours();
  mins = n.getMinutes();
  Serial.print(h);
  Serial.print(mins);
  Serial.println();
  Serial.println(n.getFormattedTime());
}
