#include <stdint.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/debounce/__init__.h"
#include "shared-bindings/debounce/Debouncer.h"

STATIC const mp_rom_map_elem_t debounce_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_debounce) },
    { MP_ROM_QSTR(MP_QSTR_Debouncer), MP_ROM_PTR(&debounce_debouncer_type) },
};

STATIC MP_DEFINE_CONST_DICT(debounce_module_globals, debounce_module_globals_table);

const mp_obj_module_t debounce_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&debounce_module_globals,
};