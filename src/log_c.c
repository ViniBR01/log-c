#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h> /* for write() on POSIX; fallback is provided in default backend */

#include "log_c.h"

#ifndef LOG_LEVEL_NAMES
/* Users can override these default names with a compiler definition */
#define LOG_LEVEL_NAMES {"off", "critical", "error", "warning", "info", "debug"}
#endif

static const char* LOG_LEVEL_TO_C_STRING(log_level_e level) {
    static const char* level_string_names[LOG_LEVEL_MAX + 1] = LOG_LEVEL_NAMES;
    return level_string_names[level];
}

/* Default backend implementation using the C standard library */
static int default_vprintf(const char *fmt, va_list ap) {
    return vprintf(fmt, ap);
}

static size_t default_write(const char *buf, size_t len) {
#if defined(_POSIX_VERSION)
    ssize_t r = write(STDOUT_FILENO, buf, len);
    return (r < 0) ? 0 : (size_t)r;
#else
    /* Fallback to fwrite if write() not available */
    return fwrite(buf, 1, len, stdout);
#endif
}

static int default_putchar(int ch) {
    return putchar(ch);
}

static const struct logc_backend default_backend = {
    .vprintf = default_vprintf,
    .write = default_write,
    .putchar = default_putchar,
};

/* Active backend pointer; defaults to default_backend. Mutable via setter. */
static const struct logc_backend *active_backend = &default_backend;

void logc_set_backend(const struct logc_backend *backend) {
    if (backend == NULL) {
        active_backend = &default_backend;
    } else {
        active_backend = backend;
    }
}

const struct logc_backend *logc_get_backend(void) {
    return active_backend;
}

void log_message(log_level_e level, const char* fmt, ...) {
    if (fmt == NULL) {
        return;
    }

    const char* level_str = LOG_LEVEL_TO_C_STRING(level);

    /* Print level prefix */
    /* We use the backend's vprintf for formatting and putchar for newline */
    va_list ap;
    va_start(ap, fmt);
    /* Print prefix using the backend write function for safety */
    char prefix[64];
    int prefix_len = snprintf(prefix, sizeof(prefix), "[%s] ", level_str);
    if (prefix_len > 0) {
        const struct logc_backend *b = logc_get_backend();
        b->write(prefix, (size_t)prefix_len);
        b->vprintf(fmt, ap);
        b->putchar('\n');
    }
    va_end(ap);
}
