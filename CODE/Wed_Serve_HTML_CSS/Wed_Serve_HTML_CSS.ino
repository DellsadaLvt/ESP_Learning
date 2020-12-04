#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>  

ESP8266WebServer webServer(80);

// khai bao ten wifi va pass
char* ssid = "IOT_Smart";
char* pass = "10100101";
// ma nguon html
const char MainPage[] PROGMEM = R"====={
  <!DOCTYPE html>
    <html>
      <head>
        <title>HOME PAGE</title>
        <style>
          body {text-align:center;}
          h1 {color:#003399;}
          a {text-decoration: none;color:#FFFFFF;}
          .bt_on { height:50px; width:100px; margin: 10px 0;background-color:#FF6600;border-radius:5px;}
          .bt_off { height:50px; width:100px; margin: 10px 0;background-color:#00FF00;border-radius:5px;}
          </style>
          <meta charset="UTF-8">
      </head>
      <body>
        <h1>ESP8266 Wed Server</h1>
        <div>Trang thai tren chan D4: <b>OFF</b></div>
        <div>
          <button class="bt_on"><a href="/onD4">ON</a></button>
          <button class="bt_off"><a href="/offD4">OFF</a></button>
        </div>
      </body>
   </html>  
}=====";

// cau hinh 
IPAddress ip( 192, 168, 1,1);
IPAddress gateway( 192,168,1,1);
IPAddress subnet (255,255,255,0);

void setup()
{
  // KHAI BAO IN/OUT
  pinMode(D4, OUTPUT);
  ////
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig( ip, gateway, subnet);
  WiFi.softAP( ssid, pass);
  ///
  webServer.on("/", mainpage);
  webServer.on("/onD4", on_D4);
  webServer.on("/offD4", off_D4);
  wedServe.begin();
}

void loop()
{
  webServer.handleClient();
}

void mainpage();
void on_D4()
{
  digitalWrite(D4, HIGH);
  String s = MainPage;
  webServer.send( 200, "text/html", s);
}
void off_D4()
{
  digitalWrite(D4, LOW);
  String s = MainPage;
  webServer.send( 200, "text/html", s);
}
