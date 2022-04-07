int thermistor = A0; //Analog Input connecting to the photoresistor
int ledPin = 8; //Pin giving voltage to the led
int sensorValue = 0; //Variable to store the value of the photoresistor
float voltageScaled = 0;
float limit = 30.0;
float klimit = 273.2+limit;
float kelvins, resistance = 0.0;

float beta = 0.0;
float R0 = 0.0;
float R1 = 220;
float R2 = 100000;
float logR2 = log(R2);
float A,B,C;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  sensorValue=analogRead(thermistor);
  //Convert to voltage scale 0..5v
  voltageScaled = sensorValue * (5.0 / 1023.0);

  kelvins = (1.0 / (A + B*logR2 + C*logR2*logR2*logR2));
  resistance = R0 * exp(beta * (1/kelvins - 1/298));
  
  if(kelvins>klimit){
    digitalWrite(ledPin,LOW);
  }else{
    digitalWrite(ledPin,HIGH);
  } 

  Serial.print(sensorValue);
  Serial.print(";");
  Serial.print(voltageScaled);
  Serial.print(";");
  Serial.print(resistance);
  Serial.print(";");
  Serial.print(kelvins-273.2);
  Serial.println("");
  delay(500);
}
