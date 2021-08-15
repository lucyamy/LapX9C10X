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
 * 1 - !INC - pin 6
 * 2 - U/!D - pin 7
 * 7 - !CS  - pin 5
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
  Serial.begin(115100);
  Serial.println("Starting, set to minimum resistance");  
  led.begin(-1);
  delay(5000);
}

void loop() {
  int counter;
  float resistance;
  
  Serial.println("Using absolute counter changes");  
  for(counter = 0; counter < 100; counter++) {
    Serial.print("Increasing, counter = ");
    Serial.print(counter);
    led.set(counter);
    Serial.print(", new resistance = ");
    Serial.print(led.getK());
    Serial.println("KOhms");
    delay(100);
  }
  for(counter = 99; counter >= 0; counter--) {
    Serial.print("Decreasing, counter = ");
    Serial.print(counter);
    led.set(counter);
    Serial.print(", new resistance = ");
    Serial.print(led.getK());
    Serial.println("KOhms");
    delay(100);
  }

  Serial.println("Using relative counter changes");  
  // pot is set at 0 from above, which is handy :)
  for(counter = 0; counter < 100; counter++) {
    Serial.print("Increasing, counter = ");
    Serial.print(counter);
    led.offset(+1);
    Serial.print(", new resistance = ");
    Serial.print(led.getK());
    Serial.println("KOhms");
    delay(100);
  }
  for(counter = 99; counter >= 0; counter--) {
    Serial.print("Decreasing, counter = ");
    Serial.print(counter);
    led.offset(-1);
    Serial.print(", new resistance = ");
    Serial.print(led.getK());
    Serial.println("KOhms");
    delay(100);
  }

  Serial.println("Using absolute resistance changes");  
  for(resistance = 0; resistance < 10; resistance += 0.1) {
    Serial.print("Increasing, resistance = ");
    Serial.print(resistance);
    led.set(resistance);
    Serial.print(", new resistance = ");
    Serial.print(led.getK());
    Serial.println("KOhms");
    delay(100);
  }
  for(resistance = 10.0; resistance >= 0; resistance -= 0.1) {
    Serial.print("Decreasing, resistance = ");
    Serial.print(resistance);
    led.set(resistance);
    Serial.print(", new resistance = ");
    Serial.print(led.getK());
    Serial.println("KOhms");
    delay(100);
  }
}
