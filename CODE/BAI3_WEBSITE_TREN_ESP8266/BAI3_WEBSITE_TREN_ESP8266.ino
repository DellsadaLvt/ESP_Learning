#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer webServer(80);
char* ssid = "My ASUS";
char* pass = "00011101";
//=========Biến chứa mã HTLM Website==//
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html> 
  <html>
   <head> 
       <title>HOME PAGE</title> 
       <style> 
          body {text-align:center;}
          h1 {color:#003399;}
          a {text-decoration: none;color:#FFFFFF;}
          .bt_on {height:50px; width:100px; margin:10px 0;background-color:#FF6600;border-radius:5px;}
          .bt_off {height:50px; width:100px; margin:10px 0;background-color:#00FF00;border-radius:5px;}
       </style>
       <meta charset="UTF-8">
   </head>
   <body> 
      <h1>ESP8266 Web Server</h1> 
      <div>Trạng thái trên chân D1: <b>OFF</b></div>
      <div>
        <button class="bt_on"><a href="/onD1">ON</a></button>
        <button class="bt_off"><a href="/offD1">OFF</a></button>
      </div>
      <div>Trạng thái trên chân D4: <b>OFF</b></div>
      <div>
        <button class="bt_on"><a href="/onD4">ON</a></button>
        <button class="bt_off"><a href="/offD4">OFF</a></button>
      </div>
   </body> 
  </html>
)=====";
//=========================================//
void setup() {
  pinMode(D1,OUTPUT);
  pinMode(D4,OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D4,HIGH);
  WiFi.begin(ssid,pass);
  Serial.begin(9600);
  Serial.print("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.println(WiFi.localIP());

  webServer.on("/",mainpage);
  webServer.on("/onD1",on_D1);
  webServer.on("/offD1",off_D1);
  webServer.on("/onD4",on_D4);
  webServer.on("/offD4",off_D4);
  webServer.begin();
}
void loop() {
  webServer.handleClient();
}
//==========Chương trình con=================//
void mainpage(){
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void on_D1(){
  digitalWrite(D1,LOW);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void off_D1(){
  digitalWrite(D1,HIGH);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void on_D4(){
  digitalWrite(D4,LOW);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
void off_D4(){
  digitalWrite(D4,HIGH);
  String s = FPSTR(MainPage);
  webServer.send(200,"text/html",s);
}
