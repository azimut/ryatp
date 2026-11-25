# TP Arduino Caja Fuerte Electronica

TP Final del curso de **Robotica y Automatizacion 2026**. Dictado en el CFL 403 por Eliam Chociananovicz.

## Circuito en Tinkercad

![Circuito en Tinkercad](./circuit.png)

## Arduino Pinout

Respeta el pinout recomendado en el PDF.

| Componente      | Pin |
|-----------------|-----|
| Led Rojo        | 5   |
| Led Verde       | 2   |
| Servo           | 9   |
| HC-SR04 Trigger | 7   |
| HC-SR04 Echo    | 8   |
| Potenciometro   | A0  |
| Pulsador 1      | 12  |
| Pulsador 2      | 13  |
| LCD SDA         | A4  |
| LCD SCL         | A5  |

## Preguntas y Respuestas

### ¿Que etapa te resulto mas dificil y por que?

Me resulto dificil diferenciar las etapas que no necesariamente ocurrian en orden del enunciado Etapas 4,5,6.

### ¿Para que usaste cada lista?

La lista `codigo` almacena la contraseña por defecto.
La lista `ingreso` almacena el input provisto por el usuario.

### ¿Que casos probaste para asegurarte de que la caja fuerte no se abra con una combinacion incorrecta?

- Probe una combinacion incorrecta
- Probe una combinacion correcta, cerrar, y despues una combinacion incorrecta
- Probe una combinacion vieja despues de cambiarla
