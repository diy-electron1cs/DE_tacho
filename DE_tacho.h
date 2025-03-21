#pragma once

#define VACUUM_CLEANER_MOTOR 504

class tacho {
  public:
    tacho(int pin) : _pin(pin) {
      pinMode(_pin, INPUT_PULLUP);
    }

    void begin(void (*isr)()) {
      attachInterrupt(digitalPinToInterrupt(_pin), isr, RISING);
    }

    void detect() {
      _rot++;
      _dt = millis() - _tm;
      if (_dt >= 100) {
        _rpm = (_rot * 60000 / _dt) / (_ratio);
        _rot = 0;
        _tm = millis();
      }
      _nullTimer = millis();
    }

    uint16_t getRPM() {
      return _rpm;
    }

    void setRatio(uint16_t ratio) {
      _ratio = ratio;
    }

    void setRadius(uint16_t radius) {
      _radius = radius / 1000;
    }

    float getSpeed() {
      const float conversionFactor = 2 * 3.14159f / 60.0f;
      _spd = _rpm * _radius * conversionFactor;
      return _spd;
    }


    void tick() {
      if (millis() - _nullTimer >= 100) {
        _rpm = 0;
      }
    }

  private:
    uint8_t _pin;
    unsigned int _rot = 0;
    unsigned long int _tm;
    unsigned long int _rpm = 0;
    float _spd;
    unsigned int _dt = 0;
    uint32_t _nullTimer;
    uint16_t _ratio = 1;
    float _radius = 0.017;
};