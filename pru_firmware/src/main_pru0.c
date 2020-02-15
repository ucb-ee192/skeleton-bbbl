#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>
#include "resource_table_pru1.h"


// Register map headers
#include <sys_tscAdcSs.h>  // ADC - in /usr/lib/ti/pru-software-support-package/include/am335x/sys_tscAdcSs.h
#include "cm_wkup.h"
#include "gpio.h"


// Shared memory definitions
#define ENCODER_MEM_OFFSET  16
#define CAM_MEM_OFFSET  (ENCODER_MEM_OFFSET + 1)
#define PRU_SHAREDMEM ((volatile unsigned int *)(0x10000)) // shared memory with Cortex A8 (Linux)


// IO definitions
#define LED_BATT1 (1<<27)  // (red) on GPIO0 - not recommended since Linux overwrites this regularly
#define LED_BATT2 (1<<11)  // (green) on GPIO0 - not recommended since Linux overwrites this regularly
#define LED_BATT3 (1<<29)  // (green) on GPIO1 - not recommended since Linux overwrites this regularly
#define LED_BATT4 (1<<26)  // (green) on GPIO0 - not recommended since Linux overwrites this regularly

#define LED_RED (1<<2)  // on GPIO2
#define LED_GRN (1<<3)  // on GPIO3

#define LED_USR0 (1<<21)  // (blue) on GPIO1
#define LED_USR1 (1<<22)  // (blue) on GPIO1
#define LED_USR3 (1<<24)  // (blue) on GPIO1

#define LED_BUSY_GPIO GPIO2
#define LED_BUSY_IO LED_RED

#define CAM_SI_GPIO GPIO3
#define CAM_SI_IO (1 << 17)
#define CAM_CLK_GPIO GPIO3
#define CAM_CLK_IO (1 << 20)
#define CAM_AO_INP = 0  // ADC INP (positive in) channel for camera in


// ADC helper prototypes
void adc_init();
uint16_t adc_read();


// The main (top-level) PRU code
int main() {
  // Pulse the LED on quickly to indicate startup.
  // You may want to remove this code so the main camera read code start immediately.
  LED_BUSY_GPIO.SETDATAOUT = LED_BUSY_IO;
  adc_init();
  CAM_SI_GPIO.OE &= (~CAM_SI_IO) & (~CAM_CLK_IO);  // set the SI and CLK pins as output (we assume LED directions set by Linux)
  CAM_SI_GPIO.CLEARDATAOUT = CAM_SI_IO;  // "initialize" SI low
  CAM_CLK_GPIO.CLEARDATAOUT = CAM_CLK_IO;  // "initialize" CLK low
  PRU_SHAREDMEM[CAM_MEM_OFFSET] = 0;
  __delay_cycles(100*1000*200);
  LED_BUSY_GPIO.CLEARDATAOUT = LED_BUSY_IO;
  
  while (1) {
    while (PRU_SHAREDMEM[CAM_MEM_OFFSET] == 0);  // loop until read command from host
    LED_BUSY_GPIO.SETDATAOUT = LED_BUSY_IO;
    
    CAM_SI_GPIO.SETDATAOUT = CAM_SI_IO;  // set SI high
    __delay_cycles(200);  // you definitely have margin to play with here, and for the other __delay_cycles
    CAM_CLK_GPIO.SETDATAOUT = CAM_CLK_IO;  // rising edge of CLK with SI asserted
    __delay_cycles(200);
    CAM_SI_GPIO.CLEARDATAOUT = CAM_SI_IO;
    __delay_cycles(200);
    
    uint8_t i;
    for (i=0; i<128; i++) {
      CAM_CLK_GPIO.CLEARDATAOUT = CAM_CLK_IO;
      PRU_SHAREDMEM[CAM_MEM_OFFSET + 1 + i] = adc_read();  // read on the falling CLK edge
      // no need for a __delay_cycles here, since adc_read() takes 7.2us
      CAM_CLK_GPIO.SETDATAOUT = CAM_CLK_IO;
      __delay_cycles(200);
    }
    CAM_CLK_GPIO.CLEARDATAOUT = CAM_CLK_IO;
    
    PRU_SHAREDMEM[CAM_MEM_OFFSET] = 0;  // reset to zero to indicate done
    LED_BUSY_GPIO.CLEARDATAOUT = LED_BUSY_IO;
  }
}


