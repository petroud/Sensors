int thermistor = A0; //Analog Input connecting to the photoresistor
int ledPin = 8; //Pin giving voltage to the led
int sensorValue = 0; //Variable to store the value of the photoresistor
float voltageScaled = 0;
float limit = 30.0;
float klimit = 273.2+limit;
float resistance, logR = 0.0;
float lnDenom = 0.0;
double kelvins;
double R0 = 98500;

float beta = 4250.0;


void setup() {
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop() {
  sensorValue=analogRead(thermistor);
  //Convert to voltage scale 0..5v
  voltageScaled = sensorValue * (5.0 / 1023.0);

  //resistance = R0*((5/voltageScaled) - 1);
  resistance = (voltageScaled*R0)/(5-voltageScaled);

  
  lnDenom = log(resistance/(R0*exp(-beta/298)));
  kelvins = (beta/lnDenom);
    
  if(kelvins>klimit){
    digitalWrite(ledPin,HIGH);
  }else{
    digitalWrite(ledPin,LOW);
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
