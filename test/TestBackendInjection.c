#include "unity.h"
#include "log_c.h"
#include <string.h>
#include <stdarg.h>

static char capture_buffer[512];
static size_t capture_pos;

static int mock_vprintf(const char *fmt, va_list ap) {
    /* Write formatted data into the capture buffer starting at capture_pos */
    int written = vsnprintf(capture_buffer + capture_pos,
                            sizeof(capture_buffer) - capture_pos,
                            fmt, ap);
    if (written > 0) {
        size_t avail = sizeof(capture_buffer) - capture_pos;
        if ((size_t)written >= avail) {
            /* truncated */
            capture_pos = sizeof(capture_buffer) - 1;
            capture_buffer[capture_pos] = '\0';
        } else {
            capture_pos += (size_t)written;
        }
    }
    return written;
}

static size_t mock_write(const char *buf, size_t len) {
    size_t to_copy = len;
    if (capture_pos + to_copy >= sizeof(capture_buffer)) {
        to_copy = sizeof(capture_buffer) - 1 - capture_pos;
    }
    memcpy(capture_buffer + capture_pos, buf, to_copy);
    capture_pos += to_copy;
    capture_buffer[capture_pos] = '\0';
    return to_copy;
}

static int mock_putchar(int ch) {
    if (capture_pos + 1 >= sizeof(capture_buffer)) {
        return EOF;
    }
    capture_buffer[capture_pos++] = (char)ch;
    capture_buffer[capture_pos] = '\0';
    return ch;
}

void setUp(void) {
    capture_pos = 0;
    capture_buffer[0] = '\0';
}

void tearDown(void) {
    /* Restore default backend to avoid interference with other tests */
    logc_set_backend(NULL);
}

void test_BackendInjection_CapturesLog(void) {
    struct logc_backend mock = {
        .vprintf = mock_vprintf,
        .write = mock_write,
        .putchar = mock_putchar,
    };

    logc_set_backend(&mock);

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
