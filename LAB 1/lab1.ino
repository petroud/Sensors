int photoResistor = A0; //Analog Input connecting to the photoresistor
int ledPin = 8; //Pin giving voltage to the led
int sensorValue = 0; //Variable to store the value of the photoresistor
float voltageScaled = 0;
float limit = 1.7;
boolean offOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}


void loop() {
  sensorValue=analogRead(photoResistor);

  //Convert to voltage scale 0..5v
  voltageScaled = sensorValue * (5.0 / 1023.0);
  
  if(voltageScaled>limit){
    digitalWrite(ledPin,LOW);
    offOn = false;
  }else{
    digitalWrite(ledPin,HIGH);
    offOn = true;
  } 
  Serial.print(sensorValue);
  Serial.print(";");
  Serial.print(voltageScaled);
  Serial.print(";");
  Serial.print(offOn);
  Serial.println("");
  delay(500);
}
