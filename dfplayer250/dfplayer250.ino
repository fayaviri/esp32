#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ---------------- CONFIGURACIÓN WIFI Y MQTT ----------------
const char* ssid = "Ayaviri";
const char* password = "76030742";
const char* mqtt_server = "192.168.1.9"; // Tu nuevo broker MQTT
const int mqtt_port = 1883;
const char* mqtt_topic = "noti/numero";

WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- DFPLAYER ----------------
HardwareSerial mySerial(2); // UART2: GPIO 16 (RX), GPIO 17 (TX)
DFRobotDFPlayerMini myDFPlayer;

// ---------------- AUDIOS ESPECIALES ----------------
#define AUDIO_TE_HAN_DEPOSITADO 256
#define AUDIO_HAS_RECIBIDO     257
#define AUDIO_BOLIVIANO        258
#define AUDIO_BOLIVIANOS       259
#define AUDIO_CENTAVO          260
#define AUDIO_CENTAVOS         261
#define AUDIO_CON              262
#define AUDIO_Y                263

// ---------------- TIEMPOS POR DÍGITO ----------------
int delay_por_digito[] = {
  0,     // índice 0 (no se usa)
  700,   // 1 dígito
  1200,  // 2 dígitos
  1700,  // 3 dígitos
  2500,  // 4 dígitos
  2900   // 5 dígitos
};

// ---------------- FUNCIONES ----------------
void reproducirArchivo(int numero, int delay_ms = 800) {
  Serial.printf("🔊 Reproduciendo archivo %03d.mp3\n", numero);
  myDFPlayer.play(numero);
  delay(delay_ms);
}

int obtenerDelayPorNumero(int numero) {
  int digitos = String(numero).length();
  if (digitos > 5) digitos = 5;
  return delay_por_digito[digitos];
}

void reproducirNumeroCompleto(float monto) {
  int entero = (int)monto;
  int decimales = (int)((monto - entero) * 100 + 0.5);

  if (entero < 1 || entero > 255) {
    Serial.println("❌ Número fuera de rango (1–255)");
    return;
  }

  // Frase inicial
  int saludo = (entero % 2 == 0) ? AUDIO_TE_HAN_DEPOSITADO : AUDIO_HAS_RECIBIDO;
  reproducirArchivo(saludo, 1100);

  // Parte entera
  reproducirArchivo(entero, obtenerDelayPorNumero(entero));

  // Boliviano(s)
  int moneda = (entero == 1 ? AUDIO_BOLIVIANO : AUDIO_BOLIVIANOS);
  reproducirArchivo(moneda, 1000);

  // Parte decimal
  if (decimales > 0) {
    reproducirArchivo(AUDIO_CON, 600);
    reproducirArchivo(decimales, obtenerDelayPorNumero(decimales));
    int centavo = (decimales == 1 ? AUDIO_CENTAVO : AUDIO_CENTAVOS);
    reproducirArchivo(centavo, 1000);
  }
}

// ---------------- CALLBACK MQTT ----------------
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  mensaje.trim();
  Serial.print("📨 MQTT recibido: ");
  Serial.println(mensaje);
  float valor = mensaje.toFloat();
  reproducirNumeroCompleto(valor);
}

// ---------------- CONECTAR MQTT ----------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("🔄 Conectando MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("✅ Conectado");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("❌ Fallo, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("❌ DFPlayer no inicializado");
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
}

// ---------------- LOOP ----------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
