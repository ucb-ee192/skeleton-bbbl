#ifndef _CM_WKUP_H_
#define _CM_WKUP_H_

typedef struct {  // TODO find a proper header...
  union {
    volatile uint32_t CLKSTCTRL;

    volatile struct {
      unsigned CLKTRCTRL  : 2;    //1:0
      unsigned undef2  : 13;  //14:2  - we don't use these
      unsigned rsvd15  : 17;    //31:15
    } CLKSTCTRL_bit;
  };

  uint8_t unimple4[0xbc-0x4];  // padding to align addresses

  union {
    volatile uint32_t ADC_TSC_CLKCTR;

    volatile struct {
      unsigned MODULEMODE  : 2;    //1:0
      unsigned undef2  : 30;  //31:2  - we don't use these
    } ADC_TSC_CLKCTR_bit;
  };
} cmWkup_reg_t;

#define CM_WKUP (*((volatile cmWkup_reg_t*)0x44E00400))

#endif
