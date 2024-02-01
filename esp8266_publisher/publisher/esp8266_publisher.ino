#include <PubSubClient.h>
#include <DHT.h>
#include <ESP8266WiFi.h>

const char *ssid = "TT_A5A8";
const char *password = "hpcihdy4c9";
const char *mqttBroker = "192.168.1.12";
const char *mqttTopic = "temperature";
const char *mqttUser = "hajer";
const char *mqttPassword = "root";

const int dhtPin = 4;

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(dhtPin, DHT11);

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  client.setServer(mqttBroker, 1883);
    client.setKeepAlive(60);  // Set the keep-alive interval to 60 seconds

  dht.begin();
   delay(100); 

}
void loop() {
  delay(3000); // Delay for DHT sensor to stabilize

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C | Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    Serial.print("Connecting to MQTT broker... ");
    
    if (client.connect("ESP8266Clienthajer", "hajer", "root", "mqtt_will_topic", 0, true, "offline")) {
      Serial.println("Connected to MQTT broker!");
      
      Serial.print("Sending data to MQTT broker... ");
      if (client.publish(mqttTopic, String(temperature).c_str())) {
        Serial.println(" Temperature Data sent successfully!");
      } else {
        Serial.println("Failed to send data to MQTT broker!");
      }
      client.disconnect();
    } else {
      Serial.print("Failed to connect to MQTT broker! Error code: ");
      Serial.println(client.state());
    }
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(15000); // Adjust as needed
}