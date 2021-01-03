#include <Arduino.h>

/*
  If someone is standing closer than 50 cm to the
  sensor, then we modify the state of the LED strip
*/
float max_distance = 50.0; 

/*
  Each index in the 2D array contains a pair of
  pin numbers. The first of the pins is the 
  echo pin, and the second is the trigger pin
*/
int hc_sr04_pins[4][4] = {
  {2, 3, 10, 0}, 
  {4, 5, 11, 0}, 
  {6, 7, 12, 0}, 
  {8, 9, 13, 0}
}; 

void setup() {

  // Initialize all pins
  for(uint8_t i = 0 ; i < sizeof(hc_sr04_pins) / sizeof(hc_sr04_pins)[0] ; i++) {
    pinMode(hc_sr04_pins[i][0], INPUT); 
    pinMode(hc_sr04_pins[i][1], OUTPUT);
    pinMode(hc_sr04_pins[i][2], OUTPUT);
  }

}

void loop() {
  for(uint8_t i = 0 ; i < sizeof(hc_sr04_pins) / sizeof(hc_sr04_pins)[0] ; i++) {
    
    // Extract variables from HC_SR04 array
    uint8_t echo_pin    = hc_sr04_pins[i][0];
    uint8_t trig_pin    = hc_sr04_pins[i][1]; 
    uint8_t fet_pin     = hc_sr04_pins[i][2]; 
    uint8_t brightness  = hc_sr04_pins[i][3]; 

    // Clear the trigger pin
    digitalWrite(trig_pin, LOW); 
    delayMicroseconds(2);

    // Set the trigger pin to HIGH for 10 μs
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Check to see if someone is standing with max_distance cm of the sensor
    if(pulseIn(echo_pin, HIGH) * 0.034 / 2 < max_distance) {
      brightness = (brightness < INT8_MAX) ? (brightness + 3) : brightness; 
    } else {
      brightness = (brightness > 0) ? (brightness - 1) : brightness;
    }

    // Set the brightness on the led strip
    analogWrite(fet_pin, brightness); 

    // Save brightness value
    hc_sr04_pins[i][3] = brightness;
  }
}