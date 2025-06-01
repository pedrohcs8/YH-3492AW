#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// LED PANNEL PINOUT
// TOP 4
// TOP R 5
// BOTTOM R 6
// BOTTOM 7
// BOTTOM L 8
// TOP L 10
// MIDDLE 11
//
int segmentPinArray[7] = { 4, 5, 6, 7, 8, 10, 11 }; // Segment Order
int displayPinArray[4] = { 1, 3, 2, 0 }; // Number order
// Number Combination array
uint16_t displayNumberArray[10] = { 0x3F, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x67 };

uint32_t delay;
int currentNumber = -1;

  int numberArray[4];

void configureNumber(int number) {

  // get single digits
  for (int i = 0; i < 4; i++)
  {
      numberArray[i] = number % 10;
      number /= 10;
  }

  // reverse the array
  for(int i = i; i<4/2; ++i)
  {
      int temp = numberArray[i];
      numberArray[i] = numberArray[4-i-1];
      numberArray[4-i-1] = temp;
  }
}

int main() {
  for (int i = 0; i < 13; i++) {
    gpio_init(i);
    gpio_set_dir(i, GPIO_OUT);
  }

  for (int x = 0; x < 4; x++) {
    gpio_put(x, true);
  }

  for (int x = 4; x < 13; x++) {
    gpio_put(x, false);
  }


  while (true) {
    // select the segment
    for (int i = 0; i < 4; i++) {
      gpio_put(displayPinArray[i], false);

      // select the number in the array
        uint16_t segments = displayNumberArray[numberArray[i]];

        // select the segments
        for (int x = 0; x < 8; x++) {
          bool segment = (segments >> x) & 1;
          gpio_put(segmentPinArray[x], segment);
        }

      sleep_us(500);
      gpio_put(displayPinArray[i], true);
    }

    if (delay < to_ms_since_boot(get_absolute_time())) {
      configureNumber(currentNumber);
      currentNumber++;
      delay = to_ms_since_boot(get_absolute_time()) + 2000;
    }
  }

  return 0;
}
