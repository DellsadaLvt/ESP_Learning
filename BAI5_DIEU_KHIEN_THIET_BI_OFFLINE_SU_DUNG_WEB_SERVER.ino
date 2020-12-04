#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
ESP8266WebServer webServer(80);
//==========AP info=======================//
char* ssid_ap = "E-SMART HOME";
char* pass_ap = "12345678";
IPAddress ip_ap(192,168,1,1);
IPAddress gateway_ap(192,168,1,1);
IPAddress subnet_ap(255,255,255,0);
String statusD1="1",statusD2="1",statusD3="1",statusD4="1";
String ssid;
String pass;
//=========Biến chứa mã HTLM Website==//
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html> 
  <html>
   <head> 
       <title>E-SMART HOME</title> 
       <style> 
          body {text-align:center;background-color:#222222; color:white}
          input {
            height:25px; 
            width:270px;
            font-size:20px;
            margin: 10px auto;
          }
          #content {
            border: white solid 1px; 
            padding:5px;  
            height:375px; 
            width:330px; 
            border-radius:20px;
            margin: 0 auto
          }
          .ledstatus{
              outline: none;
              margin: 15px 5px;
              width: 60px;
              height: 60px;
              border-radius: 50%;
              -moz-border-radius: 50%;
              -webkit-border-radius: 50%;
              color:white;
              font-size:20px;
              font-weight: bold;
          }
          #ledstatus1{
              border: solid 2px #23C48E;
              background: #23C48E;
          }
          #ledstatus2{
              border: solid 2px #ED9D00;
              background: #ED9D00;
          }
          #ledstatus3{
              border: solid 2px #5F7CD8;
              background: #5F7CD8;
          }
          #ledstatus4{
              border: solid 2px #03C0F8;
              background: #03C0F8;
          }
          #ledconnect{
              outline: none;
              margin: 0px 5px -1px 5px;
              width: 15px;
              height: 15px;
              border: solid 1px #00EE00;
              background-color: #00EE00;
              border-radius: 50%;
              -moz-border-radius: 50%;
              -webkit-border-radius: 50%;
          }
          .button {
            height:65px; 
            width:70px; 
            margin:10px 0;
            background-color:#3C89BC;
            border-radius:10px;
            outline:none;
            color:white;
            font-size:25px;
            font-weight: bold;
          }
          .button_all{
            height:70px; 
            width:140px; 
            margin:10px 0;
            outline:none;
            color:white;
            font-size:20px;
            font-weight: bold;
          }
          #button_on {
            background-color:#00BB00;
            border-radius:10px;
          }
          #button_off {
            background-color:#CC3300;
            border-radius:10px;
          }
          .button_setup {
            height:30px; 
            width:280px; 
            margin:10px 0;
            background-color:#3C89BC;
            border-radius:10px;
            outline:none;
            color:white;
            font-size:20px;
            font-weight: bold;
          }
          .button_wifi{
            height:50px; 
            width:90px; 
            margin:10px 0;
            outline:none;
            color:white;
            font-size:15px;
            font-weight: bold;
          }
          #button_save {
            background-color:#00BB00;
            border-radius:5px;
          }
          #button_restart {
            background-color:#FF9900;
            border-radius:5px;
          }
          #button_reset {
            background-color:#CC3300;
            border-radius:5px;
          }
       </style>
       <meta name="viewport" content="width=device-width,user-scalable=0" charset="UTF-8">
   </head>
   <body> 
      <div><h1>E-SMART HOME</h1></div>
      <div id="content"> 
        <div id="homecontrol" style="height:340px";>
          <div>
            <input id="ledstatus1" class="ledstatus" type="button" value="ON"/>
            <input id="ledstatus2" class="ledstatus" type="button" value="ON"/>
            <input id="ledstatus3" class="ledstatus" type="button" value="ON"/>
            <input id="ledstatus4" class="ledstatus" type="button" value="ON"/>
          </div>
           <div>
            <button class="button" onclick=getbutton(1)>1</button>
            <button class="button" onclick=getbutton(2)>2</button>
            <button class="button" onclick=getbutton(3)>3</button>
            <button class="button" onclick=getbutton(4)>4</button>
          </div>
          <div>
            <button id="button_on" class="button_all" onclick="getbuttonall('on')">Turn on all</button>
            <button id="button_off" class="button_all"onclick="getbuttonall('off')">Turn off all</button>
          </div>
          <div><input class="button_setup" type="button" onclick="configurewifi()" value="Configure WiFi"/></div>
          <div>IP connected: <b><pan id="ipconnected"></pan></b></div>
        </div>
        
        <div id="wifisetup" style="height:340px; font-size:20px; display:none";>
          <div style="text-align:left; width:270px; margin:5px 25px">SSID: </div>
          <div><input id="ssid"/></div>
          <div style="text-align:left; width:270px; margin:5px 25px">Password: </div>
          <div><input id="pass"/></div>
          <div>
            <button id="button_save" class="button_wifi" onclick="writeEEPROM()">SAVE</button>
            <button id="button_restart" class="button_wifi" onclick="restartESP()">RESTART</button>
            <button id="button_reset" class="button_wifi" onclick="clearEEPROM()">RESET</button>
          </div>
          <div><input class="button_setup" type="button" onclick="backHOME()" value="Back home"/></div>
          <div id="reponsetext"></div>
        </div>
        <div><input id="ledconnect" type="button"/>Connection status</div>
      </div>
      <div id="footer">
        <p><i>Copyright by <b>Điện thông minh E-SMART</b> 
          <br>Email: <b>dienthongminhesmart@gmail.com</b></i>
        </p>
      </div>
      <script>
        //-----------Hàm khởi tạo đối tượng request----------------
        function create_obj(){
          td = navigator.appName;
          if(td == "Microsoft Internet Explorer"){
            obj = new ActiveXObject("Microsoft.XMLHTTP");
          }else{
            obj = new XMLHttpRequest();
          }
          return obj;
        }
        //------------Khởi tạo biến toàn cục-----------------------------
        var xhttp = create_obj();//Đối tượng request cho setup wifi
        var xhttp_statusD = create_obj();//Đối tượng request cho cập nhật trạng thái
        var d1,d2,d3,d4;
        var ledconnect = 1;
        //===================Khởi tạo ban đầu khi load trang=============
        window.onload = function(){
          document.getElementById("homecontrol").style.display = "block";
          document.getElementById("wifisetup").style.display = "none";
          getIPconnect();//Gửi yêu cầu lấy IP kết nối
          getstatusD();//Gửi yêu cầu lấy trạng thái các chân điều khiển
        }
        //===================IPconnect====================================
        //--------Tạo request lấy địa chỉ IP kết nối----------------------
        function getIPconnect(){
          xhttp.open("GET","/getIP",true);
          xhttp.onreadystatechange = process_ip;//nhận reponse 
          xhttp.send();
        }
        //-----------Kiểm tra response IP và hiển thị------------------
        function process_ip(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = xhttp.responseText; 
            document.getElementById("ipconnected").innerHTML=ketqua;       
          }
        }
        //===========Cập nhật trạng thái Led===============================
        //-----------Gửi yêu cầu lấy trạng thái các chân D-----------------
        function getstatusD(){
          xhttp_statusD.open("GET","/getSTATUSD",true);
          xhttp_statusD.onreadystatechange = process_statusD;
          xhttp_statusD.send();
        }
        //-----------Kiểm tra và nhận response EEPROM----------------------
        function process_statusD(){
          if(xhttp_statusD.readyState == 4 && xhttp_statusD.status == 200){
            if(ledconnect == 1){
              ledconnect = 0;
              document.getElementById("ledconnect").style.background = "#222222";
            }else{
              ledconnect = 1;
              document.getElementById("ledconnect").style.background = "#00EE00";
            }
            //------Update trạng thái led tín hiệu lên panel điều khiển----
            var statusD = xhttp_statusD.responseText;
            var obj = JSON.parse(statusD);
            d1 = obj.D1;
            d2 = obj.D2;
            d3 = obj.D3;
            d4 = obj.D4;
            updateLedstatus(d1,d2,d3,d4);
          }
        }
        //----------Hiển thị trạng thái Led lên trình duyệt---------------------
        function updateLedstatus(D1,D2,D3,D4){
          if(D1 == "1"){
              document.getElementById("ledstatus1").value = "OFF";
              document.getElementById("ledstatus1").style.background = "#222222";
            }else{
              document.getElementById("ledstatus1").value = "ON";
              document.getElementById("ledstatus1").style.background = "#23C48E";
            }
            if(D2 == "1"){
              document.getElementById("ledstatus2").value = "OFF";
              document.getElementById("ledstatus2").style.background = "#222222";
            }else{
              document.getElementById("ledstatus2").value = "ON";
              document.getElementById("ledstatus2").style.background = "#ED9D00";
            }
            if(D3 == "1"){
              document.getElementById("ledstatus3").value = "OFF";
              document.getElementById("ledstatus3").style.background = "#222222";
            }else{
              document.getElementById("ledstatus3").value = "ON";
              document.getElementById("ledstatus3").style.background = "#5F7CD8";
            }
            if(D4 == "1"){
              document.getElementById("ledstatus4").value = "OFF";
              document.getElementById("ledstatus4").style.background = "#222222";
            }else{
              document.getElementById("ledstatus4").value = "ON";
              document.getElementById("ledstatus4").style.background = "#03C0F8";
            }
        }
        //===========Tạo và gửi request khi ấn button============================
        //-----------Thiết lập dữ liệu và gửi request ON/OFF D4---
        function getbutton(n){
          switch (n){
            case 1:
                var ledstatus1 = document.getElementById("ledstatus1").value;
                if(ledstatus1 == "ON"){
                  xhttp_statusD.open("GET","/D1off",true);
                }else{
                  xhttp_statusD.open("GET","/D1on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
            case 2:
                var ledstatus2 = document.getElementById("ledstatus2").value;
                if(ledstatus2 == "ON"){
                  xhttp_statusD.open("GET","/D2off",true);
                }else{
                  xhttp_statusD.open("GET","/D2on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
            case 3:
                var ledstatus3 = document.getElementById("ledstatus3").value;
                if(ledstatus3 == "ON"){
                  xhttp_statusD.open("GET","/D3off",true);
                }else{
                  xhttp_statusD.open("GET","/D3on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
            case 4:
               var ledstatus4 = document.getElementById("ledstatus4").value;
                if(ledstatus4 == "ON"){
                  xhttp_statusD.open("GET","/D4off",true);
                }else{
                  xhttp_statusD.open("GET","/D4on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
          }
        }
        function getbuttonall(m){
          if(m == "on"){
            xhttp_statusD.open("GET","/Allon",true);
          }else if(m=="off"){
            xhttp_statusD.open("GET","/Alloff",true);
          }
          xhttp_statusD.onreadystatechange = process_statusD;
          xhttp_statusD.send();
        }
        //===========Configure WiFi=====================================
        function configurewifi(){
          document.getElementById("homecontrol").style.display = "none";
          document.getElementById("wifisetup").style.display = "block";
        }
        //-----------Thiết lập dữ liệu và gửi request ssid và password---
        function writeEEPROM(){
          if(Empty(document.getElementById("ssid"), "Please enter ssid!")&&Empty(document.getElementById("pass"), "Please enter password")){
            var ssid = document.getElementById("ssid").value;
            var pass = document.getElementById("pass").value;
            xhttp.open("GET","/writeEEPROM?ssid="+ssid+"&pass="+pass,true);
            xhttp.onreadystatechange = process;//nhận reponse 
            xhttp.send();
          }
        }
        function clearEEPROM(){
          if(confirm("Do you want to delete all saved wifi configurations?")){
            xhttp.open("GET","/clearEEPROM",true);
            xhttp.onreadystatechange = process;//nhận reponse 
            xhttp.send();
          }
        }
        function restartESP(){
          if(confirm("Do you want to reboot the device?")){
            xhttp.open("GET","/restartESP",true);
            xhttp.send();
            alert("Device is restarting! If no wifi is found please press reset!");
          }
        }
        //-----------Kiểm tra response -------------------------------------------
        function process(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = xhttp.responseText; 
            document.getElementById("reponsetext").innerHTML=ketqua;       
          }
        }
       //============Hàm thực hiện chứ năng khác================================
       //--------Cập nhật trạng thái tự động sau 2000 giây----------------------
        setInterval(function() {
          getstatusD();
        },2000);
       //--------Load lại trang để quay về Home control-------------------------
        function backHOME(){
          window.onload();
        }
       //----------------------------CHECK EMPTY--------------------------------
       function Empty(element, AlertMessage){
          if(element.value.trim()== ""){
            alert(AlertMessage);
            element.focus();
            return false;
          }else{
            return true;
          }
       }
       //------------------------------------------------------------------------
      </script>
   </body> 
  </html>
)=====";
//=========================================//
void setup() {
  Serial.begin(9600);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  
  EEPROM.begin(512);       //Khởi tạo bộ nhớ EEPROM
  delay(10);
  
  if(read_EEPROM()){
    checkConnection();
  }else{
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
    WiFi.softAP(ssid_ap,pass_ap,1,false);
    Serial.println("Soft Access Point mode!");
    Serial.print("Please connect to");
    Serial.println(ssid_ap);
    Serial.print("Web Server IP Address: ");
    Serial.println(ip_ap);
  }
  webServer.on("/",mainpage);
  webServer.on("/getSTATUSD",get_STATUSD);
  webServer.on("/D1on",D1_on);
  webServer.on("/D1off",D1_off);
  webServer.on("/D2on",D2_on);
  webServer.on("/D2off",D2_off);
  webServer.on("/D3on",D3_on);
  webServer.on("/D3off",D3_off);
  webServer.on("/D4on",D4_on);
  webServer.on("/D4off",D4_off);
  webServer.on("/Allon",All_on);
  webServer.on("/Alloff",All_off);
  webServer.on("/getIP",get_IP);
  webServer.on("/writeEEPROM",write_EEPROM);
  webServer.on("/restartESP",restart_ESP);
  webServer.on("/clearEEPROM",clear_EEPROM);
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
void get_STATUSD(){
  if(digitalRead(D1)==0){
    statusD1 = "0";
  }else{
    statusD1 = "1";
  }
  if(digitalRead(D2)==0){
    statusD2 = "0";
  }else{
    statusD2 = "1";
  }
  if(digitalRead(D3)==0){
    statusD3 = "0";
  }else{
    statusD3 = "1";
  }
  if(digitalRead(D4)==0){
    statusD4 = "0";
  }else{
    statusD4 = "1";
  }
  String s = "{\"D1\": \""+ statusD1 +"\"," +
              "\"D2\": \""+ statusD2 + "\"," +
              "\"D3\": \""+ statusD3 + "\"," +
              "\"D4\": \""+ statusD4 +"\"}";
  webServer.send(200,"application/json",s);
}
//--------------Điều khiển chân D1----------------
void D1_on(){
  digitalWrite(D1,LOW);
  get_STATUSD();
}
void D1_off(){
  digitalWrite(D1,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D2----------------
void D2_on(){
  digitalWrite(D2,LOW);
  get_STATUSD();
}
void D2_off(){
  digitalWrite(D2,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D3----------------
void D3_on(){
  digitalWrite(D3,LOW);
  get_STATUSD();
}
void D3_off(){
  digitalWrite(D3,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D4----------------
void D4_on(){
  digitalWrite(D4,LOW);
  get_STATUSD();
}
void D4_off(){
  digitalWrite(D4,HIGH);
  get_STATUSD();
}
//--------------Điều khiển chân D----------------
void All_on(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  get_STATUSD();
}
void All_off(){
  digitalWrite(D1,HIGH);
  digitalWrite(D2,HIGH);
  digitalWrite(D3,HIGH);
  digitalWrite(D4,HIGH);
  get_STATUSD();
}

void get_IP(){
  String s = WiFi.localIP().toString();
  webServer.send(200,"text/html", s);
}
boolean read_EEPROM(){
  Serial.println("Reading EEPROM...");
  if(EEPROM.read(0)!=0){
    ssid = "";
    pass = "";
    for (int i=0; i<32; ++i){
      ssid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(ssid);
    for (int i=32; i<96; ++i){
      pass += char(EEPROM.read(i));
    }
    Serial.print("PASSWORD: ");
    Serial.println(pass);
    ssid = ssid.c_str();
    pass = pass.c_str();
    return true;
  }else{
    Serial.println("Data wifi not found!");
    return false;
  }
}
//---------------SETUP WIFI------------------------------
void checkConnection() {
  Serial.println();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.print("Check connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  int count=0;
  while(count < 50){
    if(WiFi.status() == WL_CONNECTED){
      Serial.println();
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("Web Server IP Address: ");
      Serial.println(WiFi.localIP());
      return;
    }
    delay(200);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
  WiFi.softAP(ssid_ap,pass_ap,1,false);
  Serial.println("Soft Access Point mode!");
  Serial.print("Please connect to");
  Serial.println(ssid_ap);
  Serial.print("Web Server IP Address: ");
  Serial.println(ip_ap);
}
void write_EEPROM(){
  ssid = webServer.arg("ssid");
  pass = webServer.arg("pass");
  Serial.println("Clear EEPROM!");
  for (int i = 0; i < 96; ++i) {
    EEPROM.write(i, 0);           
    delay(10);
  }
  for (int i = 0; i < ssid.length(); ++i) {
    EEPROM.write(i, ssid[i]);
  }
  for (int i = 0; i < pass.length(); ++i) {
    EEPROM.write(32 + i, pass[i]);
  }
  EEPROM.commit();
  Serial.println("Writed to EEPROM!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("PASS: ");
  Serial.println(pass);
  String s = "Wifi configuration saved!";
  webServer.send(200, "text/html", s);
}
void restart_ESP(){
  ESP.restart();
}
void clear_EEPROM(){
  Serial.println("Clear EEPROM!");
  for (int i = 0; i < 512; ++i) {
    EEPROM.write(i, 0);           
    delay(10);
  }
  EEPROM.commit();
  String s = "Device has been reset!";
  webServer.send(200,"text/html", s);
}
