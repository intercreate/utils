/*
 * Copyright (c) 2023, Intercreate, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * This file is here to test the unity install and can also be used as a
 * template for new tests.
 */

#include "unity.h"

void setUp(void) {} /* Is run before every test, put unit init calls here. */
void tearDown(void) {} /* Is run after every test, put unit clean-up calls here. */

void test(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test);

    return UNITY_END();
}