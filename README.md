# LapX9C10X
An Arduino library to drive the X9C102, X9C103, X9C104, and X9C503 Digitally Controlled Potentiometers. The four members of the family have these resistances:
    • X9C102 = 1k
    • X9C103 = 10k
    • X9C503 = 50k
    • X9C104 = 100k
These chips are controlled by a three wire interface, consisting of:
    Up/Down (U/D)
    The U/D input controls the direction of the wiper movement
    and whether the counter is incremented or decremented.
    Increment (INC)
    The INC input is negative-edge triggered. Toggling INC will
    move the wiper and either increment or decrement the counter
    in the direction indicated by the logic level on the U/D input.
    Chip Select (CS)
    The device is selected when the CS input is LOW. The current
    counter value is stored in non-volatile memory when CS is
    returned HIGH while the INC input is also HIGH. After the store
    operation is complete the ISLX9C102, X9C103, X9C104,
    X9C503 device will be placed in the low power standby mode
    until the device is selected once again.

The library allows you to set resistance values either by using the counter values used internally by the chips (0 to 99), or by specifying the resistance value you want directly. (All resistances refer to the resistance between the low end of the potentiometer - the VH/RH pin, and the wiper - the VW/RW pin.)

**LapX9C10X** is invoked as:
 
    LapX9C10X(uint8_t incrementPin, uint8_t upDownPin, uint8_t selectNVRAMWritePin, float resistance);

Where the resistance parameter can be one of:
    LAPX9C10X_X9C102       (1 kOhm)
    LAPX9C10X_X9C103       (10 kOhm)
    LAPX9C10X_X9C503       (50 kOhm)
    LAPX9C10X_X9C104       (100 kOhm)
or can be the actual resistance of the potentiometer, as measured between VH/RH  and VL/RL (when the chip is powered on). Using the actual resistance will of course give more accurate results, as the tolerance of the parts is ±20%.
The chips contain non-volatile memory, and can be programmed to power up to any of the 100 available wiper positions. **LapX9C10X** has a function to save to this memory.

When using **LapX9C10X** functions that are passed a 0-99 wiper position, or a 0-max resistance, any value that is higher than the maximum is changed to the maximum.
The functions provided by **LapX9C10X** are:

void begin();
-------------
void begin(int);
----------------
This must be called before using any other function. If called with no argument, it sets the wiper to 99 – the maximum. If called with any negative value, it sets the wiper to 0 – the minimum. Any value from 0-99 tells **LapX9C10X** to assume the power up value to be that number, and it will not change it. Use this with care, as if you give it an incorrect value, all operations you then perform will also be incorrect.

void set(int wiper);
--------------
void set(float resistance);
----------------
If passed an integer, sets the wiper to that value (0-99).
If passed a float, sets the wiper to the position that is closest to the resistance given.

void offset(int change);
-----------------
Moves the wiper ‘change’ amount – that is, it adds or subtracts ‘change’ from the wiper position.

void reset(int wiper);
----------------
Forces the wiper to the value given, ignoring **LapX9C10X**s idea of where the wiper currently is. It does this by first forcing the wiper to the end nearest the desired position, then moving it from there.

uint8_t get();
--------------
Returns the current wiper position.

float getK();
-------------
Returns the resistance of the current wiper position.

void writeNVM();
----------------
Writes the current wiper position to the chip’s non-volatile memory.


