import paho.mqtt.client as mqtt

# Configuración del broker MQTT
broker = "52.91.226.181"
puerto = 1883
topic = "noti/numero"

# Mensaje que quieres enviar (puede ser decimal o entero)
# numero = 225.45  # Cambia este valor
numero = 3234.53  # Cambia este valor

# Crear el cliente MQTT
client = mqtt.Client()

# Conectar al broker
try:
    client.connect(broker, puerto, 60)
    print(f"✅ Conectado a {broker}:{puerto}")
except Exception as e:
    print(f"❌ Error de conexión: {e}")
    exit()

# Publicar el número
mensaje = str(numero)
client.publish(topic, mensaje)
print(f"📤 Enviado '{mensaje}' al topic '{topic}'")

# Cerrar conexión
client.disconnect()