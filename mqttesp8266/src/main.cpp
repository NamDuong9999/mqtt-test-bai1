#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "NAM DEP TRAI"; // doi ten wifi dang dung
const char *password = "saigon8888"; // mat khau wifi dang dung
const char *mqtt_server = "192.168.1.6";//dia chi ip cuc bo tren may tinh dang test cua ban
const int mqtt_port = 1883; // port test mac dinh cua node-red
const char *mqtt_id = "esp8266"; // ten ket noi den mqtt serer

unsigned long tg=0;
int count=0;

WiFiClient client; // tao wificlient
PubSubClient mqtt_client(client); //tao ket noi mqtt

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Recived data from: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (size_t i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("------------------------------------------");
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Connecting to Wifi... ");
  Serial.print(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to Wifi ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println();
  delay(1000);

  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);

  Serial.println("Connecting to mqtt... ");
  while (!mqtt_client.connect(mqtt_id))
  {
    delay(500);
  }
  Serial.println("Connected to mqtt ");
  mqtt_client.subscribe("to-esp8266");
  mqtt_client.publish("from-esp8266", "Hello Server");
}
void senServer(){
 if(millis() -tg >500){
   count = random(0,3000);
   
   if(mqtt_client.connected()){
    mqtt_client.publish("from-esp8266",String(count).c_str());//ep kieu String
   }
   tg =millis();
 }
}
void loop()
{
  mqtt_client.loop();
  senServer();
}