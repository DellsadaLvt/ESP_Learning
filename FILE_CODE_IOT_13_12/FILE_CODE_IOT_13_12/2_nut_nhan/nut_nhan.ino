int ledPin = 16;                 // LED connected to digital pin 16 ( D0)
int btnPin = 0;                  // BUTTON connected to digital pin 0 ( D3)
//int ledState = LOW;

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  pinMode(btnPin, INPUT);       // sets the digital pin as input
  digitalWrite(btnPin, HIGH);
  Serial.begin(115200);
}
void loop()
{
 if(digitalRead(btnPin)==LOW)
 {
  digitalWrite ( ledPin, HIGH);
 }
 else { digitalWrite ( ledPin, LOW);}
}

