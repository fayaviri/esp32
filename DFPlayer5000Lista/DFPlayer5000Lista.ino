#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <ArduinoJson.h>

// ---------------- CONFIGURACIÓN WIFI Y MQTT ----------------
const char* ssid = "fayaviri";
const char* password = "12345678";
const char* mqtt_server = "52.91.226.181";
const int mqtt_port = 1883;
const char* mqtt_topic = "noti/numero";

WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- DFPLAYER ----------------
HardwareSerial mySerial(2); // UART2
DFRobotDFPlayerMini myDFPlayer;

#define DF_BUSY_PIN 4  // ← conectar aquí el pin BUSY del DFPlayer

// ---------------- FUNCIONES ----------------
void reproducirArchivo(int carpeta, int archivo) {
  Serial.printf("🔊 Reproduciendo carpeta %02d archivo %03d\n", carpeta, archivo);

  myDFPlayer.stop();
  delay(100);
  myDFPlayer.playFolder(carpeta, archivo);
  delay(100);  // espera inicial

  unsigned long t0 = millis();
  unsigned long timeout = 8000;  // seguridad

  // Esperar a que termine usando BUSY
  while (digitalRead(DF_BUSY_PIN) == LOW && (millis() - t0 < timeout)) {
    Serial.println(" DF_BUSY_PIN");
    delay(50);
  }

  Serial.println("✅ Finalizó reproducción");
  delay(100); // espacio entre audios
}

// ---------------- MQTT CALLBACK ----------------
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  Serial.println("📥 callbackMQTT INVOCADO");

  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  mensaje.trim();
  Serial.println("📨 JSON recibido por MQTT:");
  Serial.println(mensaje);

  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, mensaje);
  if (error) {
    Serial.print("❌ Error al parsear JSON: ");
    Serial.println(error.c_str());
    return;
  }

  JsonArray pasos = doc.as<JsonArray>();
  if (pasos.size() == 0) {
    Serial.println("⚠️ JSON vacío o mal formado.");
    return;
  }

  int total = pasos.size();
  int index = 1;

  for (JsonObject paso : pasos) {
    int carpeta = paso["carpeta"];
    int archivo = paso["archivo"];
    Serial.printf("▶ Reproduciendo paso %d de %d\n", index, total);
    reproducirArchivo(carpeta, archivo);
    index++;
  }

  Serial.println("✅ Reproducción completa.");
}

// ---------------- CONECTAR MQTT ----------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("🔄 Conectando MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("✅ Conectado al broker MQTT");
      client.subscribe(mqtt_topic);
      Serial.printf("📡 Suscrito al tópico: %s\n", mqtt_topic);
    } else {
      Serial.print("❌ Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" - reintentando en 2 segundos...");
      delay(2000);
    }
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // TX=16, RX=17

  pinMode(DF_BUSY_PIN, INPUT); // ← configurar pin BUSY

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("❌ DFPlayer error");
    while (true);
  }
  myDFPlayer.volume(30);
  Serial.println("✅ DFPlayer listo");

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado a WiFi");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackMQTT);
  client.setBufferSize(1024);
}

// ---------------- LOOP ----------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
