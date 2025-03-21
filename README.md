[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/de_libs)


# DE_tacho
лёгкая библиотека тахометра на аппаратных прерываниях

- доступно вычисление скорости (по радиусу колеса)
- учитывается передаточное число редуктора
- простая инициализация без ручной установки аппаратных прерываний

### подключение и инициализация
```cpp
#include <DE_tacho.h>
DE_tacho(uint8_t pin);            //пин энкодера
```

### использование
```cpp
begin(void (*isr)());             //указатель на обработчик прерываний
void detect();                    //обработчик прерываний
uint16_t getRPM();                //возвращает частоту, об/мин
void setRatio(uint16_t ratio);    //установка передаточного отношения редуктора
void setRadius(uint16_t radius);  //установка радиуса колеса, мм
float getSpeed();                 //возвращает скорость, м/с
void tick();                      //тикер для таймаута, не обязательно!
```
### пример
```cpp
#include <DE_tacho.h>

DE_tacho enc(2);

void setup() {
  Serial.begin(9600);
  enc.begin(isr);                //вызываем функцию с обработчиком прерываний
  enc.setRatio(8 * 63);
}

void isr() {
  enc.detect();                  //обработчик прерываний
}

void loop() {
  enc.tick();
  Serial.print(enc.getRPM());
  Serial.print('\t');
  Serial.println(enc.getSpeed());
}
```
