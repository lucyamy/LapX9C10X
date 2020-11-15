#ifndef LAPX9C10X_H
#define LAPX9C10X_H

#include "Arduino.h"

#define LAPX9C10X_X9C102       1
#define LAPX9C10X_X9C103       10
#define LAPX9C10X_X9C503       50
#define LAPX9C10X_X9C104       100

class LapX9C10X {
  public:
    LapX9C10X(uint8_t, uint8_t, uint8_t, float);
    void begin();
    void begin(int);
    void set(int);
    void set(float);
    void offset(int);
    void reset(int);
    uint8_t get(void);
    float getK(void);
    void writeNVM(void);

  private:
    void _setValue(uint8_t);
    void _setupChange(uint8_t);
    void _pulseInc(void);
    void _doChange(uint8_t);
    void _reset(bool);
    uint8_t _incrementPin;
    uint8_t _upDownPin;
    uint8_t _csNVRAMWritePin;
    uint8_t _counterValue;
    float _totalKOhms;
};

#endif
