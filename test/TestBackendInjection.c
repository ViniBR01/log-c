#include <string.h>
#include <stdio.h>

#include "unity.h"
#include "log_c.h"

static char capture_buffer[512];
static size_t capture_length;

/* New callback-based mock */
static void mock_output_callback(const char* message, size_t length) {
    if (length < sizeof(capture_buffer)) {
        memcpy(capture_buffer, message, length);
        capture_length = length;
        capture_buffer[length] = '\0'; /* Null-terminate for testing */
    }
}

void setUp(void) {
    capture_length = 0;
    capture_buffer[0] = '\0';
}

void tearDown(void) {
    /* Clear the output callback */
    log_set_output_callback(NULL);
}

void test_BackendInjection_CapturesLog(void) {
    log_set_output_callback(mock_output_callback);

    /* Log a known message */
    loginfo("Hello %s %d", "world", 123);

    /* Expect prefix [info] and the formatted message followed by newline */
    TEST_ASSERT_TRUE(strstr(capture_buffer, "[info] ") == capture_buffer);
    TEST_ASSERT_NOT_NULL(strstr(capture_buffer, "Hello world 123"));
    TEST_ASSERT_GREATER_THAN(0, capture_length);
}

void test_BackendInjection_FormatsUnsigned(void) {
    log_set_output_callback(mock_output_callback);

    loginfo("Value: %u", 42);

    TEST_ASSERT_NOT_NULL(strstr(capture_buffer, "Value: 42"));
}

void test_BackendInjection_FormatsHex(void) {
    log_set_output_callback(mock_output_callback);

    loginfo("Hex: 0x%x", 255);

    TEST_ASSERT_NOT_NULL(strstr(capture_buffer, "Hex: 0xff"));
}

void test_BackendInjection_FormatsChar(void) {
    log_set_output_callback(mock_output_callback);

    loginfo("Char: %c", 'A');

    TEST_ASSERT_NOT_NULL(strstr(capture_buffer, "Char: A"));
}

void test_BackendInjection_IsConfiguredCheck(void) {
    TEST_ASSERT_FALSE(log_is_output_configured());
    
    log_set_output_callback(mock_output_callback);
    
    TEST_ASSERT_TRUE(log_is_output_configured());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_BackendInjection_CapturesLog);
    RUN_TEST(test_BackendInjection_FormatsUnsigned);
    RUN_TEST(test_BackendInjection_FormatsHex);
    RUN_TEST(test_BackendInjection_FormatsChar);
    RUN_TEST(test_BackendInjection_IsConfiguredCheck);
    return UNITY_END();
}
