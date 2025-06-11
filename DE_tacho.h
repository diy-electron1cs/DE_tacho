#pragma once

#include <EnableInterrupt.h>

class DE_tacho {
  public:
    DE_tacho(uint8_t pin) : _pin(pin), _rot(0), _rpm(0), _radius(0.017f), _ratio(1),
      _lastCalcMillis(0), _nullTimer(0) {
      pinMode(_pin, INPUT_PULLUP);
    }

    void begin(void (*isr)()) {
      enableInterrupt(_pin, isr, RISING);
      _lastCalcMillis = millis();
      _nullTimer = _lastCalcMillis;
    }


    void detect() {
      _rot++;
      _nullTimer = millis();
    }

    void tick() {
      unsigned long now = millis();

      if (now - _nullTimer >= 1000) {
        _rpm = 0;
        _rot = 0;
      }

      if (now - _lastCalcMillis >= 100) {
        noInterrupts();
        unsigned int pulses = _rot;
        _rot = 0;
        interrupts();

        unsigned long dt = now - _lastCalcMillis;
        if (dt > 0) {
          // RPM = (импульсы / dt(ms)) * (60000 мс в минуте) / ratio
          _rpm = (unsigned long)((pulses * 60000UL) / dt) / _ratio;
        } else {
          _rpm = 0;
        }

        _lastCalcMillis = now;
      }
    }

    uint16_t getRPM() {
      noInterrupts();
      uint16_t rpmCopy = _rpm;
      interrupts();
      return rpmCopy;
    }

    void setRatio(uint16_t ratio) {
      if (ratio > 0) {
        _ratio = ratio;
      }
    }

    void setRadius(uint16_t radiusMm) {
      _radius = radiusMm / 1000.0f;
    }

    float getSpeed() {
      // Скорость (м/с) = RPM * радиус (м) * 2 * pi / 60
      const float conversionFactor = 2.0f * 3.1415926f / 60.0f;
      uint16_t rpmCopy = getRPM();
      return rpmCopy * _radius * conversionFactor;
    }

  private:
    uint8_t _pin;
    volatile unsigned int _rot;       // счётчик импульсов за период
    volatile unsigned int _rpm;       // последнее посчитанное RPM
    unsigned long _lastCalcMillis;    // время последнего расчёта
    unsigned long _nullTimer;         // таймер сброса RPM если нет импульсов
    uint16_t _ratio;
    float _radius;
};
