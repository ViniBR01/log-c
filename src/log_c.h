#ifndef LOG_C_
#define LOG_C_

#include <stddef.h>
#include <stdbool.h>

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

/* Backend API
 *
 * The logging library requires an output callback to send formatted log
 * messages to the user's output device (UART, file, network, etc.).
 * 
 * The callback receives the complete formatted message including the log
 * level prefix and newline. The user is responsible for outputting this
 * message to their desired destination.
 * 
 * This API replaces the old putchar-based approach and eliminates the
 * dependency on printf and weak symbols.
 */

/**
 * @brief Output callback function type
 * 
 * This callback is invoked for each log message. The message is fully
 * formatted and includes:
 * - Log level prefix: "[info] ", "[error] ", etc.
 * - Formatted user message
 * - Newline character at the end
 * 
 * The message buffer is only valid during the callback - if you need to
 * keep it, make a copy.
 * 
 * Thread Safety: This callback may be called from different contexts
 * (main code, ISRs, different threads). Ensure your implementation is
 * thread-safe if needed.
 * 
 * @param message Pointer to formatted message buffer (not null-terminated)
 * @param length Length of the message in bytes
 */
typedef void (*log_output_callback_t)(const char* message, size_t length);

/**
 * @brief Set the output callback for log messages
 * 
 * This function must be called before any logging occurs. After setting
 * the callback, all log messages will be sent to this function.
 * 
 * Example:
 * @code
 * void my_output(const char* msg, size_t len) {
 *     for (size_t i = 0; i < len; i++) {
 *         uart_write(msg[i]);
 *     }
 * }
 * 
 * int main(void) {
 *     log_set_output_callback(my_output);
 *     loginfo("Hello, world!");
 * }
 * @endcode
 * 
 * @param callback Output callback function (must not be NULL)
 */
void log_set_output_callback(log_output_callback_t callback);

/**
 * @brief Check if output callback is configured
 * 
 * @return true if output callback has been set, false otherwise
 */
bool log_is_output_configured(void);

/* Runtime Log Level Control
 *
 * These functions enable dynamic control of log verbosity at runtime.
 * The runtime level filters messages within the compile-time maximum.
 *
 * Example:
 * @code
 * // Compiled with LOG_LEVEL=LOG_LEVEL_DEBUG (all levels included)
 * log_set_level(LOG_LEVEL_INFO);  // Suppress debug messages at runtime
 * loginfo("This prints");          // ✓ Compiled in, runtime allows
 * logdebug("This is suppressed");  // ✗ Compiled in, runtime blocks
 * 
 * log_set_level(LOG_LEVEL_DEBUG); // Re-enable debug messages
 * logdebug("Now this prints");     // ✓ Compiled in, runtime allows
 * @endcode
 */

/**
 * @brief Set the runtime log level
 * 
 * Controls which log messages are output at runtime. Messages with a level
 * higher than the runtime level will be discarded before formatting.
 * 
 * The runtime level cannot exceed the compile-time maximum (LOG_LEVEL).
 * If you try to set a level higher than compiled in, it will be clamped
 * to the compile-time maximum.
 * 
 * This function is safe to call from main code. For thread-safe operation
 * in RTOS environments, ensure exclusive access during the call.
 * 
 * @param level New runtime log level (will be clamped to compile-time max)
 */
void log_set_level(log_level_e level);

/**
 * @brief Get the current runtime log level
 * 
 * @return Current runtime log level
 */
log_level_e log_get_level(void);

/**
 * @brief Get the compile-time maximum log level
 * 
 * This returns the maximum level that was compiled into the binary via
 * the LOG_LEVEL define. You cannot set the runtime level higher than this.
 * 
 * This is useful for:
 * - Validating runtime level changes
 * - Displaying available log levels in UI/CLI
 * - Debugging build configuration
 * 
 * @return Compile-time maximum log level (value of LOG_LEVEL at compile time)
 */
log_level_e log_get_compile_time_level(void);

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
