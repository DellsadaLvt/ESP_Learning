// khai bao thu vien
#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

ESP8266WebServer WebServer(80);
// cai dat ssid va pass
char* ssid = "IOT_ESP";
char* pass = "10100101";
// cau hinh wifi
IPAddress ip( 192, 168 , 1, 1);
IPAddress gateway( 192,168,1,1);
IPAddress subnet( 255,255,255,0);

/// html ////
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <title> HOME PAGE </title>
    <style>
      body {text-align:center;}
      h1 {color:#003399;}
      a {text-decoration: none;color:#FFFFFF;}
      .bt_on {height: 50px; width: 100px; margin:10px 0; background-color:#FF6600; border-radius: 5px;}
      .bt_off {height: 50px; width: 100px; margin:10px 0; background-color:#00FF00; border-radius: 5px;}
    </style>
    <meta charset = "UTF-8">
    </head>
    <body>
      <h1> ESP8266 Web Server </h1>
      <div> Trang thai chan D4: <b><pan id = "trangthaiD4"><pan></b></div>
      <div>
        <button class="bt_on" onclick="getdata('onD4')">ON</button>
        <button class="bt_off" onclick="getdata('offD4')">OFF</button>
      </div>
      <div id="reponsetext"></div>
      <script>
      //----------------Tao doi tuong request-----------------------//
      function create_obj()
      {
        td = navigator.appName;
        if ( td == "Microsoft Internet Explorer")
        {
          obj = new ActiveXObject("Microsoft.XMLHTTP");
        }
        else
        {
          obj = new XMLHttpRequest();
        }
        return obj;
      }
      var xhttp = create_obj();
      //----------------------thiet lap du lieu va goi request-------------//
      function getdata(url)
      {
        xhttp.open("GET","/"+url, true);
        xhttp.onreadystatechange = process; // nhan response
        xhttp.send();
      }
      //----------Kiem tra response-------------------//
      function process()
      {
        if(xhttp.readyState == 4 && xhttp.status == 200)
        {
          //--------update data su dung javascript------------//
          ketqua = xhttp.responseText;
          document.getElementById("responsetext").innerHTML=ketqua;
        }
      }
      //------------ Kiem tra trng thai chan D4-------------------//
      function getstatusD4()
      {
        xhttp.open("GET", "getstatusD4", true);
        xhttp.onreadystatechange = process_json;
        xhttp.send();
      }
      //--------------Kiem tra response-------------//
      function process_json()
      {
        if(xhttp.readyState == 4 && xhttp.status == 200)
        {
          //--------------update data su dung javascript---------------//
          var trangthaiD4_JSON = xhttp.responseText;
          var Obj = JSON.parse(trangthaiD4_JSON);
          document.getElementById("trangthaiD4").innerHTML = Obj.D4;
        }
      }
      //--------------Ham update data tu dong----------------//
      setInterval(function()
      {
        getstatusD4();
      }, 1000);
      </script>
    </body>
    </html>
    )=====";
    //======================================================//
    

void setup()
{
 // setup WiFi
 WiFi.disconnect();
 WiFi.mode( WIFI_AP);
 ////
 WiFi.softAPConfig( ip, gateway, subnet);
 WiFi.softAP( ssid, pass); 
 // set pin
 pinMode( D4, OUTPUT);
 //tt ban dau
 pinMode( D4, HIGH);
 // set webServer
 WebServer.on("/", mainpage);
 WebServer.on("/onD4", on_D4);
 WebServer.on("/offD4", off_D4);
 WebServer.on("/getstatusD4", get_statusD4);
 WebServer.begin();
}

void loop()
{
  WebServer.handleClient();
}


//================= Chuong trinh con===========//
void mainpage()
{
  WebServer.send(200, "text/html", FPSTR(MainPage));
}

void on_D4()
{
  digitalWrite( D4, LOW);
  WebServer.send( 200, "text/html", " Chan D4 ON");
}

void off_D4()
{
  digitalWrite(D4, HIGH);
  WebServer.send( 200, "text/html", " Chan D4 OFF");
}

void get_statusD4()
{
  String d4;
  if( digitalRead(D4) ==1)
    d4 = "OFF";
  else
    d4 = "ON";


 String s = "{\"D4\": \""+ d4 +"\"}";
 WebServer.send(200, "application/json",s);
}
