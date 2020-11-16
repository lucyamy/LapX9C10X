#include "Arduino.h"
#include "LapX9C10X.h"

#define LAPX9C10X_ASSERT       LOW
#define LAPX9C10X_DEASSERT     HIGH
#define LAPX9C10X_DOWN         LAPX9C10X_ASSERT
#define LAPX9C10X_UP           LAPX9C10X_DEASSERT
#define LAPX9C10X_MAX          true
#define LAPX9C10X_MIN          false
#define cToK(c) ((float) (c) / 99.0 * _totalKOhms)
#define kToC(r) ((uint8_t) (((r) * 99.0 / _totalKOhms) + 0.5))

//
// Public functions
//

//
// Constructor
//

LapX9C10X::LapX9C10X(uint8_t incrementPin, uint8_t upDownPin, uint8_t selectNVRAMWritePin,
                     float resistance) {
  _incrementPin = incrementPin;
  _upDownPin = upDownPin;
  _csNVRAMWritePin = selectNVRAMWritePin;  
  _totalKOhms = resistance;
}

//
// begin(). Must be called first, before using other calls, or behaviour is undefined.
//

//
// begin() function for when the current counter value is unknown.
//

void LapX9C10X::begin() {
  pinMode(_incrementPin, OUTPUT);
  pinMode(_upDownPin, OUTPUT);
  pinMode(_csNVRAMWritePin, OUTPUT);
  // !INC will be high by default, after the pinMode() in the contructor.
  // We might as well take it low before deasserting !CS - which should also
  // be high at this point - or the chip will save the current value to
  // non-volatile memory.
  digitalWrite(_incrementPin, LAPX9C10X_ASSERT);
  digitalWrite(_csNVRAMWritePin, LAPX9C10X_DEASSERT);
  // Push resistance to max - note that very low values may occur as
  // we do this, due to the internal operation of the chip.
  reset(LAPX9C10X_MAX);
}

//
// begin() function which is passed current counter value.
// It's up to the caller to make sure this is correct.
// Passing any negative integer will force the counter to 0.
//

void LapX9C10X::begin(int value) {
  pinMode(_incrementPin, OUTPUT);
  pinMode(_upDownPin, OUTPUT);
  pinMode(_csNVRAMWritePin, OUTPUT);
  // !INC will be high by default, after the pinMode() in the contructor.
  // We might as well take it low before deasserting !CS - which should also
  // be high at this point - or the chip will save the current value to
  // non-volatile memory.
  digitalWrite(_incrementPin, LAPX9C10X_ASSERT);
  digitalWrite(_csNVRAMWritePin, LAPX9C10X_DEASSERT);
  // Set the counter to the value we were given, and hope for the best,
  // or force it to 0.
  if(value < 0) reset(LAPX9C10X_MIN);
  else _counterValue = value > 99 ? value : 99;
}

//
// Set a counter value directly (absolute change)
//

void LapX9C10X::set(int value) {
  _setValue(value);
}


//
// Set a resistance directly (absolute change)
//

void LapX9C10X::set(float value) {
  _setValue(kToC(value));
}

//
// Get the current value
//

uint8_t LapX9C10X::get(void) {
  return _counterValue;
}

//
// Get the current resistance
//

float LapX9C10X::getK(void) {
  return cToK(_counterValue);
}

//
// Increase/decrease by an amount (counter) (relative change)
//

void LapX9C10X::offset(int value) {
  int v;
  v = _counterValue + value;
  v = v < 0 ? 0 : v;
  _setValue(v);
}

//
// Reset the counter value directly (absolute change)
//

void LapX9C10X::reset(int value) {
  if(value > 49) _reset(LAPX9C10X_MIN);
  else _reset(LAPX9C10X_MAX);
  _setValue(value);
}


//
// Write current setting to non-volatile memory
//

void LapX9C10X::writeNVM(void) {
  // Select the chip
  // Mustn't assert !CS with !INC asserted.
  digitalWrite(_incrementPin, LAPX9C10X_DEASSERT);
  digitalWrite(_csNVRAMWritePin, LAPX9C10X_ASSERT);
  // The value of the counter is stored in non-volatile memory whenever !CS transitions
  // high while the !INC input is also high.
  delayMicroseconds(1);
  digitalWrite(_csNVRAMWritePin, LAPX9C10X_DEASSERT);
  // The chip is now saving the counter value. As noted below, this is slow. 20 whole
  // milleseconds of slow, as compared to 100 nanoseconds deselect time if not storing.
  delayMicroseconds(20000);
  // Put the !INC input low again, Now we're back to normal.
  digitalWrite(_incrementPin, LAPX9C10X_ASSERT);
}

//
// Private functions
//

//
// Step the chip counter to the desired value
//

void LapX9C10X::_setValue(uint8_t value) {
  uint8_t steps;
  value = value > 99 ? 99 : value;
  if(value > _counterValue) {
    _setupChange(LAPX9C10X_UP);
    // Get number of steps
    steps = value - _counterValue;
  } else if(value < _counterValue) {
    _setupChange(LAPX9C10X_DOWN);
    // Get number of steps
    steps = _counterValue - value;
  } else {
    // Nothing to do
    return;
  }
  // Set the requested value by incrementing or decrementing from the current value.
  _doChange(steps);
  _counterValue = value;
}

//
// Prepare the chip before changing the counter.
//

void LapX9C10X::_setupChange(uint8_t direction) {
  // Set the direction pin for up/down
  digitalWrite(_upDownPin, direction);
  // Mustn't assert !CS with !INC asserted.
  // The datasheet says goblins appear if you do.
  // Well, "(not recommended)" at least. It's also
  // worth noting that if you do do it, it moves the
  // wiper. I've seen so many bits of code that miss that.
  digitalWrite(_incrementPin, LAPX9C10X_DEASSERT);
  // Select the chip
  digitalWrite(_csNVRAMWritePin, LAPX9C10X_ASSERT);
}

//
// Pulse !INC, 1us high-going.
//

void LapX9C10X::_pulseInc() {
  // We send a high-going pulse, the chip triggers on the falling edge.
  // We're one step closer
  digitalWrite(_incrementPin, LAPX9C10X_DEASSERT);
  delayMicroseconds(1);
  digitalWrite(_incrementPin, LAPX9C10X_ASSERT);
  delayMicroseconds(1);
}

//
// Send a string of pulses on !INC, then sanely deselect the chip.
//

void LapX9C10X::_doChange(uint8_t steps) {
  for(uint8_t i = 0; i < steps; i++)
    _pulseInc();
  // We leave !INC low after the pulseInc() call. That may seem strange,
  // but !INC _MUST_ be low before deasserting !CS, or the chip will save
  // the current value to non-volatile memory. This will wear out if we
  // abuse it. It is also slooooooow as f**k.
  digitalWrite(_csNVRAMWritePin, LAPX9C10X_DEASSERT);
}

void LapX9C10X::_reset(bool minMax) {
  if(minMax == LAPX9C10X_MIN) {
    _counterValue = 99;
    _setValue(0);
  } else {
    _counterValue = 0;
    _setValue(99);
  }
}

