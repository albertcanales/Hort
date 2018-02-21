#include <dht.h>
#include <SoftwareSerial.h>

#define WATER_SENSOR 14
#define LDR 15
#define DHT_SENSOR 16

dht DHT;
SoftwareSerial bt(10,11); // RX, TX
byte byteReceived;

int earthHumidity;
int light;
int airHumidity;
int temperature;
long t = 0;

byte wateringFrequency = 8;
byte timeWatering = 5;
byte waterUnderHumidity = 30;


void setup() {
  Serial.begin(9600);
  bt.begin(38400);
  for(int i = 14; i < 17; i++) {
      pinMode(i, INPUT);
    }
  pinMode(3,OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  CheckReceivingData();
  CheckWatering();
}

void CheckReceivingData() {
  if(bt.available()>0) {
    byteReceived = bt.read();
    Serial.println("Receiving Data...");
    Serial.println(byteReceived);
    Serial.println("");
    switch(byteReceived) {
      case 2:
        UpdateSensors();
        break;
      case 3:
        UpdateConfig();      
        break;
      case 4:
        WaterNow();
        break;
      case 5:
        SendConfig();
        break;        
      default:
        bt.read();
    }
  }
}

void CheckWatering() {
  if( millis() / (wateringFrequency * 3600000) != t || waterUnderHumidity > earthHumidity) {
    t = millis() / (wateringFrequency * 3600000);
    WaterNow();
    }
  }
  
void UpdateSensors() {
    Serial.println("Updating Sensors...");
    bt.print(2);
    bt.print("|");
    Serial.print("Earth humidity: ");
    earthHumidity = analogRead(WATER_SENSOR); 
    Serial.println(earthHumidity);
    bt.print(earthHumidity);
    bt.print("|");
    Serial.print("Light: ");
    light = analogRead(LDR);
    Serial.println(light);
    bt.print(light);
    bt.print("|");
    int chk = DHT.read11(DHT_SENSOR);
    Serial.print("Air humidity: ");
    airHumidity = (DHT.humidity, 0);    
    Serial.println(airHumidity);
    bt.print(airHumidity);
    bt.print("|");
    Serial.print("Temperature: ");
    temperature = DHT.temperature, 0;
    Serial.println(temperature);
    bt.print(temperature);
    Serial.println("");
  }

void UpdateConfig() {
    Serial.println("Updating Configuration...");
    bt.print(3);
    bt.print("|");
    Serial.print("Watering frequency: ");
    Serial.println(wateringFrequency);
    bt.print(wateringFrequency); 
    bt.print("|");
    Serial.print("Time watering: ");
    Serial.println(timeWatering);
    bt.print(timeWatering); 
    bt.print("|");
    Serial.print("Water when humidity drops: ");
    Serial.println(waterUnderHumidity);
    bt.print(waterUnderHumidity);
    Serial.println("");
  }

void WaterNow() {
  Serial.println("Watering now...");
  Serial.println("");
  digitalWrite(4, HIGH);
  delay(timeWatering * 1000);
  digitalWrite(4, LOW);
  }

void SendConfig() {
  Serial.println("Sending Configuration...");
  delay(100);
  wateringFrequency = bt.read();
  while (wateringFrequency == 255) {
    wateringFrequency = bt.read();
    }
  Serial.print("Watering Frequency: ");
  Serial.println(wateringFrequency);
  delay(100);
  timeWatering = bt.read();
  Serial.print("Time Watering: ");
  Serial.println(timeWatering);
  delay(100);
  waterUnderHumidity = bt.read();
  Serial.print("Water when humidity drops: ");
  Serial.println(waterUnderHumidity);
  Serial.println("");
  delay(200);
  UpdateConfig();
  

  }
