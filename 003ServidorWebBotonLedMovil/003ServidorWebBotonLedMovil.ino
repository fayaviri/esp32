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
  // Contenido HTML con CSS para hacer el botón responsive
  String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<title>Control de LED</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }";
  html += "h1 { color: #333; }";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px;";
  html += "text-align: center; text-decoration: none; display: inline-block; font-size: 16px;";
  html += "margin: 4px 2px; cursor: pointer; border-radius: 12px; }";
  html += ".button:hover { background-color: #45a049; }";
  html += "</style></head><body>";
  html += "<h1>Control de LED</h1>";
  html += "<button class=\"button\" onclick=\"encenderLED()\">Encender/Apagar LED</button>";
  html += "<script>function encenderLED() {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/led/on', true);";
  html += "xhr.send();"
  html += "}</script>";
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
