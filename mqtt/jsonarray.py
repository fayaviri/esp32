import os
import json
from pydub import AudioSegment

# Ruta base donde están las carpetas 1, 2, ..., 21
RUTA_BASE = r"C:\Users\fayaviri\Documents\github\esp32\mqtt"

datos = []

for carpeta in range(1, 22):  # del 1 al 21 inclusive
    carpeta_path = os.path.join(RUTA_BASE, f"{carpeta:02}")
    if not os.path.isdir(carpeta_path):
        continue

    for archivo in os.listdir(carpeta_path):
        if archivo.lower().endswith(".mp3"):
            archivo_num = int(os.path.splitext(archivo)[0])
            ruta_mp3 = os.path.join(carpeta_path, archivo)

            try:
                audio = AudioSegment.from_mp3(ruta_mp3)
                duracion_ms = len(audio)

                datos.append({
                    "carpeta": carpeta,
                    "archivo": archivo_num,
                    "milisegundos": duracion_ms,
                    "numero": carpeta * 1000 + archivo_num  # ID único estimado
                })

                print(f"✅ {ruta_mp3} → {duracion_ms} ms")

            except Exception as e:
                print(f"⚠️ Error con {ruta_mp3}: {e}")

# Guardar en archivo JSON
with open("todos_los_audios.json", "w", encoding="utf-8") as f:
    json.dump(datos, f, ensure_ascii=False, indent=2)

print(f"\n🎉 Listo. Total audios procesados: {len(datos)}")
