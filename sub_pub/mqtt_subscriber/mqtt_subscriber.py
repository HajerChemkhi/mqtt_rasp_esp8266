import paho.mqtt.client as mqtt

# Callback when the client receives a CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")

    # Subscribe to the desired topic upon successful connection
    client.subscribe("temperature/humidity")

# Callback when a message is received from the subscribed topic
def on_message(client, userdata, msg):
    print(f"Received message on topic {msg.topic}: {msg.payload.decode()}")

# Create an MQTT client instance
client = mqtt.Client()

# Set the callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker running on the Raspberry Pi
client.connect("localhost", 1883, 60)

# Loop to maintain the network connection and process incoming messages
client.loop_forever()
