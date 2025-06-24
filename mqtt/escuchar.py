import paho.mqtt.client as mqtt

# Configuración del broker MQTT
broker = "52.91.226.181"
puerto = 1883
topic = "noti/numero"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Conectado al broker MQTT")
        client.subscribe(topic)
        print(f"📡 Suscrito al topic: {topic}")
    else:
        print("❌ Error de conexión:", rc)

def on_message(client, userdata, msg):
    print("📨 Mensaje recibido:")
    print(f"📍 Topic: {msg.topic}")
    print(f"📦 Payload: {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(broker, puerto, 60)
    print(f"⏳ Conectando a {broker}:{puerto}...")
    client.loop_forever()
except Exception as e:
    print("❌ Error:", e)
