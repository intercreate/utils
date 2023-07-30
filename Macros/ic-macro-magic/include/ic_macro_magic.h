/*
 * Copyright (c) 2023, Intercreate, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * uSHET library
 *      Copyright (c) 2014 Thomas Nixon, Jonathan Heathcote
 *
 *      SPDX-License-Identifier: MIT
 *
 * Zephyr Util Macros
 *      Copyright (c) 2011-2014, Wind River Systems, Inc.
 *      Copyright (c) 2021, Nordic Semiconductor ASA
 *
 *      SPDX-License-Identifier: Apache-2.0
 */

#ifndef _IC_MACRO_MAGIC_H
#define _IC_MACRO_MAGIC_H

#include "ic_macro_magic/ic_macro_magic_internal.h"

/**
 * @brief Macro that maps the prefix onto each item of the list, separated by _.
 *
 * @details For example, IC_PREFIXES_FOR(MY_NAMESPACE, ONE, TWO, THREE) would produce
 * MY_NAMESPACE_ONE, MY_NAMESPACE_TWO, MY_NAMESPACE_THREE.
 *
 * @param prefix The prefix to add to each item of the list
 * @param ... Variable argument list
 */
#define IC_PREFIXES_FOR(prefix, ...) _IC_PREFIXES_FOR(prefix, __VA_ARGS__)

/**
 * @brief Macro that expands a list into an enum definition starting at start.
 *
 * @details Example usage:
 *
 *      enum offset_enum {
 *          IC_ENUMS_FOR(20, TWENTY, TWENTY1),
 *          IC_ENUMS_FOR(40, FORTY, FORTY1)
 *      };
 *
 * This usage expands to:
 *
 *      enum offset_enum {
 *          TWENTY = 20,
 *          TWENTY1,
 *          FORTY = 40,
 *          FORTY1
 *      };
 *
 * @param start The starting value of an enum series; for negative values, use val * -1.
 * @param ... Variable argument list
 */
#define IC_ENUMS_FOR(start, ...) _IC_ENUMS_FOR(start, __VA_ARGS__)

/**
 * @brief Macro that produces bit flags for each item of the list, postfixed with _FLAG.
 *
 * @details For example, IC_FLAGS_FOR(ZERO, ONE, TWO) would produce
 *      ZERO_FLAG = (1UL << ZERO),
 *      ONE_FLAG = (1UL << ONE),
 *      TWO_FLAG = (1UL << TWO)
 *
 * @param ... Variable argument list
 */
#define IC_FLAGS_FOR(...) _IC_FLAGS_FOR(__VA_ARGS__)

/**
 * @brief Macro that creates a map of enum constants to string constant representations.
 *
 * @details Example usage:
 *
 *      char const * const MY_STRING_MAP[] = {IC_STRING_MAP_FOR(NEG_ONE, ZERO, ONE)};
 *
 * This usage expands to:
 *
 *      chart const * const MY_STRING_MAP[] = {
 *          [NEG_ONE - NEG_ONE] = "NEG_ONE",
 *          [ZERO - NEG_ONE] = "ZERO",
 *          [ONE - NEG_ONE] = "ONE"
 *      };
 *
 * @param ... Variable argument list
 */
#define IC_STRING_MAP_FOR(...) _IC_STRING_MAP_FOR(__VA_ARGS__)

/**
 * @brief Macro that returns a macro that maps enum constants to their string constant
 *        representations.
 *
 * @details Example usage:
 *
 *      #define MY_LIST NEG_ONE, ZERO, ONE
 *      enum my_enum { IC_ENUMS_FOR(1 * -1, MY_LIST) };
 *      char const * const MY_STRINGS[] = {IC_STRING_MAP_FOR(MY_LIST)};
 *
 *      // Instead of this:
 *      printf("The enum is %s\n", MY_STRINGS[NEG_ONE - NEG_ONE]);
 *
 *      // Do this:
 *      #define MY_STRING(enum) IC_MAKE_STRING_GETTER(enum, MY_STRINGS, MY_LIST)
 *      printf("The enum is %s\n", MY_STRING(NEG_ONE));
 *
 *  The example usage of MY_STRING() would expand to:
 *
 *      printf("The enum is %s\n", MY_STRINGS[NEG_ONE - NEG_ONE]);
 *
 *  It also provides simple bounds checking that assumes that the strings array is
 *  contiguous.
 *
 * @param enum The argument that will be passed to the created macro
 * @param arr The array that maps enums to their string constant representations
 * @param ... The variable argument list that was used to create the string map
 */
#define IC_MAKE_STRING_GETTER(enum, arr, ...) _IC_MAKE_STRING_GETTER(enum, arr, __VA_ARGS__)

#endif
