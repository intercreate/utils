#include "unity.h"

void setUp (void) {} /* Is run before every test, put unit init calls here. */
void tearDown (void) {} /* Is run after every test, put unit clean-up calls here. */

#include "my_enum.h"

void test_enum(void) {
    TEST_ASSERT_EQUAL(0, MY_NAMESPACE_ONE);
    TEST_ASSERT_EQUAL(1, MY_NAMESPACE_TWO);
    TEST_ASSERT_EQUAL(2, MY_NAMESPACE_THREE);
    TEST_ASSERT_EQUAL(3, MY_NAMESPACE_FOUR);
}

void test_enum_flags(void) {
    TEST_ASSERT_EQUAL(1 << 0, MY_NAMESPACE_ONE_FLAG);
    TEST_ASSERT_EQUAL(1 << 1, MY_NAMESPACE_TWO_FLAG);
    TEST_ASSERT_EQUAL(1 << 2, MY_NAMESPACE_THREE_FLAG);
    TEST_ASSERT_EQUAL(1 << 3, MY_NAMESPACE_FOUR_FLAG);
}

void test_enum_strings(void) {
    TEST_ASSERT_EQUAL_STRING("MY_NAMESPACE_ONE", MY_NAMESPACE_STRINGS[MY_NAMESPACE_ONE]);
    TEST_ASSERT_EQUAL_STRING("MY_NAMESPACE_TWO", MY_NAMESPACE_STRINGS[MY_NAMESPACE_TWO]);
    TEST_ASSERT_EQUAL_STRING("MY_NAMESPACE_THREE", MY_NAMESPACE_STRINGS[MY_NAMESPACE_THREE]);
    TEST_ASSERT_EQUAL_STRING("MY_NAMESPACE_FOUR", MY_NAMESPACE_STRINGS[MY_NAMESPACE_FOUR]);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_enum);
    RUN_TEST(test_enum_flags);
    
    return UNITY_END();
}