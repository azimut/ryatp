#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// PINES
#define LROJ   3
#define LVER   2
#define SERVO 11
#define TRIG   7
#define ECHO   6
#define POTE  A0
#define PUL1  10
#define PUL2   9

Servo servo;
LiquidCrystal_I2C lcd(0x20,16,2);

int codigo[4]  = { 3, 7, 1, 9}; // codigo objetivo
int ingreso[4] = {-1,-1,-1,-1}; // codigo ingresado
int intentos = 0; // cantidad de intentos hechos

void setup()
{
  Serial.begin(9600);
  pinMode(LROJ , OUTPUT);
  pinMode(LVER , OUTPUT);
  pinMode(SERVO, OUTPUT);
  pinMode(TRIG , OUTPUT);
  pinMode(ECHO , INPUT);
  pinMode(POTE , INPUT);
  pinMode(PUL1 , INPUT);
  pinMode(PUL2 , INPUT);
  lcd.init();
  lcd.backlight();
  servo.attach(SERVO);
  servo.write(0);
}

int leerDigito() {
  return map(analogRead(POTE), 0, 1023, 0, 9);
}

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

bool esCodigoCorrecto() {
  for(int i = 0; i < 4; i++) {
    if(codigo[i] != ingreso[i]) {
      return false;
    }
  }
  return true;
}

void encenderLedRojo()  { digitalWrite(LROJ, HIGH); }
void encenderLedVerde() { digitalWrite(LVER, HIGH); }
void apagarLedRojo()    { digitalWrite(LROJ, LOW); }
void apagarLedVerde()   { digitalWrite(LVER, LOW); }

void mostrar(String msg, int offset = 0) {
  lcd.clear();
  lcd.setCursor(offset, 0);
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

bool pulsadorDeConfirmacion() {
  bool estado = digitalRead(PUL1) == HIGH;
  while (digitalRead(PUL1)) {} // Esperar release
  return estado;
}
bool pulsadorDeCerrado() {
  bool estado = digitalRead(PUL2) == HIGH;
  while (digitalRead(PUL2)) {} // Esperar release
  return estado;
}

void loop()
{
  // ETAPA 1 - Despertar el sistema
  float distancia = 9999;
  while(distancia > 15) {
    mostrar("Acerca la mano", 1);
    apagarLedRojo();
    distancia = leerDistancia();
  }
  mostrar("Ingresa codigo:", 1);

  // ETAPA 2 - La perilla
  // ETAPA 3 - Ingresar la combinacion
  int voffset = 6; // offset horizontal de combinacion
  lcd.setCursor(voffset,1);
  lcd.print("----");
  for (int digito = 0; digito < 4; digito++) {
    bool confirmado = false;
    while (!confirmado) {
      lcd.noBlink(); // evitar parpadear fuera del digito
      lcd.setCursor(voffset+digito,1);
      lcd.print(leerDigito());
      lcd.setCursor(voffset+digito,1);
      lcd.blink();
      confirmado = pulsadorDeConfirmacion();
    }
    ingreso[digito] = leerDigito();
    lcd.setCursor(voffset+digito,1);
    lcd.print("*");
  }
  lcd.noBlink();
  delay(1000); // Pensando...

  // ETAPA 4 - Verificar y abrir
  if (esCodigoCorrecto()) {
    encenderLedVerde();
    mostrar("Abierto", 4);
    abrirCaja();
    // ETAPA 6 - Volver a cerrar
    while (!pulsadorDeCerrado()) { }
    cerrarCaja();
    apagarLedRojo();
    apagarLedVerde();
  } else {
    encenderLedRojo();
    mostrar("Incorrecto", 2);
    // ETAPA 5 - Tres intentos y bloqueo
    intentos++;
    lcd.print(" ");
    lcd.print(intentos);
    delay(2000); // Dejar leer el msg
    if (intentos == 3) {
      mostrar("Bloqueado: ", 2);
      for (int i = 10; i > 0; i--) {
        lcd.setCursor(strlen("Bloqueado: ") + 2, 0);
        lcd.print(i);
        lcd.print(" "); // HACK para limpiar un cuadrado
        delay(1000);
      }
      intentos = 0;
    }
  }

  // ETAPA ? - Cambiar la combinacion
  // ETAPA # - Registro de intentos
  // ETAPA ! - Contraseña con pista
}
