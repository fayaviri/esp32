#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial mySerial(2);  // UART2: TX=16, RX=17
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  delay(1000);  // Espera inicial

  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("❌ Error inicializando DFPlayer Mini");
    while (true);
  }

  myDFPlayer.volume(25);
  Serial.println("✅ DFPlayer inicializado. Estabilizando...");

  // 🔧 "Warmup" - estabiliza el DFPlayer
  delay(1000);  // Importante: darle tiempo al módulo
  int totalFiles = myDFPlayer.readFileCounts();  // Fuerza la comunicación
  Serial.print("📁 Archivos detectados: ");
  Serial.println(totalFiles);
  delay(500);  // Otro pequeño delay

  // 🔊 Reproducción real
  Serial.println("▶ Reproduciendo ahora sí:");
  myDFPlayer.playFolder(14, 249);
}

void loop() {
  // Nada aquí
}
