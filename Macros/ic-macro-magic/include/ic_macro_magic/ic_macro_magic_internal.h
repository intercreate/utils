/*
 * Copyright (c) 2023, Intercreate, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * uSHET library
 *      Copyright (c) 2014 Thomas Nixon, Jonathan Heathcote
 *      SPDX-License-Identifier: MIT
 *
 * Zephyr Util Macros
 *      Copyright (c) 2011-2014, Wind River Systems, Inc.
 *      Copyright (c) 2021, Nordic Semiconductor ASA
 *      SPDX-License-Identifier: Apache-2.0
 */

/* The zephyr macros are prefixed with _IC_Z_ */
#include "zephyr/util_macro.h"

/* The cpp_magic macros are prefixed with _IC_M_ */
#include "ushet/cpp_magic.h"

/* Intercreate internal macros are prefixed with _IC_ */


#define _IC_PREFIXES_FOR(prefix, ...) \
    _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__)) \
        (_IC_M_EVAL(_IC_PREFIX_INNER(prefix, __VA_ARGS__)))
#define _IC_PREFIX_INNER(prefix, cur_val, ...) \
    _IC_M_CAT3(prefix, _, cur_val) \
    _IC_M_IF(_IC_M_HAS_ARGS(__VA_ARGS__)) \
        (_IC_M_COMMA() _IC_M_DEFER2(_IC__PREFIX_INNER)()(prefix, ##__VA_ARGS__))
#define _IC__PREFIX_INNER() _IC_PREFIX_INNER

#define _IC_ENUM_FLAGS_MACRO(enum) enum##_FLAG = _IC_Z_BIT(enum)
#define _IC_FLAGS_FOR(...) _IC_M_MAP(_IC_ENUM_FLAGS_MACRO, _IC_M_COMMA, __VA_ARGS__)

#define _IC_ENUM_STRING_MAP_MACRO(enum) [enum] = #enum
#define _IC_STRING_MAP_FOR(...) _IC_M_MAP(_IC_ENUM_STRING_MAP_MACRO, _IC_M_COMMA, __VA_ARGS__)
