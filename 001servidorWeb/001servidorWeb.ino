#include <WiFi.h>
#include <WebServer.h>

// Definir el SSID y la contraseña de la red Wi-Fi
const char* ssid = "ESP32-AP";
const char* password = "12345678";

// Crear una instancia del servidor en el puerto 80
WebServer server(80);

// Función para manejar la solicitud a la ruta raíz "/"
void handleRoot() {
  server.send(200, "text/html", "<h1>Hola, esta es una página web alojada en el ESP32!</h1>");
}

void setup() {
  // Iniciar el monitor serial
  Serial.begin(9600);

  // Configurar el ESP32 como un punto de acceso
  WiFi.softAP(ssid, password);

  // Obtener y mostrar la dirección IP del ESP32
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP del punto de acceso: ");
  Serial.println(IP);

  // Definir la ruta raíz y la función manejadora
  server.on("/", handleRoot);

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  // Manejar las solicitudes de los clientes
  server.handleClient();
}
