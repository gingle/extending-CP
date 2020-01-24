#include <stdint.h>

#include "lib/utils/context_manager_helpers.h"
#include "py/objproperty.h"
#include "py/runtime.h"
#include "py/runtime0.h"
#include "shared-bindings/microcontroller/Pin.h"
#include "shared-bindings/debounce/Debouncer.h"
#include "shared-bindings/util.h"

//| .. currentmodule:: debounce
//|
//| :class:`Debouncer` -- Debounce an input pin
//| ====================================================================================
//|
//| Debouncer cleans up an input pin and provides value/rise/fall properties.
//|
//| .. class:: Debouncer(pin, mode, interval)
//|
//|   Create a Debouncer object associated with the given pin. It tracks the value of the pin over time,
//|   allowing it to settle before acknowledging transitions.
//|
//|   :param ~microcontroller.Pin pin: Pin to debounce
//|   :param ~int interval: debounce interval in milliseconds
//|
//|   For example::
//|
//|     import debounce
//|     import board
//|
//|     deb = bebounce.Debounce(board.D10, 10)
//|     while True:
//|         deb.update()
//|         if deb.fell
//|             print("Pressed")
//|
STATIC mp_obj_t debounce_debouncer_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_pin, ARG_interval };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_pin_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_interval, MP_ARG_INT },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    assert_pin(args[ARG_pin].u_obj, false);
    mcu_pin_obj_t* pin = MP_OBJ_TO_PTR(args[ARG_pin].u_obj);

    mp_int_t interval = 10;
    if (n_args == 2) {
      interval = args[ARG_interval].u_int;
    }

    debounce_debouncer_obj_t *self = m_new_obj(debounce_debouncer_obj_t);
    self->base.type = &debounce_debouncer_type;

    shared_module_debounce_debouncer_construct(self, pin, interval);

    return MP_OBJ_FROM_PTR(self);
}


//|   .. method:: deinit()
//|
//|      Deinitializes the debouncer and releases any hardware resources for reuse.
//|
STATIC mp_obj_t debounce_debouncer_deinit(mp_obj_t self_in) {
    debounce_debouncer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    shared_module_debounce_debouncer_deinit(self);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(debounce_debouncer_deinit_obj, debounce_debouncer_deinit);

//|   .. method:: update()
//|
//|      Do an update cycle it it's time to.
//|
STATIC mp_obj_t debounce_debouncer_obj_update(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    shared_module_debounce_debouncer_update(args[0]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(debounce_debouncer_update_obj, 1, 1, debounce_debouncer_obj_update);


//|   .. method:: __exit__()
//|
//|      Automatically deinitializes the hardware when exiting a context. See
//|      :ref:`lifetime-and-contextmanagers` for more info.
//|
STATIC mp_obj_t debounce_debouncer_obj___exit__(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    shared_module_debounce_debouncer_deinit(args[0]);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(debounce_debouncer___exit___obj, 4, 4, debounce_debouncer_obj___exit__);


//|   .. attribute:: value
//|
//|     The current debounced value
//|
STATIC mp_obj_t debounce_debouncer_obj_get_value(mp_obj_t self_in) {
    debounce_debouncer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    raise_error_if_deinited(shared_module_debounce_debouncer_deinited(self));

    return mp_obj_new_bool(shared_module_debounce_debouncer_get_value(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(debounce_debouncer_get_value_obj, debounce_debouncer_obj_get_value);


//|   .. attribute:: rose
//|
//|     Whether the input transitioned low to high since last update
//|
STATIC mp_obj_t debounce_debouncer_obj_get_rose(mp_obj_t self_in) {
    debounce_debouncer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    raise_error_if_deinited(shared_module_debounce_debouncer_deinited(self));

    return mp_obj_new_bool(shared_module_debounce_debouncer_get_rose(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(debounce_debouncer_get_rose_obj, debounce_debouncer_obj_get_rose);


//|   .. attribute:: value
//|
//|     Whether the input transitioned high to low since last update
//|
STATIC mp_obj_t debounce_debouncer_obj_get_fell(mp_obj_t self_in) {
    debounce_debouncer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    raise_error_if_deinited(shared_module_debounce_debouncer_deinited(self));

    return mp_obj_new_bool(shared_module_debounce_debouncer_get_fell(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(debounce_debouncer_get_fell_obj, debounce_debouncer_obj_get_fell);


const mp_obj_property_t debounce_debouncer_value_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&debounce_debouncer_get_value_obj,
              (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t debounce_debouncer_rose_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&debounce_debouncer_get_rose_obj,
              (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t debounce_debouncer_fell_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&debounce_debouncer_get_fell_obj,
              (mp_obj_t)&mp_const_none_obj},
};

STATIC const mp_rom_map_elem_t debounce_debouncer_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&debounce_debouncer_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&debounce_debouncer___exit___obj) },
    { MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&debounce_debouncer_update_obj) },
    { MP_ROM_QSTR(MP_QSTR_value), MP_ROM_PTR(&debounce_debouncer_value_obj) },
    { MP_ROM_QSTR(MP_QSTR_rose), MP_ROM_PTR(&debounce_debouncer_rose_obj) },
    { MP_ROM_QSTR(MP_QSTR_fell), MP_ROM_PTR(&debounce_debouncer_fell_obj) },
};
STATIC MP_DEFINE_CONST_DICT(debounce_debouncer_locals_dict, debounce_debouncer_locals_dict_table);

const mp_obj_type_t debounce_debouncer_type = {
    { &mp_type_type },
    .name = MP_QSTR_Debouncer,
    .make_new = debounce_debouncer_make_new,
    .locals_dict = (mp_obj_dict_t*)&debounce_debouncer_locals_dict,
};

STATIC mp_obj_t debounce_debouncer_obj_get_rose(mp_obj_t self_in) {
    debounce_debouncer_obj_t *self = MP_OBJ_TO_PTR(self_in);
    raise_error_if_deinited(shared_module_debounce_debouncer_deinited(self));

    return mp_obj_new_bool(shared_module_debounce_debouncer_get_rose(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(debounce_debouncer_get_rose_obj, debounce_debouncer_obj_get_rose);

STATIC const mp_rom_map_elem_t debounce_debouncer_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&debounce_debouncer_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&debounce_debouncer___exit___obj) },
    { MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&debounce_debouncer_update_obj) },
    { MP_ROM_QSTR(MP_QSTR_value), MP_ROM_PTR(&debounce_debouncer_value_obj) },
    { MP_ROM_QSTR(MP_QSTR_rose), MP_ROM_PTR(&debounce_debouncer_rose_obj) },
    { MP_ROM_QSTR(MP_QSTR_fell), MP_ROM_PTR(&debounce_debouncer_fell_obj) },
};
STATIC MP_DEFINE_CONST_DICT(debounce_debouncer_locals_dict, debounce_debouncer_locals_dict_table);

const mp_obj_type_t debounce_debouncer_type = {
    { &mp_type_type },
    .name = MP_QSTR_Debouncer,
    .make_new = debounce_debouncer_make_new,
    .locals_dict = (mp_obj_dict_t*)&debounce_debouncer_locals_dict,
};
