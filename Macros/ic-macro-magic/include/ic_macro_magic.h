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

#include "ic_macro_magic/ic_macro_magic_internal.h"

/**
 * @brief Macro that expands to its argument.
 *
 * @details This is useful in macros like @c FOR_EACH() when there is no
 * transformation required on the list elements.
 *
 * @param V any value
 */
#define IC_IDENTITY(V) _IC_Z_IDENTITY(V)

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

#define IC_FLAGS_FOR(...) _IC_FLAGS_FOR(__VA_ARGS__)

#define IC_STRING_MAP_FOR(...) _IC_STRING_MAP_FOR(__VA_ARGS__)
