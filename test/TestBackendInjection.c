#include <string.h>
#include <stdio.h>

#include "unity.h"
#include "log_c.h"

static char capture_buffer[512];
static size_t capture_pos;

static void mock_putchar(char ch) {
    if (capture_pos + 1 < sizeof(capture_buffer)) {
        capture_buffer[capture_pos++] = ch;
        capture_buffer[capture_pos] = '\0';
    }
}

void setUp(void) {
    capture_pos = 0;
    capture_buffer[0] = '\0';
}

void tearDown(void) {
    /* Restore default backend to avoid interference with other tests */
    logc_set_putchar(NULL);
}

void test_BackendInjection_CapturesLog(void) {
    logc_set_putchar(mock_putchar);

    /* Log a known message */
    loginfo("Hello %s %d", "world", 123);

    /* Expect prefix [info] and the formatted message followed by newline */
    TEST_ASSERT_TRUE(strstr(capture_buffer, "[info] ") == capture_buffer);
    TEST_ASSERT_NOT_NULL(strstr(capture_buffer, "Hello world 123"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_BackendInjection_CapturesLog);
    return UNITY_END();
}
