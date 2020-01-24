#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_DEBOUNCE_DEBOUNCER_H
#define MICROPY_INCLUDED_SHARED_BINDINGS_DEBOUNCE_DEBOUNCER_H

#include "shared-module/debounce/Debouncer.h"

extern const mp_obj_type_t debounce_debouncer_type;

extern void shared_module_debounce_debouncer_construct(debounce_debouncer_obj_t* self, mcu_pin_obj_t* pin, mp_int_t interval);
extern void shared_module_debounce_debouncer_deinit(debounce_debouncer_obj_t* self);
extern bool shared_module_debounce_debouncer_deinited(debounce_debouncer_obj_t* self);
extern void shared_module_debounce_debouncer_update(debounce_debouncer_obj_t* self);
extern bool shared_module_debounce_debouncer_get_fell(debounce_debouncer_obj_t* self);
extern bool shared_module_debounce_debouncer_get_rose(debounce_debouncer_obj_t* self);
extern bool shared_module_debounce_debouncer_get_value(debounce_debouncer_obj_t* self);

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_DEBOUNCE_DEBOUNCER_H