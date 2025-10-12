#include "unity.h"
#include "log_c.h"

void setUp(void) {}

void tearDown(void) {}

void test_HelloWorld_Basic(void) {
    int val = 42;
    TEST_ASSERT_EQUAL(42, val);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_HelloWorld_Basic);
    return UNITY_END();
}
