#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ---------------- CONFIGURACIÓN WIFI Y MQTT ----------------
const char* ssid = "Ayaviri";
const char* password = "76030742";

// const char* ssid = "fayaviri";
// const char* password = "12345678";


const char* mqtt_server = "52.91.226.181";
const int mqtt_port = 1883;
const char* mqtt_topic = "noti/numero";

WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- DFPLAYER ----------------
HardwareSerial mySerial(2); // UART2
DFRobotDFPlayerMini myDFPlayer;

// ---------------- TIEMPOS POR DÍGITO ----------------
// int delay_por_digito[] = {
//   0,     // índice 0 (no se usa)
//   700,   // 1 dígito
//   1200,  // 2 dígitos
//   1700,  // 3 dígitos
//   2100,  // 4 dígitos
//   2200   // 5 dígitos
// };

int delay_por_digito[] = {
  0,     // índice 0 (no se usa)
  700,   // 1 dígito
  1200,  // 2 dígitos
  1700,  // 3 dígitos
  2500,  // 4 dígitos
  2900   // 5 dígitos
};

// ---------------- TIEMPOS AUDIOS FIJOS CARPETA 21 ----------------
int delay_carpeta_21[] = {
  0,     // 000 (no usado)
  1368,  // 001.mp3 → Te han depositado
  1080,  // 002.mp3 → Has recibido
  960,   // 003.mp3 → Boliviano
  1080,  // 004.mp3 → Bolivianos
  912,   // 005.mp3 → Centavo
  984,   // 006.mp3 → Centavos
  576,   // 007.mp3 → con
  504    // 008.mp3 → y (si lo usas después)
};

// ---------------- FUNCIONES ----------------
void reproducirArchivo(int carpeta, int archivo, int delay_ms = 800) {
  Serial.printf("🔊 Reproduciendo carpeta %02d archivo %03d\n", carpeta, archivo);
  myDFPlayer.playFolder(carpeta, archivo);
  delay(delay_ms); // espera personalizada
}

int obtenerDelayPorNumero(int numero) {
  int digitos = String(numero).length();
  if (digitos > 5) digitos = 5;
  return delay_por_digito[digitos];
}

void reproducirNumeroCompleto(float monto) {
  int entero = (int)monto;
  int decimales = (int)((monto - entero) * 100 + 0.5);

  // Elegir frase inicial: par = "Te han depositado", impar = "Has recibido"
  int saludo = (entero % 2 == 0) ? 1 : 2;
  Serial.printf("💡 Monto %.2f es %s\n", monto, (saludo == 1 ? "par (Te han depositado)" : "impar (Has recibido)"));
  reproducirArchivo(21, saludo, delay_carpeta_21[saludo]);

  // Parte entera
  int carpetaNumero = (entero - 1) / 255 + 1;
  int archivoNumero = ((entero - 1) % 255) + 1;
  int delay_entero = obtenerDelayPorNumero(entero);
  reproducirArchivo(carpetaNumero, archivoNumero, delay_entero);

  // Boliviano(s)
  int boliviano_index = (entero == 1 ? 3 : 4);
  reproducirArchivo(21, boliviano_index, delay_carpeta_21[boliviano_index]);

  if (decimales > 0) {
    reproducirArchivo(21, 7, delay_carpeta_21[7]);  // "con"

    int carpetaDecimal = (decimales - 1) / 255 + 1;
    int archivoDecimal = ((decimales - 1) % 255) + 1;
    int delay_decimales = obtenerDelayPorNumero(decimales);
    reproducirArchivo(carpetaDecimal, archivoDecimal, delay_decimales);

    int centavo_index = (decimales == 1 ? 5 : 6);
    reproducirArchivo(21, centavo_index, delay_carpeta_21[centavo_index]);
  }
}

// ---------------- MQTT CALLBACK ----------------
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
}

// ---------------- LOOP ----------------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}