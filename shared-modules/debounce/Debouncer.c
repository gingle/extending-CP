#include "common-hal/microcontroller/Pin.h"
#include "shared-bindings/digitalio/Pull.h"
#include "shared-bindings/digitalio/DigitalInOut.h"
#include "py/runtime.h"
#include "supervisor/shared/translate.h"
#include "Debouncer.h"


#define DEBOUNCED_STATE (0x01)
#define UNSTABLE_STATE (0x02)
#define CHANGED_STATE (0x04)


extern uint64_t common_hal_time_monotonic(void);

void set_state(debounce_debouncer_obj_t* self, uint8_t bits)
{
  self->state |= bits;
}

void unset_state(debounce_debouncer_obj_t* self, uint8_t bits)
{
  self->state &= ~bits;
}

void toggle_state(debounce_debouncer_obj_t* self, uint8_t bits)
{
  self->state ^= bits;
}

uint8_t get_state(debounce_debouncer_obj_t* self, uint8_t bits)
{
  return (self->state & bits) != 0;
}

void shared_module_debounce_debouncer_construct(debounce_debouncer_obj_t* self,
                                             mcu_pin_obj_t* pin, mp_int_t interval) {
  digitalinout_result_t result = common_hal_digitalio_digitalinout_construct(&self->pin, pin);
  if (result != DIGITALINOUT_OK) {
    return;
  }
  common_hal_digitalio_digitalinout_switch_to_input(&self->pin, PULL_UP);

  self->state = 0x00;
  if (common_hal_digitalio_digitalinout_get_value(&self->pin)) {
    set_state(self, DEBOUNCED_STATE | UNSTABLE_STATE);
  }
  self->interval = interval;
}

bool shared_module_debounce_debouncer_deinited(debounce_debouncer_obj_t* self) {
  return common_hal_digitalio_digitalinout_deinited(&self->pin);
}

void shared_module_debounce_debouncer_deinit(debounce_debouncer_obj_t* self) {
  if (shared_module_debounce_debouncer_deinited(self)) {
    return;
  }
  common_hal_digitalio_digitalinout_deinit(&self->pin);
}

void shared_module_debounce_debouncer_update(debounce_debouncer_obj_t* self) {
  if (shared_module_debounce_debouncer_deinited(self)) {
    return;
  }
  uint64_t now = common_hal_time_monotonic();
  unset_state(self, CHANGED_STATE);
  bool current_state = common_hal_digitalio_digitalinout_get_value(&self->pin);
  if (current_state != get_state(self, UNSTABLE_STATE)) {
    self->previous_time = now;
    toggle_state(self, UNSTABLE_STATE);
  } else {
    if (now - self->previous_time >= self->interval) {
      if (current_state != get_state(self, DEBOUNCED_STATE)) {
        self->previous_time = now;
        toggle_state(self, DEBOUNCED_STATE);
        set_state(self, CHANGED_STATE);
      }
    }
  }
}

bool shared_module_debounce_debouncer_get_value(debounce_debouncer_obj_t* self) {
  return get_state(self, DEBOUNCED_STATE);
}

bool shared_module_debounce_debouncer_get_rose(debounce_debouncer_obj_t* self) {
  return get_state(self, DEBOUNCED_STATE) && get_state(self, CHANGED_STATE);
}

bool shared_module_debounce_debouncer_get_fell(debounce_debouncer_obj_t* self) {
  return !get_state(self, DEBOUNCED_STATE) && get_state(self, CHANGED_STATE);
}