#include <WiFi.h>
#include <WebServer.h>

// Definir el SSID y la contraseña de la red Wi-Fi
const char* ssid = "ESP32-AP";
const char* password = "12345678";

// Pin donde está conectado el LED
const int ledPin = 21;

// Crear una instancia del servidor en el puerto 80
WebServer server(80);
bool sw=true;
// Función para manejar la solicitud a la ruta raíz "/"
void handleRoot() {
  // Contenido HTML con un botón para encender el LED
  String html = "<html><body>";
  html += "<h1>Control de LED</h1>";
  html += "<button onclick=\"encenderLED()\">Encender LED</button>";
  html += "<script>function encenderLED() {"
          "var xhr = new XMLHttpRequest();"
          "xhr.open('GET', '/led/on', true);"
          "xhr.send();"
          "}</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Función para encender el LED
void handleLEDOn() {
  if(sw){
    digitalWrite(ledPin, HIGH);  // Encender el LED
    server.send(200, "text/plain", "LED Encendido");
  }else
  {
    digitalWrite(ledPin, LOW);  // Encender el LED
    server.send(200, "text/plain", "LED Encendido");
  }
  sw=!sw;
  
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

  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Inicialmente apagar el LED

  // Definir la ruta raíz y la función manejadora
  server.on("/", handleRoot);
  server.on("/led/on", handleLEDOn);   // Ruta para encender el LED

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  // Manejar las solicitudes de los clientes
  server.handleClient();
}
