/* Implementation of the logging backend and log_message
 * This file is the main compilation unit for the logger. Build it into
 * a static library (liblogc.a) for reuse.
 *
 * This implementation is self-contained with no external dependencies
 * (except stdarg.h, stddef.h, stdbool.h, string.h) and provides its own
 * minimal formatting functions.
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "log_c.h"

/* Configuration: Maximum message buffer size */
#ifndef LOG_MAX_MESSAGE_SIZE
#define LOG_MAX_MESSAGE_SIZE 256
#endif

/*=============================================================================
 * Internal Formatting Utilities
 *============================================================================*/

/**
 * @brief Convert unsigned integer to decimal string
 * @param value Value to convert
 * @param buffer Output buffer
 * @param buf_size Size of output buffer
 * @return Number of characters written
 */
static size_t format_uint(unsigned int value, char* buffer, size_t buf_size) {
    if (buf_size == 0) return 0;
    
    /* Handle zero specially */
    if (value == 0) {
        if (buf_size < 2) return 0;
        buffer[0] = '0';
        return 1;
    }
    
    /* Convert digits in reverse */
    char temp[12]; /* Max 10 digits for 32-bit + null */
    size_t i = 0;
    while (value > 0 && i < sizeof(temp) - 1) {
        temp[i++] = '0' + (value % 10);
        value /= 10;
    }
    
    /* Reverse into output buffer */
    if (i >= buf_size) {
        i = buf_size - 1;
    }
    for (size_t j = 0; j < i; j++) {
        buffer[j] = temp[i - 1 - j];
    }
    
    return i;
}

/**
 * @brief Convert signed integer to decimal string
 * @param value Value to convert
 * @param buffer Output buffer
 * @param buf_size Size of output buffer
 * @return Number of characters written
 */
static size_t format_int(int value, char* buffer, size_t buf_size) {
    if (buf_size == 0) return 0;
    
    size_t pos = 0;
    
    /* Handle negative */
    if (value < 0) {
        buffer[pos++] = '-';
        if (pos >= buf_size) return pos;
        /* Convert to positive (handle INT_MIN edge case) */
        value = (value == -2147483647 - 1) ? 2147483647 : -value;
        if (value < 0) value = -(value + 1); /* Handle INT_MIN */
    }
    
    pos += format_uint((unsigned int)value, buffer + pos, buf_size - pos);
    return pos;
}

/**
 * @brief Convert unsigned integer to hexadecimal string
 * @param value Value to convert
 * @param buffer Output buffer
 * @param buf_size Size of output buffer
 * @param uppercase Use uppercase letters (A-F) if true
 * @return Number of characters written
 */
static size_t format_hex(unsigned int value, char* buffer, size_t buf_size,
                         bool uppercase) {
    if (buf_size == 0) return 0;
    
    const char* hex_chars = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    
    /* Handle zero specially */
    if (value == 0) {
        if (buf_size < 2) return 0;
        buffer[0] = '0';
        return 1;
    }
    
    /* Convert digits in reverse */
    char temp[9]; /* Max 8 hex digits for 32-bit + null */
    size_t i = 0;
    while (value > 0 && i < sizeof(temp) - 1) {
        temp[i++] = hex_chars[value & 0xF];
        value >>= 4;
    }
    
    /* Reverse into output buffer */
    if (i >= buf_size) {
        i = buf_size - 1;
    }
    for (size_t j = 0; j < i; j++) {
        buffer[j] = temp[i - 1 - j];
    }
    
    return i;
}

/**
 * @brief Copy string to buffer
 * @param str Source string
 * @param buffer Output buffer
 * @param buf_size Size of output buffer
 * @return Number of characters written
 */
static size_t copy_string(const char* str, char* buffer, size_t buf_size) {
    if (buf_size == 0 || str == NULL) return 0;
    
    size_t i;
    for (i = 0; i < buf_size - 1 && str[i] != '\0'; i++) {
        buffer[i] = str[i];
    }
    
    return i;
}

/**
 * @brief Format string with arguments (minimal sprintf-like functionality)
 * 
 * Supported format specifiers:
 *   %d, %i - signed integer
 *   %u     - unsigned integer
 *   %x     - lowercase hexadecimal
 *   %X     - uppercase hexadecimal
 *   %s     - string
 *   %c     - character
 *   %%     - literal %
 *
 * @param buffer Output buffer
 * @param buf_size Size of output buffer
 * @param fmt Format string
 * @param args Variable arguments
 * @return Number of characters written
 */
