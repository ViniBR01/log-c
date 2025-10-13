# log-c

This repository contains a minimal logging library which is appropriate for usage in embedded systems.

## Features (Wishlist)

- Supports standard format strings (like printf, but with minimal features to minimize footprint)
- Use logging levels that match the [Python logger](https://docs.python.org/3/library/logging.html)
  - Log output has a tag for the level, e.g. <error>
  - Log levels are filterable at compile-time and run-time
- Provide the ability to add method name + file + line number to the log statement
- Displays a timestamp in log messages
  - Initially to be milliseconds from boot
  - Ideally user-specifiable, with a long-term goal to support an RTC with HH:MM:SS format
- Supports multiple output streams, selectable by the user. Examples may include:
  - Serial port
  - Buffer in memory
  - FLASH
- Option to echo log statements over the Serial port (or other Print class configured for use printf())
- Support automatic rotation of log files

## Backend replacement (custom printf / output)

This library supports replacing the formatting/output backend at runtime. The
API exposes a `struct logc_backend` with three function pointers:

- `int (*vprintf)(const char *fmt, va_list ap);` — format using a `va_list`.
- `size_t (*write)(const char *buf, size_t len);` — write raw bytes to the sink.
- `int (*putchar)(int ch);` — put a single character to the sink.

The library provides a default backend that uses the C standard library
(`vprintf`, `write`/`fwrite`, `putchar`). Tests and user code can call
`logc_set_backend()` to install a custom backend. To restore the default
backend, call `logc_set_backend(NULL)`.

### Example: register a backend that writes into an in-memory buffer

```c
#include "log_c.h"
#include <stdarg.h>
#include <string.h>

static char mybuf[1024];
static size_t mypos = 0;

static int my_vprintf(const char *fmt, va_list ap) {
    int n = vsnprintf(mybuf + mypos, sizeof(mybuf) - mypos, fmt, ap);
    if (n > 0) mypos += (size_t)n;
    return n;
}

static size_t my_write(const char *buf, size_t len) {
    size_t to_copy = len;
    if (mypos + to_copy >= sizeof(mybuf)) to_copy = sizeof(mybuf) - 1 - mypos;
    memcpy(mybuf + mypos, buf, to_copy);
    mypos += to_copy;
    mybuf[mypos] = '\0';
    return to_copy;
}

static int my_putchar(int ch) {
    if (mypos + 1 >= sizeof(mybuf)) return EOF;
    mybuf[mypos++] = (char)ch;
    mybuf[mypos] = '\0';
    return ch;
}

void install_my_backend(void) {
    struct logc_backend b = { .vprintf = my_vprintf, .write = my_write, .putchar = my_putchar };
    logc_set_backend(&b);
}

// Call install_my_backend() early in your program (before threads).
```

Notes:

- Use the `vprintf`-style function pointer (takes `va_list`) — pointers to
  variadic functions are non-portable.
- `logc_set_backend()` is not thread-safe in this version; set the backend
  once during initialization or add locking/atomic swap if you need runtime
  replacement in a multithreaded program.

## License

MIT License. See [LICENSE](LICENSE) for details.

---

**Note:** This project is for educational purposes

# TODO:
- Add state to the logger and allow changing the state at run time
