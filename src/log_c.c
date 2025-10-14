/* Implementation of the logging backend and log_message
 * This file is the main compilation unit for the logger. Build it into
 * a static library (liblogc.a) for reuse.
 */

#include <stdarg.h>
#include <string.h>
#include <unistd.h> /* for write() */

#include "log_c.h"
#include "../3rd-party/printf/printf.h"

#ifndef LOG_LEVEL_NAMES
/* Users can override these default names with a compiler definition */
#define LOG_LEVEL_NAMES {"off", "critical", "error", "warning", "info", "debug"}
#endif

static const char* LOG_LEVEL_TO_C_STRING(log_level_e level) {
    static const char* level_string_names[LOG_LEVEL_MAX + 1] = LOG_LEVEL_NAMES;
    return level_string_names[level];
}

/* Default putchar function that will be used by the printf library */
static void (*custom_putchar)(char) = NULL;

/* Implementation of _putchar required by the printf library */
void _putchar(char character) {
    if (custom_putchar) {
        custom_putchar(character);
    } else {
        /* Default behavior - write to stdout */
        #ifdef _WIN32
        _write(1, &character, 1);
        #else
        write(1, &character, 1);
        #endif
    }
}

void logc_set_putchar(void (*putchar_func)(char)) {
    custom_putchar = putchar_func;
}

void log_message(log_level_e level, const char* fmt, ...) {
    if (fmt == NULL) {
        return;
    }

    const char* level_str = LOG_LEVEL_TO_C_STRING(level);

    /* Print level prefix using printf library */
    printf_("[%s] ", level_str);
    
    /* Print the actual message */
    va_list ap;
    va_start(ap, fmt);
    vprintf_(fmt, ap);
    va_end(ap);
    
    /* Add newline */
    _putchar('\n');
}
