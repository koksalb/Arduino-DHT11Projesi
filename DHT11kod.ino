
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <dht11.h>


byte mac[]    = {0x34, 0xE6, 0xD7, 0x18, 0xDE, 0x89 }; //Kocsistem tarafından verilen MAC adresi
char macstr[] = "deedbafefeed";//random key

byte localserver[] = {192,168,1,2 };//gateaway adresi
byte ip[]     = {192,168,1,4 };//cihazın ip adresi

String clientName = String("d:quickstart:arduino:") + macstr;
String topicName = String("iot-2/evt/status/fmt/json");

PubSubClient client(localserver, 1883, 0, ethClient);

dht11 DHT11;
float tempF = 0.0;
float tempC = 0.0;
float humidity = 0.0;
EthernetClient ethClient;
void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  DHT11.attach(3);
}

void loop()
{
  char clientStr[34];
  clientName.toCharArray(clientStr,34);
  char topicStr[26];
  topicName.toCharArray(topicStr,26);
  getData();
  if (!client.connected()) {
    Serial.print("Trying to connect to: ");
    Serial.println(clientStr);
    client.connect(clientStr);
  }
  if (client.connected() ) {
    String json = buildJson();
    char jsonStr[200];
    json.toCharArray(jsonStr,200);
    boolean pubresult = client.publish(topicStr,jsonStr);
    Serial.print("attempt to send ");
    Serial.println(jsonStr);
    Serial.print("to ");
    Serial.println(topicStr);
    if (pubresult)
      Serial.println("successfully sent");
    else
      Serial.println("unsuccessfully sent");
  }
  delay(5000);
}

String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"myName\": \"Arduino DHT11\",";
  data+="\n";
  data+="\"temperature (F)\": ";
  data+=(int)tempF;
  data+= ",";
  data+="\n";
  data+="\"temperature (C)\": ";
  data+=(int)tempC;
  data+= ",";
  data+="\n";
  data+="\"humidity\": ";
  data+=(int)humidity;
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}
void getData() {
  int chk = DHT11.read();
  switch (chk)
  {
  case 0: 
    Serial.println("Read OK"); 
    humidity = (float)DHT11.humidity;
    tempF = DHT11.fahrenheit();
    tempC = DHT11.temperature();
    break;
  case -1: 
    Serial.println("Checksum error"); 
    
    break;
  case -2: 
    Serial.println("Time out error"); 
    break;
  default: 
    Serial.println("Unknown error"); 
    break;
  }
}






