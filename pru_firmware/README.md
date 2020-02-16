Code for Programmable Realtime Unit (coprocessor)

## Components
### PRU0: Linescan
PRU0 is the line camera code, generating the SI/CLK signals and sampling the AO from the camera, written in C.
As currently written, it spends 100ms at the beginning pulsing the red LED to indicate a successful boot, then drives the red LED while it is reading the camera.

Partial register definitions (at least what is directly used) for the GPIO and clock are provided.
Register definitions for the ADC can be found on the BeagleBoneBlue at `/usr/lib/ti/pru-software-support-package/include/am335x/sys_tscAdcSs.h`.

Note: PRU0 originally was used by librobotcontrol for the 4th encoder - and since we're repurposing PRU0, only the three eQEP-based encoders can be used.

### PRU1: Servo
This is the unmodified servo PRU code from [librobotcontrol](https://github.com/StrawsonDesign/librobotcontrol), written in assembly.

## Building
On the BeagleBoneBlue, run:
```
make
```
which will place the built firmware objects in `build/fw`.

To install the built firmware (so it is accessible from `rc_pru_start`), run:
```
make install
```
which will copy the built firmware to `/lib/firmware`.
