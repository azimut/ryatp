# TP Final: Caja Fuerte Electronica

Para el curso de **Robotica y Automatizacion 2026 2C**. Dictado en el CFL 403 por Eliam Chociananovicz.

## Descripcion

El trabajo consistia en desarrollar una caja fuerte electrónica utilizando Arduino, integrando distintos componentes electrónicos para controlar su funcionamiento. Elegi el desafio extra de implementar la funcionalidad de poder cambiar la combinacion una vez abierta.

## Circuito en Tinkercad

![Circuito en Tinkercad](./circuit.png)

## Arduino Pinout

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

La lista `codigo` almacena la combinacion por defecto (3719). La lista `ingreso` almacena el input provisto por el usuario.

### ¿Que casos probaste para asegurarte de que la caja fuerte no se abra con una combinacion incorrecta?

- Probe una combinacion incorrecta
- Probe una combinacion correcta, cerrar, y despues una combinacion incorrecta
- Probe una combinacion vieja despues de cambiarla
