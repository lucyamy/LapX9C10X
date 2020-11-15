#include <LapX9C10X.h>

/*                                                                                                                                     
 * This example assumes you're using an X9C103 (10k) chip. For other variants,
 * Change the value in the line:
 *   LapX9C10X led(INCPIN, UDPIN, CSPIN, LAPX9C10X_X9C103);
 * to one of:
 * LAPX9C10X_X9C102       (1k)
 * LAPX9C10X_X9C103       (10k)
 * LAPX9C10X_X9C503       (50k)
 * LAPX9C10X_X9C104       (100k)
 *
 * The chip is connected to the Arduino like this:
 * 1 - !INC - pin 5
 * 2 - U/!D - pin 6
 * 7 - !CS  - pin 7
 * (! indicates that the thing following has a line over the top, called a bar.)
 *   
 * The other pins on the chip must be wired like this:
 * 8 - VCC - 5/3.3V
 * 3 - VH  - 5/3.3V
 * 4 - VSS - GND
 * 6 - VL  - GND
 * 5 - VW  - Output: 680 Ohm resistor to an LED, the other terminal
 *           of the LED connected to GND This value may seem high,
 *           but the maximum steady state wiper current of the chip
 *           is 4.4mA (8.8mA peak for 10 seconds). 680 Ohms - assuming
 *           a 5V supply, and an LED forward voltage drop of 2V, gives 
 *           us almost dead on 4.4mA worst case current (when the wiper
 *           is at the top).
 *           If you want to get the brightest you can, use this formula
 *           for the resistor value (in kilohms), where VCC is the 
 *           supply voltage, and VLED is the LED forward voltage drop:
 *           
 *                R = (VCC - VLED) 
 *                    ------------
 *                        4.4
 *
 * If you use different pins, just change the defines below
 *
 */

#define CSPIN 5
#define INCPIN 6
#define UDPIN 7


LapX9C10X led(INCPIN, UDPIN, CSPIN, LAPX9C10X_X9C103);

void setup() {
  Serial.begin(1152000);
  Serial.setTimeout(30000);
  Serial.println("Starting");  
  led.begin(99);
  delay(5000);
}

void loop() {
  uint8_t counter;
  float resistance;
  
  Serial.print("Enter counter value: ");
  counter = Serial.parseInt();
  Serial.println(counter);
  led.set(counter);
  Serial.print("Counter = ");
  Serial.print(led.get());
  Serial.print(", resistance = ");
  Serial.print(led.getK());
  Serial.println("KOhms");

  Serial.print("Enter resistance in kOhms: ");
  resistance = Serial.parseFloat();
  Serial.println(resistance);
  led.set(resistance);
  Serial.print("Counter = ");
  Serial.print(led.get());
  Serial.print(", resistance = ");
  Serial.print(led.getK());
  Serial.println("KOhms");
}
