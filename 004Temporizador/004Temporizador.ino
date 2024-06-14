#include <LiquidCrystal.h>

LiquidCrystal pantalla(22, 19, 23, 18, 5, 17);
byte btnIncrementar = 2;
byte btnDecrementar = 15;
byte btnEnter = 4;

int valor = 0;
bool cronometroEnMarcha = false;

// Definición de los caracteres personalizados para la animación
byte frame0[8] = {
  B00100,
  B01110,
  B00100,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010
};

byte frame1[8] = {
  B00100,
  B01110,
  B00100,
  B00100,
  B01110,
  B01010,
  B00100,
  B10101
};

byte frame2[8] = {
  B00100,
  B01110,
  B00100,
  B00100,
  B01110,
  B00100,
  B10101,
  B01010
};

void setup() {
  Serial.begin(9600);
  delay(500);
  pantalla.begin(16, 2);
  pantalla.print("Temporizador");
  
  pinMode(btnIncrementar, INPUT);
  pinMode(btnDecrementar, INPUT);
  pinMode(btnEnter, INPUT);
  
  // Cargar los caracteres personalizados en el LCD
  pantalla.createChar(0, frame0);
  pantalla.createChar(1, frame1);
  pantalla.createChar(2, frame2);
}

void loop() {
  if (digitalRead(btnEnter) == HIGH) {
    while(digitalRead(btnEnter) == HIGH){}
    Serial.println("Se presionó enter");
    cronometroEnMarcha = !cronometroEnMarcha;
    delay(200); // Añadir un pequeño retraso para evitar múltiples lecturas rápidas
  }
  if (!cronometroEnMarcha) {
    if (digitalRead(btnIncrementar) == HIGH) {
      while(digitalRead(btnIncrementar) == HIGH){}
      valor++;
      actualizarPantalla();
      delay(200); // Añadir un pequeño retraso para evitar múltiples incrementos rápidos
    } else if (digitalRead(btnDecrementar) == HIGH) {
      while(digitalRead(btnDecrementar) == HIGH){}
      if (valor > 0) valor--; // Evitar que el valor sea negativo
      actualizarPantalla();
      delay(200); // Añadir un pequeño retraso para evitar múltiples decrementos rápidos
    }

  } else {
    if (valor > 0) {
      delay(1000); // Esperar un segundo
      valor--;
      actualizarPantalla();
      mostrarAnimacion();
    } else {
      cronometroEnMarcha = false;
      pantalla.setCursor(0, 1);
      pantalla.print("Tiempo cumplido!");
    }
  }
}

void actualizarPantalla() {
  pantalla.clear();
  pantalla.setCursor(0, 0);
  pantalla.print("Temporizador:");
  pantalla.setCursor(0, 1);
  pantalla.print(valor);
}

// Función para mostrar la animación
void mostrarAnimacion() {
  static int frame = 0;
  pantalla.setCursor(15, 0); // Ubicar en el borde derecho de la primera línea
  pantalla.write(frame); // Mostrar el frame actual de la animación
  frame = (frame + 1) % 3; // Cambiar al siguiente frame
}
