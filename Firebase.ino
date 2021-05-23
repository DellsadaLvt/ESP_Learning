#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "Ngoc Nu"
#define STAPSK  "03041997"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;


#define FIREBASE_HOST "demoesp8266-231c8-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "WkfJzzeLmbg19AkCofckphQtVE98JPDfsR8a986E"
FirebaseData firebaseData;
FirebaseJson json;
String path = "/";

unsigned long ti;
byte x;

void setup(){
  /* Setup Serial*/
  //Serial.begin(9600);

  /* SETUP WIFI */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected");
  // Print the IP address
  //Serial.println(WiFi.localIP());

  /* SETUP FIREBASE */
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if( !Firebase.beginStream(firebaseData, path)){
    //Serial.println("Reason: " + firebaseData.errorReason());
    //Serial.println();
  }

  /* SETUP IO */
  pinMode( D4, OUTPUT);
  digitalWrite(D4, LOW);

  /* INITIAL VALUE */
  ti = 0;
}

/*===========================
 * Transfer data 
 * Firebase.setInt(firebaseData, path + "/data", random(0, 100));
 * Firebase.setFloat(firebaseData, path + "/data", random(0, 100)/3.0);
 * Firebase.setString(firebaseData, path + "/data", "Dellsada");
 *
 * Getdata
 * if( Firebase.getInt(firebaseData, path + "/data") ){
 *      x = firebaseData.intData();
 * }
 =============================*/
void loop(){
  if( millis() <= 2000){
    ti = 0;
  }
  if(millis() - ti >= 1000){
    ti = millis();
    if( Firebase.getInt(firebaseData, path + "/data") ){
       x = firebaseData.intData();
    }
    
    ControlLed( &x );
  }
}

/*=============== SUBROUTINE ============*/
void ControlLed( byte *x ){
  switch( *x){
    case 0:
      digitalWrite( D4, HIGH);
      break;
    case 1:
      digitalWrite( D4, LOW);
      break;
    case 2:
      digitalWrite( D4, HIGH);
      delay(1000);
      digitalWrite( D4, LOW);
      delay(1000);
      break;
    default:
      break;
  } 
}
