#include <stdio.h>
#include <stdarg.h>

#include "log_c.h"
#include "printf.h"

#ifndef LOG_LEVEL_NAMES
/* Users can override these default names with a compiler definition */
#define LOG_LEVEL_NAMES {"off", "critical", "error", "warning", "info", "debug"}
#endif

const char* LOG_LEVEL_TO_C_STRING(log_level_e level) {
    const char* level_string_names[LOG_LEVEL_MAX + 1] = LOG_LEVEL_NAMES;
    return level_string_names[level];
}

void log_message(log_level_e level, const char* fmt, ...) {
    if (fmt == NULL) {
        return;
    }

    /* Print the level name, then the formatted message using the
       variable argument list. We append a newline for readability. */
    const char* level_str = LOG_LEVEL_TO_C_STRING(level);
    va_list ap;
    va_start(ap, fmt);
    /* Print level prefix */
    printf("[%s] ", level_str);
    /* Print the user-supplied formatted message */
    vprintf(fmt, ap);
    va_end(ap);
    putchar('\n');
}

int main(void) {
    loginfo("This is an info message: %d", 42);
    logdebug("This is a debug message: %s", "debugging"); // Suppressed if LOG_LEVEL < LOG_LEVEL_DEBUG
    logerror("This is an error message");
    return 0;
}
