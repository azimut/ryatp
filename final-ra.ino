#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define HOFFSET 6 /* offset horizontal en el lcd, para la combinacion */

#define LROJ 3
#define LVER 2
#define SERVO 11
#define TRIG 7
#define ECHO 6
#define POTE A0
#define PUL1 10
#define PUL2 9

Servo servo;
LiquidCrystal_I2C lcd(0x20, 16, 2);

int codigo[4] = {3, 7, 1, 9};      // codigo objetivo
int ingreso[4] = {-1, -1, -1, -1}; // codigo ingresado
int intentos = 0;                  // cantidad de intentos hechos

void setup() {
  Serial.begin(9600);
  pinMode(LROJ, OUTPUT);
  pinMode(LVER, OUTPUT);
  pinMode(SERVO, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(POTE, INPUT);
  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
  lcd.init();
  lcd.backlight();
  servo.attach(SERVO);
  servo.write(0);
}

int leerDigito() { return map(analogRead(POTE), 0, 1023, 0, 9); }

float leerDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  float duracion = pulseIn(ECHO, HIGH, 30000);
  float distancia = duracion * 0.034 / 2;
  return distancia;
}

void actualizarCodigo() {
  mostrar("Nuevo Codigo:");
  lcd.setCursor(HOFFSET, 1);
  lcd.print("----");
  for (int digito = 0; digito < 4; digito++) {
    bool confirmado = false;
    while (!confirmado) {
      lcd.noBlink(); // evitar parpadear fuera del digito
      lcd.setCursor(HOFFSET + digito, 1);
      lcd.print(leerDigito());
      lcd.setCursor(HOFFSET + digito, 1);
      lcd.blink();
      confirmado = pulsadorDeConfirmacion();
      while (pulsadorDeConfirmacion()) { } // hasta que se suelte
    }
    codigo[digito] = leerDigito();
    lcd.print("*");
  }
  lcd.noBlink();
  delay(1000); // Pensando...
  mostrar("OK!");
  delay(1000); // Pensando...
}

bool esCodigoCorrecto() {
  for (int i = 0; i < 4; i++) {
    if (codigo[i] != ingreso[i]) {
      return false;
    }
  }
  return true;
}

void encenderLedRojo()  { digitalWrite(LROJ, HIGH); }
void encenderLedVerde() { digitalWrite(LVER, HIGH); }
void apagarLedRojo()    { digitalWrite(LROJ, LOW); }
void apagarLedVerde()   { digitalWrite(LVER, LOW); }
void respirarLedRojo() { // (255 + 255) * 2 = 1020ms
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 255; j++) {
      analogWrite(LROJ, j);
      delay(1);
    }
    for (int j = 255; j >= 0; j--) {
      analogWrite(LROJ, j);
      delay(1);
    }
  }
}

void mostrar(String msg) {
  lcd.clear();
  lcd.setCursor((16 / 2) - (msg.length() / 2), 0);
  lcd.print(msg);
}

void cerrarCaja() {
  for (int i = 90; i > 0; i--) {
    servo.write(i);
  }
}
void abrirCaja() {
  for (int i = 0; i < 90; i++) {
    servo.write(i);
  }
}

bool pulsadorDeConfirmacion() { return digitalRead(PUL1); }
bool pulsadorDeCerrado() { return digitalRead(PUL2); }

void loop() {
  // ETAPA 1 - Despertar el sistema
  float distancia = leerDistancia();
  while (distancia > 15) {
    mostrar("Acerca la mano");
    apagarLedRojo();
    distancia = leerDistancia();
  }
  mostrar("Ingresa codigo:");
  lcd.setCursor(HOFFSET, 1);
  lcd.print("----");

  // ETAPA 2 - La perilla
  // ETAPA 3 - Ingresar la combinacion
  for (int digito = 0; digito < 4; digito++) {
    bool confirmado = false;
    while (!confirmado) {
      lcd.noBlink(); // evitar parpadear fuera del digito
      lcd.setCursor(HOFFSET + digito, 1);
      lcd.print(leerDigito());
      lcd.setCursor(HOFFSET + digito, 1);
      lcd.blink();
      confirmado = pulsadorDeConfirmacion();
      while (pulsadorDeConfirmacion()) { } // hasta que se suelte
    }
    ingreso[digito] = leerDigito();
    lcd.print("*");
  }
  lcd.noBlink();
  delay(1000); // Pensando...

  // ETAPA 4 - Verificar y abrir
  if (esCodigoCorrecto()) {
    encenderLedVerde();
    mostrar("Abierto");
    abrirCaja();
    // ETAPA 6 - Volver a cerrar
    // ETAPA 7 - Cambiar la combinacion
    while (!pulsadorDeCerrado()) { } // esperar hasta que se precione
    if (pulsadorDeCerrado() && pulsadorDeConfirmacion()) {
      while (pulsadorDeCerrado() || pulsadorDeConfirmacion()) { }
      actualizarCodigo();
    } else {
      while (pulsadorDeCerrado()) { } // esperar que se suelte
      cerrarCaja();
      apagarLedRojo();
      apagarLedVerde();
      intentos = 0; // contar solo reintentos consecutivos
    }
  } else {
    encenderLedRojo();
    mostrar("Incorrecto");
    // ETAPA 5 - Tres intentos y bloqueo
    intentos++;
    lcd.setCursor(6, 1);
    lcd.print(intentos);
    lcd.print("/3");
    delay(2000); // Dejar leer el msg
    if (intentos == 3) {
      mostrar("Bloqueado: ");
      for (int i = 10; i > 0; i--) {
        lcd.setCursor(7, 1);
        lcd.print(i);
        lcd.print(" "); // HACK para limpiar un cuadrado
        respirarLedRojo();
      }
      intentos = 0;
    }
  }
}
