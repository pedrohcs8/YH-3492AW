#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

volatile int segmentCounter = 4; // Digit Counter
volatile int displayCounter = 0; // Segment Counter

int delay = 200;
uint32_t btn1Timer;
uint32_t btn2Timer;
volatile int lastDisplayPin = 0;
volatile int lastSegPin = 4;

void gpio_callback(uint gpio, uint32_t events) {
  if (gpio == 15) {
    if (btn1Timer < to_ms_since_boot(get_absolute_time())) {
      lastDisplayPin = displayCounter;
      displayCounter++;
      if (displayCounter > 3) {
        displayCounter = 0;
      }

      btn1Timer = to_ms_since_boot(get_absolute_time()) + delay;
    }
  } else if (gpio == 16) {
    if (btn2Timer < to_ms_since_boot(get_absolute_time())) {
      lastSegPin = segmentCounter;
      segmentCounter++;
      if (segmentCounter > 12) {
        segmentCounter = 4;
      }

      btn2Timer = to_ms_since_boot(get_absolute_time()) + delay;
    }
  }
}

int main() {
  for (int i = 0; i < 13; i++) {
    gpio_init(i);
    gpio_set_dir(i, GPIO_OUT);
  }

  gpio_init(15);
  gpio_init(16);
  gpio_set_dir(15, GPIO_IN);
  gpio_set_dir(16, GPIO_IN);
  gpio_pull_up(15);
  gpio_pull_up(16);

  gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(16, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

  for (int x = 0; x < 4; x++) {
    gpio_put(x, true);
  }

  for (int x = 4; x < 13; x++) {
    gpio_put(x, false);
  }

  while (true) {
    gpio_put(lastDisplayPin, true);
    gpio_put(displayCounter, false);

    gpio_put(lastSegPin, false);
    gpio_put(segmentCounter, true);
  }

  return 0;
}
