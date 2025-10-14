#ifndef LOG_C_
#define LOG_C_

/* Defining Log Levels */
#define LOG_LEVEL_OFF 0 /* Logging is disabled */
#define LOG_LEVEL_CRITICAL 1 /* Indicates an error that is unrecoverable */
#define LOG_LEVEL_ERROR 2 /* Indicates an error condition */
#define LOG_LEVEL_WARNING 3 /* Indicates a warning condition */
#define LOG_LEVEL_INFO 4 /* Informational messages */
#define LOG_LEVEL_DEBUG 5 /* Debug-level messages */
#define LOG_LEVEL_MAX LOG_LEVEL_DEBUG /* The maximum log level */

typedef enum {
    off = LOG_LEVEL_OFF,
    critical = LOG_LEVEL_CRITICAL,
    error = LOG_LEVEL_ERROR,
    warning = LOG_LEVEL_WARNING,
    info = LOG_LEVEL_INFO,
    debug = LOG_LEVEL_DEBUG
} log_level_e;

/* Defining default configurations */
#ifndef LOG_LEVEL
/** Default maximum log level.
 *
 * This is the maximum log level that will be compiled in.
 */
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

/* Logging API */
void log_message(log_level_e l, const char* fmt, ...);

/* Backend replacement API
 *
 * Consumers can provide their own putchar implementation by calling
 * `logc_set_putchar()` at program initialization time (before spawning
 * threads). The default backend writes to stdout using write().
 */

/* Set custom putchar function. Not thread-safe; set at init time. */
void logc_set_putchar(void (*putchar_func)(char));

/* Public interface for logging */
#if LOG_LEVEL >= LOG_LEVEL_CRITICAL
#ifndef logcritical
#define logcritical(...) log_message(critical, __VA_ARGS__)
#endif
#else
#define logcritical(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#ifndef logerror
#define logerror(...) log_message(error, __VA_ARGS__)
#endif
#else
#define logerror(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#ifndef logwarning
#define logwarning(...) log_message(warning, __VA_ARGS__)
#endif
#else
#define logwarning(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#ifndef loginfo
#define loginfo(...) log_message(info, __VA_ARGS__)
#endif
#else
#define loginfo(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#ifndef logdebug
#define logdebug(...) log_message(debug, __VA_ARGS__)
#endif
#else
#define logdebug(...)
#endif

#endif /* LOG_C_ */
