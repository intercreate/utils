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
