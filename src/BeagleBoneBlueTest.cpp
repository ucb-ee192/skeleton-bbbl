#include <stdio.h>
#include <robotcontrol.h>

int main() {
  // Initialize the Pause (PAU) button
  rc_button_init(RC_BTN_PIN_PAUSE, RC_BTN_POLARITY_NORM_HIGH,
      RC_BTN_DEBOUNCE_DEFAULT_US);

  // A quick printf as a sanity check
  // Note, the \n is necessary since the buffer flushes on a line break
  printf("Hello World!\n");

  // A quick 0.5s LED pulse as an indicator the LED works
  rc_led_set(RC_LED_GREEN, 1);
  rc_usleep(500*1000);
  rc_led_set(RC_LED_GREEN, 0);

  uint32_t i = 0;
  while (1) {
    // This simple code sets the LED equal to the state of the Pause (PAU) button.
    // Note that rc_button_get_state returns 1 when pressed.
    rc_led_set(RC_LED_RED, 1);
    rc_usleep(500*1000);
    rc_led_set(RC_LED_RED, 0);

    printf("Iteration %i\n", i);
    rc_usleep(500*1000);  // Limit console spam
    i++;
  }

  rc_led_cleanup();
  rc_button_cleanup();

  return 0;
}
