# log-c

A minimal, lightweight logging library for C, ideal for embedded systems and small projects.

## Features

-   **Multiple Log Levels:** Supports `critical`, `error`, `warning`, `info`, and `debug` levels.
-   **Compile-Time Filtering:** Easily set the maximum log level at compile time to control binary size.
-   **Custom Backends:** Redirect log output to any destination (e.g., serial port, file, memory buffer) by implementing a simple `putchar` function.
-   **Minimal Dependencies:** Designed to be lightweight and have a small footprint.

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

## Custom Backend Implementation

By default, `log-c` writes log messages to `stdout`. You can easily customize the output by providing your own `putchar` function.

1.  **Create a `putchar` function:** This function will be called for each character of the log message.

    ```c
    #include "log_c.h"
    #include <stdio.h>

    // Example: A simple putchar that writes to a file
    void my_file_putchar(char character) {
        FILE* log_file = fopen("my_log.txt", "a");
        if (log_file) {
            fputc(character, log_file);
            fclose(log_file);
        }
    }
    ```

2.  **Set the custom `putchar` function:** Use `logc_set_putchar` to register your function. This should be done early in your program's initialization.

    ```c
    int main(void) {
        // Set the custom putchar before any logging calls
        logc_set_putchar(my_file_putchar);

        // Now, all log messages will be written to "my_log.txt"
        loginfo("This is a test message.");

        return 0;
    }
    ```

    **Note:** `logc_set_putchar()` is not thread-safe. It should be called once during initialization.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.