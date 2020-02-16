#ifndef _PRU_CAMERA_H_
#define _PRU_CAMERA_H_

#include <stdexcept>
#include <robotcontrol.h>
#include <rc/pru.h>

class PruCamera {
public:
  PruCamera() {
  }

  void init() {
    if (rc_pru_start(0, "am335x-pru0-custom-fw")) {
      throw std::runtime_error("PRU init failed");
    }
    pruSharedMem_ = rc_pru_shared_mem_ptr();
  }

  void stop() {
    rc_pru_stop(0);
    pruSharedMem_ = NULL;
  }

  void read(uint16_t* out) {
    if (pruSharedMem_ == NULL) {
      throw std::runtime_error("PRU not initialized, did you call PruCamera::init() ?");
    }
    while (pruSharedMem_[kPruMemOffset] != 0);  // wait for PRU to be ready; usually shouldn't happen

    pruSharedMem_[kPruMemOffset] = 1;  // signal to start reading the camera

    while (pruSharedMem_[kPruMemOffset] != 0);  // wait for PRU to finish; you might sleep the thread here

    for (size_t i=0; i<128; i++) {
      out[i] = pruSharedMem_[kPruMemOffset + 1 + i];
    }
  }

protected:
  volatile uint32_t* pruSharedMem_ = NULL;

  static const size_t kPruMemOffset = (16 + 1);
};

#endif
