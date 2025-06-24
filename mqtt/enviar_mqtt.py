import json
import paho.mqtt.client as mqtt

# ------------------ CONFIGURACIÓN ------------------
broker = "52.91.226.181"
puerto = 1883
topic = "noti/numero"
numero = 3564.56  # 🔁 Cambia este número para probar otro

# ------------------ CARGAR JSON ------------------
with open("todos_los_audios.json", "r", encoding="utf-8") as f:
    audios = json.load(f)

def buscar_audio(numero):
    for audio in audios:
        if audio.get("numero") == numero:
            return audio
    return None

def armar_objeto(carpeta, archivo):
    for audio in audios:
        if audio["carpeta"] == carpeta and audio["archivo"] == archivo:
            return {
                "carpeta": carpeta,
                "archivo": archivo,
                "milisegundos": audio["milisegundos"]
            }
    # Valor por defecto si no se encuentra
    return {
        "carpeta": carpeta,
        "archivo": archivo,
        "milisegundos": 1000
    }

# ------------------ CONVERSIÓN ------------------
entero = int(numero)
decimales = int(round((numero - entero) * 100))

def get_carpeta_archivo(n):
    carpeta = (n - 1) // 255 + 1
    archivo = ((n - 1) % 255) + 1
    return carpeta, archivo

# ------------------ ARMAR ARRAY JSON ------------------
ejemplo = []

# Frase inicial
saludo = 1 if entero % 2 == 0 else 2
ejemplo.append(armar_objeto(21, saludo))

# Parte entera
carpeta_entero, archivo_entero = get_carpeta_archivo(entero)
ejemplo.append(armar_objeto(carpeta_entero, archivo_entero))

# Boliviano(s)
if entero == 1:
    ejemplo.append(armar_objeto(21, 3))  # boliviano
else:
    ejemplo.append(armar_objeto(21, 4))  # bolivianos

# Parte decimal
if decimales > 0:
    ejemplo.append(armar_objeto(21, 7))  # "con"

    carpeta_dec, archivo_dec = get_carpeta_archivo(decimales)
    ejemplo.append(armar_objeto(carpeta_dec, archivo_dec))

    if decimales == 1:
        ejemplo.append(armar_objeto(21, 5))  # centavo
    else:
        ejemplo.append(armar_objeto(21, 6))  # centavos

# ------------------ ENVIAR POR MQTT ------------------
mensaje = json.dumps(ejemplo, ensure_ascii=False)

client = mqtt.Client()
try:
    client.connect(broker, puerto, 60)
    client.publish(topic, mensaje)
    print(f"📤 Enviado: {mensaje}")
    client.disconnect()
except Exception as e:
    print(f"❌ Error: {e}")
