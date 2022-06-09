#include <SoftwareSerial.h>
// serial config
#define     RX    10
#define     TX    11
SoftwareSerial AT(RX,TX); 

// TODO: change user config
String ssid     = "HPY413"; //Wifi SSID
String password = "#@!PY314"; //Wifi Password

//Requests uri 
String requestUri1 = "GET /update?api_key=AXGNNYBQCY2NNXIB&field1=";
String tempArg = "&field2=";
String requestUri2 = "GET /update?api_key=MVZ6ZSXYVRZ5X7GK&field2=";

// Channel public link : https://thingspeak.com/channels/1723656
String host = "api.thingspeak.com";  // API host name
int AT_cmd_time; 
boolean AT_cmd_result = false; 

int sensorValue = 0;
int terminationValue = 0;
int terminationLimit = 4;
float voltageTermin = 0.0;
float voltageLM = 0.0;
float temp = 0.0;
float temp_ref = 298.2;
float v_ref = 2.982;


void setup() {
  Serial.begin(9600);

  //Initiate serial communication
  Serial.println("*****************************************************************************");
  Serial.println("******* Program Start : Arduino and ESP8266 connected to IoT service ********");
  AT.begin(115200);

  Serial.println("--> Initiating AT communicatin with ESP8266...");
  sendATcmd("AT",5,"OK");
  sendATcmd("AT+CWMODE=1",5,"OK");

  //Connect to wireless network
  Serial.print("--> Connecting to WiFi: ");
  Serial.println(ssid);
  sendATcmd("AT+CWJAP=\""+ ssid +"\",\""+ password +"\"",20,"OK");
  
  
  delay(2000);
  openTCP();
}

void loop() {
    sensorValue = analogRead(A5);
    terminationValue = analogRead(A3);
    voltageTermin = terminationValue* (5.0 / 1023.0);

    if(voltageTermin > terminationLimit){
      closeTCP();
      delay(1000);
      exit(0);
    }

    voltageLM = sensorValue * (5.0 / 1023.0);
    temp = (voltageLM * temp_ref)/v_ref;
    temp = temp - 273.2;
    Serial.print(voltageLM);
    Serial.print(";");
    Serial.print(temp);
    Serial.print(";");
    Serial.println(terminationValue);

    sendData(requestUri1, voltageLM, temp);
    delay(5000);
    closeTCP();

    openTCP();
    shareData(requestUri2, temp);
    delay(5000);
    closeTCP();

    delay(2000);
    openTCP();

    Serial.println("\n\nNew cycle of measurements\n");
}


void sendData(String request_url, float voltage, float temp){   
   request_url += String(voltage,2) + tempArg + String(temp,2);
   Serial.print("\n--> Request URI: ");
   Serial.println(request_url);
   sendATcmd("AT+CIPSEND=0," + String(request_url.length() + 4), 10, ">");
   AT.println(request_url);
   delay(3000);
}


void shareData(String request_url,float temp){
   request_url += String(temp,2);
   Serial.print("\n--> Request URI: ");
   Serial.println(request_url);
   sendATcmd("AT+CIPSEND=0," + String(request_url.length() + 4), 10, ">");
   AT.println(request_url);
   delay(3000);
}



void openTCP(){
  Serial.println("**********************************************************");
  Serial.println("******* Open TCP connection to Thingspeak REST API *******");
  sendATcmd("AT+CIPMUX=1", 10, "OK");
  sendATcmd("AT+CIPSTART=0,\"TCP\",\"" + host +"\"," + "80", 20, "OK");
}

void closeTCP(){
  delay(2000);
  Serial.println("--> Closing TCP connection...");
  sendATcmd("AT+CIPCLOSE=0", 10, "OK");
  Serial.println("****************** TCP Connection Ended *******************");
  Serial.println("***********************************************************");
}


// sendATcmd
void sendATcmd(String AT_cmd, int AT_cmd_maxTime, char readReplay[]) {
  Serial.print("--> AT command to send: ");
  Serial.println(AT_cmd);

  while(AT_cmd_time < (AT_cmd_maxTime)) {
    AT.println(AT_cmd);
    if(AT.find(readReplay)) {
      AT_cmd_result = true;
      break;
    }
    AT_cmd_time++;
  }
  
  Serial.print("...Result:");
  
  if(AT_cmd_result == true) {
    Serial.println("SUCCESS");
    AT_cmd_time = 0;
  }
  
  if(AT_cmd_result == false) {
    Serial.println("FAILURE");
    AT_cmd_time = 0;
  }
  
  AT_cmd_result = false;
 }