static size_t format_string(char* buffer, size_t buf_size, const char* fmt,
                           va_list args) {
    if (buffer == NULL || buf_size == 0 || fmt == NULL) {
        return 0;
    }
    
    size_t pos = 0;
    const char* p = fmt;
    
    while (*p != '\0' && pos < buf_size - 1) {
        if (*p == '%') {
            p++; /* Skip '%' */
            
            if (*p == '\0') break;
            
            switch (*p) {
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    pos += format_int(val, buffer + pos, buf_size - pos);
                    break;
                }
                
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    pos += format_uint(val, buffer + pos, buf_size - pos);
                    break;
                }
                
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    pos += format_hex(val, buffer + pos, buf_size - pos, false);
                    break;
                }
                
                case 'X': {
                    unsigned int val = va_arg(args, unsigned int);
                    pos += format_hex(val, buffer + pos, buf_size - pos, true);
                    break;
                }
                
                case 's': {
                    const char* str = va_arg(args, const char*);
                    if (str == NULL) {
                        str = "(null)";
                    }
                    pos += copy_string(str, buffer + pos, buf_size - pos);
                    break;
                }
                
                case 'c': {
                    char ch = (char)va_arg(args, int);
                    if (pos < buf_size - 1) {
                        buffer[pos++] = ch;
                    }
                    break;
                }
                
                case '%': {
                    if (pos < buf_size - 1) {
                        buffer[pos++] = '%';
                    }
                    break;
                }
                
                default:
                    /* Unknown format specifier - just copy it */
                    if (pos < buf_size - 2) {
                        buffer[pos++] = '%';
                        buffer[pos++] = *p;
                    }
                    break;
            }
            p++;
        } else {
            buffer[pos++] = *p++;
        }
    }
    
    return pos;
}

/*=============================================================================
 * Backend API
 *============================================================================*/

/* Output callback function pointer */
static log_output_callback_t output_callback = NULL;

void log_set_output_callback(log_output_callback_t callback) {
    output_callback = callback;
}

bool log_is_output_configured(void) {
    return (output_callback != NULL);
}

/*=============================================================================
 * Logging Implementation
 *============================================================================*/

/* Simplified implementation that avoids array initialization issues
 * in bare-metal environments without full C runtime */
static const char* LOG_LEVEL_TO_C_STRING(log_level_e level) {
    switch(level) {
        case LOG_LEVEL_OFF:      return "off";
        case LOG_LEVEL_CRITICAL: return "critical";
        case LOG_LEVEL_ERROR:    return "error";
        case LOG_LEVEL_WARNING:  return "warning";
        case LOG_LEVEL_INFO:     return "info";
        case LOG_LEVEL_DEBUG:    return "debug";
        default:                 return "unknown";
    }
}

/**
 * @brief Format the log level prefix into buffer
 * @return Number of characters written
 */
static size_t format_level_prefix(char* buffer, size_t buf_size, log_level_e level) {
    if (buf_size < 3) return 0; /* Need at least "[X] " */
    
    size_t pos = 0;
    buffer[pos++] = '[';
    
    const char* level_str = LOG_LEVEL_TO_C_STRING(level);
    pos += copy_string(level_str, buffer + pos, buf_size - pos);
    
    if (pos < buf_size - 2) {
        buffer[pos++] = ']';
        buffer[pos++] = ' ';
    }
    
    return pos;
}

void log_message(log_level_e level, const char* fmt, ...) {
    /* Check if output is configured */
    if (output_callback == NULL) {
        return;
    }
    
    /* Check format string */
    if (fmt == NULL) {
        return;
    }
    
    /* Format message into buffer */
    char buffer[LOG_MAX_MESSAGE_SIZE];
    size_t pos = 0;
    
    /* Format level prefix: "[info] " */
    pos += format_level_prefix(buffer + pos, sizeof(buffer) - pos, level);
    
    /* Format user message */
    va_list args;
    va_start(args, fmt);
    pos += format_string(buffer + pos, sizeof(buffer) - pos, fmt, args);
    va_end(args);
    
    /* Add newline */
    if (pos < sizeof(buffer) - 1) {
        buffer[pos++] = '\n';
    }
    
    /* Output via callback */
    output_callback(buffer, pos);
}
