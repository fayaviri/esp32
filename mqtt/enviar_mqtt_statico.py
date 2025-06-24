import paho.mqtt.client as mqtt
import json

# ------------------ ARRAY AUTOMÁTICO CON MILISEGUNDOS ------------------
ejemplo = [
    {
        "carpeta": 21,
        "archivo": 1,
        "milisegundos": 1120
    },
    {
        "carpeta": 14,
        "archivo": 249,
        "milisegundos": 4080
    },
    {
        "carpeta": 21,
        "archivo": 4,
        "milisegundos": 1080
    },
    {
        "carpeta": 21,
        "archivo": 7,
        "milisegundos": 576
    },
    {
        "carpeta": 1,
        "archivo": 56,
        "milisegundos": 1200
    },
    {
        "carpeta": 21,
        "archivo": 6,
        "milisegundos": 984
    }
]

# ------------------ CONFIGURACIÓN MQTT ------------------
broker = "52.91.226.181"
puerto = 1883
topic = "noti/numero"

# Convertir array a JSON
mensaje = json.dumps(ejemplo, ensure_ascii=False)

# Enviar por MQTT
client = mqtt.Client()
try:
    client.connect(broker, puerto, 60)
    client.publish(topic, mensaje)
    print(f"📤 Enviado: {mensaje}")
    client.disconnect()
except Exception as e:
    print(f"❌ Error: {e}")
