#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define LROJ   3
#define LVER   2
#define ECHO   6
#define TRIG   7
#define PUL1  10
#define PUL2   9
#define SERVO 11
#define POTE  A0

Servo servo;
LiquidCrystal_I2C lcd(0x20,16,2);

int codigo[4]  = { 3, 7, 1, 9};
int ingreso[4] = {-1,-1,-1,-1};

void setup()
{
  Serial.begin(9600);
  pinMode(LROJ, OUTPUT);
  pinMode(LVER, OUTPUT);
  pinMode(POTE, INPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(SERVO, OUTPUT);
  pinMode(PUL1, INPUT);
  pinMode(PUL2, INPUT);
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

void encenderLedRojo() {
  digitalWrite(LROJ, HIGH);
}
void encenderLedVerde() {
  digitalWrite(LVER, HIGH);
}

void mostrar(String msg) {
  lcd.clear();
  lcd.print(msg);
}

void cerrarCaja() {
  for (int i = 90; i < 0; i--) {
    servo.write(i);
  }
}

void abrirCaja() {
  for (int i = 0; i < 90; i++) {
    servo.write(i);
  }
}

bool pulsadorConfirmar() {
  return digitalRead(PUL1);
}
bool pulsadorCerrado() {
  return digitalRead(PUL2);
}

void reset() {
  digitalWrite(LROJ, LOW);
  digitalWrite(LVER, LOW);
  lcd.clear();
  servo.write(0);
}

void loop()
{
  // ETAPA 1 - Despertar el sistema
  reset();
  mostrar("Acerca la mano");
  float distancia = 9999;
  while(distancia > 15) {
    distancia = leerDistancia();
  }
  mostrar("Ingresa codigo:");

  // ETAPA 2 - La perilla
  // ETAPA 3 - Ingresar la combinacion
  for (int digito = 0; digito < 4; digito++) {
    bool confirmado = false;
    while (!confirmado) {
      lcd.noBlink(); // evitar parpadear fuera del digito
      lcd.setCursor(digito,1);
      lcd.print(leerDigito());
      lcd.setCursor(digito,1);
      lcd.blink();
      confirmado = pulsadorConfirmar();
      while(pulsadorConfirmar()) {} // esperar release...
    }
    ingreso[digito] = leerDigito();
    lcd.setCursor(digito,1);
    lcd.print("*");
  }
  lcd.noBlink();
  delay(3000); // FIXME
  // ETAPA 4 - Verificar y abrir
  if (esCodigoCorrecto()) {
    encenderLedVerde();
    abrirCaja();
    mostrar("Abierto");
  } else {
    encenderLedRojo();
    mostrar("Incorecto");
    // TODO: retry
  }
  // ETAPA 5 - Tres intentos y bloqueo


  // ETAPA 6 - Volver a cerrar
  while (!pulsadorCerrado()) { }
  cerrarCaja();
  reset();
  // ETAPA ? - Cambiar la combinacion
  // ETAPA # - Registro de intentos
  // ETAPA ! - Contraseña con pista
}
