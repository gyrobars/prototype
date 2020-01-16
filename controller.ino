#include <WiFi.h>
#include "MPU9250.h"

MPU9250 IMU(Wire,0x68);
int status;
const int weight=10;
float temp_X[2][100];

char ssid[] = "HelloCar";
char pass[] = "99eleven";
IPAddress server(192,168,4,15);
WiFiClient client;
int ledPin = 2;

void setup() 
{
  Serial.begin(115200);
  for(int i=0;i<weight;i++) 
  {
    temp_X[0][i]=0;
    temp_X[1][i]=0;
  }
  
  while(!Serial) {} 
  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }

   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, pass);           // connects to the WiFi AP
   Serial.println();
   Serial.println("Connection to the AP");
   while (WiFi.status() != WL_CONNECTED) 
     {
      Serial.print(".");
      delay(500);
     }
   Serial.println();
   Serial.println("Connected");
   Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
   Serial.println("MAC:" + WiFi.macAddress());
   Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
   Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
   pinMode(ledPin, OUTPUT);
}

void loop() 
{
  int i;
  if(!client.connected()) client.connect(server, 80);
  float value_x=0;
  
  IMU.readSensor();
  
  for(i=weight-2;i>=0;i--)
  {
    temp_X[0][i+1]=temp_X[0][i];
    temp_X[1][i+1]=temp_X[1][i];
  }
  temp_X[0][0]=IMU.getAccelX_mss();
  temp_X[1][0]=IMU.getGyroX_rads();
  for( i=0;i<weight;i++) value_x+=0.7*temp_X[0][i]+0.3*temp_X[1][i];
  value_x/=weight;
  Serial.print(value_x);
  Serial.println();
  client.println(enc_val);
  delay(50);
}
