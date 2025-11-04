# log-c

A minimal, lightweight logging library for C, ideal for embedded systems and small projects.

## Features

-   **Multiple Log Levels:** Supports `critical`, `error`, `warning`, `info`, and `debug` levels.
-   **Compile-Time Filtering:** Easily set the maximum log level at compile time to control binary size.
-   **Self-Contained:** No external dependencies (no printf library required).
-   **Custom Backends:** Redirect log output to any destination (e.g., serial port, file, memory buffer) via a simple callback API.
-   **Minimal Footprint:** Lightweight implementation with internal formatting (~1.8KB compiled size).
-   **Flexible Formatting:** Supports `%d`, `%u`, `%x`, `%X`, `%s`, `%c`, `%%` format specifiers.

## Getting Started

### Cloning the Repository

```bash
git clone https://github.com/your-username/log-c.git
cd log-c
```

### Building and Testing

To build the library and run the tests, simply use `make`:

```bash
make test
```

This will compile the `log-c` library and run the test suite to ensure everything is working correctly.

## Usage

### Basic Example

```c
#include "log_c.h"
#include <stdio.h>

// Output callback function
void my_output(const char* message, size_t length) {
    // Send to your output device (UART, file, etc.)
    fwrite(message, 1, length, stdout);
}

int main(void) {
    // Set the output callback before logging
    log_set_output_callback(my_output);
    
    // Use logging macros
    loginfo("Application started");
    logdebug("Debug value: %d", 42);
    logerror("Error occurred: %s", "example error");
    
    return 0;
}
```

### Output Backend Implementation

The library requires you to provide an output callback function that handles the formatted log messages. The callback signature is:

```c
typedef void (*log_output_callback_t)(const char* message, size_t length);
```

The callback receives:
- A pointer to the formatted message buffer (includes level prefix like `[info]` and newline)
- The length of the message in bytes

**Example for UART:**
```c
void uart_log_output(const char* message, size_t length) {
    for (size_t i = 0; i < length; i++) {
        uart_write(message[i]);
    }
}

int main(void) {
    uart_init();
    log_set_output_callback(uart_log_output);
    
    loginfo("UART logging initialized");
}
```

**Example for File:**
```c
void file_log_output(const char* message, size_t length) {
    FILE* log_file = fopen("app.log", "a");
    if (log_file) {
        fwrite(message, 1, length, log_file);
        fclose(log_file);
    }
}
```

## API Reference

### Logging Functions

- `logcritical(fmt, ...)` - Log critical messages (unrecoverable errors)
- `logerror(fmt, ...)` - Log error messages
- `logwarning(fmt, ...)` - Log warning messages
- `loginfo(fmt, ...)` - Log informational messages
- `logdebug(fmt, ...)` - Log debug messages

### Backend API

- `void log_set_output_callback(log_output_callback_t callback)` - Set the output callback
- `bool log_is_output_configured(void)` - Check if output callback is configured

### Configuration

```c
// Set maximum message buffer size (default: 256)
#define LOG_MAX_MESSAGE_SIZE 512
```

## Compile-Time Log Level

Control which log levels are compiled into your binary:

```c
// Set before including log_c.h
#define LOG_LEVEL LOG_LEVEL_DEBUG  // Include all messages
#include "log_c.h"
```

Available levels:
- `LOG_LEVEL_OFF` - Disable all logging
- `LOG_LEVEL_CRITICAL` - Only critical messages
- `LOG_LEVEL_ERROR` - Critical and error messages
- `LOG_LEVEL_WARNING` - Critical, error, and warning messages
- `LOG_LEVEL_INFO` - All except debug (default)
- `LOG_LEVEL_DEBUG` - All messages

Compile-time filtering completely eliminates unused log calls from the binary, saving code space.

## Format Specifiers

Supported format specifiers:
- `%d`, `%i` - Signed integer
- `%u` - Unsigned integer
- `%x` - Lowercase hexadecimal
- `%X` - Uppercase hexadecimal
- `%s` - String
- `%c` - Character
- `%%` - Literal percent sign

**Note:** Float, long long, and width specifiers are not supported to keep the library minimal.

## Migration from Previous Version

The library has been updated to remove the printf dependency and use a callback-based API.

**Old API (deprecated):**
```c
void my_putchar(char c) {
    uart_write(c);
}
logc_set_putchar(my_putchar);
```

**New API:**
```c
void my_output(const char* msg, size_t len) {
    for (size_t i = 0; i < len; i++) {
        uart_write(msg[i]);
    }
}
log_set_output_callback(my_output);
```

## Thread Safety

The library itself is thread-safe for logging (no shared mutable state). However, your output callback must be thread-safe if you plan to log from multiple threads or interrupt contexts.

## Code Size

The self-contained log-c library compiles to approximately **1.8KB** (ARM Cortex-M4, -O0), compared to ~4-6KB when using printf. This makes it ideal for resource-constrained embedded systems.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
