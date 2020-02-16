#ifndef _GPIO_H_
#define _GPIO_H_

typedef struct {  // TODO find a proper header...
  union {
    volatile uint32_t REVISION;

    volatile struct {
      unsigned MINOR  : 6;    //5:0
      unsigned CUSTOM   : 2;    //7:6
      unsigned MAJOR  : 3;    //10:8
      unsigned RTL    : 5;    //15:11
      unsigned FUNC   : 12;   //27:16
      unsigned rsvd28   : 2;    //29:28
      unsigned SCHEME   : 2;    //31:30
    } REVISION_bit;
  };

  uint8_t unimpl4[0x134-0x4];  // padding to align addresses

  volatile uint32_t OE;
  volatile uint32_t DATAIN;
  volatile uint32_t DATAOUT;

  uint8_t unimpl140[0x190-0x13C-0x4];  // padding to align addresses

  volatile uint32_t CLEARDATAOUT;
  volatile uint32_t SETDATAOUT;
} gpio_reg_t;

#define GPIO0 (*((volatile gpio_reg_t*)0x44E07000))
#define GPIO1 (*((volatile gpio_reg_t*)0x4804C000))
#define GPIO2 (*((volatile gpio_reg_t*)0x481AC000))
#define GPIO3 (*((volatile gpio_reg_t*)0x481AE000))

#endif
