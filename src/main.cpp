#include <stdio.h>
#include <robotcontrol.h>

// Headers for nonblocking stdin + console
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#include "telemetry/telemetry.h"


int main() {
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);  // set stdin (file descriptor 0) to nonblocking mode

  // Initialize the Pause (PAU) and mode (MOD) buttons
  rc_button_init(RC_BTN_PIN_PAUSE, RC_BTN_POLARITY_NORM_HIGH,
      RC_BTN_DEBOUNCE_DEFAULT_US);
  rc_button_init(RC_BTN_PIN_MODE, RC_BTN_POLARITY_NORM_HIGH,
      RC_BTN_DEBOUNCE_DEFAULT_US);

  // Create teleemtry object and open listen socket
  telemetry::SocketsHal telemetrySocket;
  telemetrySocket.init();
  telemetry::Telemetry telemetryObj(telemetrySocket);

  telemetry::Numeric<uint32_t> tTimeMs(telemetryObj, "time", "Time", "ms", 0);
  telemetry::Numeric<uint8_t> tBtnPause(telemetryObj, "btn_pause", "Pause Button", "raw", 0);
  telemetry::Numeric<uint8_t> tBtnMode(telemetryObj, "btn_mode", "Mode Button", "raw", 0);
  telemetry::Numeric<uint32_t> tLedBlinkDelay(telemetryObj, "led_blink_delay", "LED Blink Delay", "ms", 500);

  // A quick printf as a sanity check
  // Note, the \n is necessary since the buffer flushes on a line break
  printf("Hello World!\n");

  // A quick 0.5s LED pulse as an indicator the LED works
  rc_led_set(RC_LED_GREEN, 1);
  rc_usleep(500*1000);
  rc_led_set(RC_LED_GREEN, 0);

  uint32_t i = 0;
  while (1) {
    // Process nonblocking console IO
    char buf[128];  // buffer to hold bytes read from console
    int numRead = read(0, buf, 128);  // returns number of bytes read, up to 128
    while (numRead > 0 && (buf[numRead - 1] == '\n' || buf[numRead - 1] == '\r')) {
      numRead--;  // strip trailing newline characters
    }
    if (numRead > 0) {  // if read returned something that was "nonempty"
      buf[numRead] = '\0';  // read() doesn't attach a null terminator, which makes strcmp and print happier
      if (strcmp(buf, "quit") == 0) {
        break;  // break out of the main loop, so the cleanup code can run
      } else {  // you might add more commands here
        printf("unknown command '%s'\n", buf);
      }
    }
    
    //
    // YOUR MAIN LOOP CODE HERE
    //
    tTimeMs = (uint32_t)(rc_nanos_since_boot() / 1000 / 1000);
    tBtnPause = rc_button_get_state(RC_BTN_PIN_PAUSE);
    tBtnMode = rc_button_get_state(RC_BTN_PIN_MODE);

    if (telemetrySocket.check_connect()) {
      telemetryObj.transmit_header();
    }
    telemetryObj.do_io();
    
    // This simple code blinks the green LED at a user-defined rate.
    rc_led_set(RC_LED_GREEN, 1);
    rc_usleep(tLedBlinkDelay*1000);
    rc_led_set(RC_LED_GREEN, 0);

    printf("Iteration %i\n", i);
    rc_usleep(tLedBlinkDelay*1000);  // Limit console spam
    i++;
  }

  telemetrySocket.cleanup();
  rc_led_cleanup();
  rc_button_cleanup();

  return 0;
}