// Initializes the ADC in "fast" mode
// Based on the old PRU code at https://github.com/ucb-ee192/SkeletonBeagle/blob/master/pru_firmware/src/main_pru0.c
void adc_init() {
  while (CM_WKUP.ADC_TSC_CLKCTR_bit.MODULEMODE != 0x02) {
    CM_WKUP.CLKSTCTRL_bit.CLKTRCTRL = 0;
    CM_WKUP.ADC_TSC_CLKCTR_bit.MODULEMODE = 0x02;
    /* Optional: implement timeout logic. */
  }

  /* 
   * Set the ADC_TSC CTRL register. 
   * Disable TSC_ADC_SS module so we can program it.
   * Set step configuration registers to writable.
   */
  ADC_TSC.CTRL_bit.ENABLE = 0;
  ADC_TSC.CTRL_bit.STEPCONFIG_WRITEPROTECT_N_ACTIVE_LOW = 1;

  /* 
   * make sure no step is enabled, until channel is read with read_adc(chan)  
   * disable TS_CHARGE and Steps 1-16
   */
  ADC_TSC.STEPENABLE = ADC_TSC.STEPENABLE & 0xfffe0000;
  
  /* 
   * set the ADC_TSC STEPCONFIG1 register for channel 0  
   * Mode = 0; SW enabled, one-shot
   * Averaging = 0; 1 sample "average"
   * SEL_INP_SWC_3_0 = 0x0 = Channel 0
   * use FIFO0
   */
  ADC_TSC.STEPCONFIG1_bit.MODE = 0;
  ADC_TSC.STEPCONFIG1_bit.AVERAGING = 0;
  ADC_TSC.STEPCONFIG1_bit.SEL_INP_SWC_3_0 = 0;
  ADC_TSC.STEPCONFIG1_bit.FIFO_SELECT = 0;

  /* make sure delay is also minimum. This is set to zero at reset, but just in case see if it makes any difference. */
  /* sped up by factor of 6 by setting to zero. Perhaps it was set by other rc library code being run. 
   * This gives 7.9 us per conversion
   */
  ADC_TSC.STEPDELAY1_bit.OPENDELAY = 0x0;
  ADC_TSC.STEPDELAY1_bit.SAMPLEDELAY = 0x0;

  /* 
   * set the ADC_TSC CTRL register
   * set step configuration registers to protected
   * store channel ID tag if needed for debug
   * Enable TSC_ADC_SS module
   */
  ADC_TSC.CTRL_bit.STEPCONFIG_WRITEPROTECT_N_ACTIVE_LOW = 0;
  ADC_TSC.CTRL_bit.STEP_ID_TAG = 1;
  ADC_TSC.CTRL_bit.ENABLE = 1;
}

// Reads the ADC, returning the raw 12b value
uint16_t adc_read() {
  while (ADC_TSC.FIFO0COUNT) {
    (void)ADC_TSC.FIFO0DATA;  // perform a dummy read
  }
  
  ADC_TSC.STEPENABLE_bit.STEP1 = 1;  // start the conversion
  
  while (ADC_TSC.FIFO0COUNT == 0) {
    /*
     * loop until value placed in fifo.
     * Optional: implement timeout logic.
     *
     * Other potential options include: 
     *   - configure PRU to receive an ADC interrupt. Note that 
     *     END_OF_SEQUENCE does not mean that the value has been
     *     loaded into the FIFO yet
     *   - perform other actions, and periodically poll for 
     *     FIFO0COUNT > 0
     */
  }
  return ADC_TSC.FIFO0DATA_bit.ADCDATA;
}
