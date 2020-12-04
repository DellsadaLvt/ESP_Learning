
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);// chân điều khiển relay là chân GIPO13 ( D7)
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the RELAY on
  delay(5000);              // wait for  5 second
  digitalWrite(13, LOW);    // turn the RELAY off
  delay(2000);              // wait for  2 second
}
