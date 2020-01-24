#ifndef MICROPY_INCLUDED_DEBOUNCE_DEBOUNCER_H
#define MICROPY_INCLUDED_DEBOUNCE_DEBOUNCER_H

#include "shared-bindings/digitalio/DigitalInOut.h"
#include "py/obj.h"

typedef struct {
  mp_obj_base_t base;
  uint8_t state;
  digitalio_digitalinout_obj_t pin;
  uint64_t previous_time;
  uint64_t interval;
} debounce_debouncer_obj_t;


#endif // MICROPY_INCLUDED_DEBOUNCE_DEBOUNCER_H
