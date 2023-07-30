/*
 * Copyright (c) 2023, Intercreate, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "../../include/ic_macro_magic.h"

#define NAMESPACED_LIST IC_PREFIXES_FOR(MY_NAMESPACE, ONE, TWO, THREE, FOUR)

enum my_enum {
    NAMESPACED_LIST
};
enum my_enum_flags {
    IC_FLAGS_FOR(NAMESPACED_LIST)
};
extern char const * const MY_NAMESPACE_STRINGS[];

#define MY_NAMESPACE_STRING(enum) IC_MAKE_STRING_GETTER(enum, MY_NAMESPACE_STRINGS, NAMESPACED_LIST)

#define OFFSET1_LIST IC_PREFIXES_FOR(OFFSET, TWENTY, TWENTY1, TWENTY2, TWENTY3)
#define OFFSET2_LIST IC_PREFIXES_FOR(OFFSET, FORTY, FORTY1, FORTY2, FORTY3)

enum offset_enum {
    IC_ENUMS_FOR(0, ZERO, ONE),
    IC_ENUMS_FOR(20, OFFSET1_LIST),
    IC_ENUMS_FOR(40, OFFSET2_LIST)
};
extern char const * const OFFSET1_STRINGS[];
extern char const * const OFFSET2_STRINGS[];

#define OFFSET1_STRING(enum) IC_MAKE_STRING_GETTER(enum, OFFSET1_STRINGS, OFFSET1_LIST)
#define OFFSET2_STRING(enum) IC_MAKE_STRING_GETTER(enum, OFFSET2_STRINGS, OFFSET2_LIST)

#define NEGATIVE_LIST IC_PREFIXES_FOR(ERR, 128, 127)

enum negative_enum {
    IC_ENUMS_FOR(128 * -1, NEGATIVE_LIST),
};

extern char const * const ERR_STRINGS[];

#define ERR_STRING(enum) IC_MAKE_STRING_GETTER(enum, ERR_STRINGS, NEGATIVE_LIST)
