#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  delay(1000);

  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("❌ DFPlayer no inicializado");
    while (1);
  }

  myDFPlayer.volume(25);

  for (int i = 1; i <= 263; i++) {
    Serial.printf("▶ Reproduciendo %03d.mp3\n", i);
    myDFPlayer.play(i);
    delay(2500); // tiempo entre audios
  }
}

void loop() {}
