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

## License

MIT License. See [LICENSE](LICENSE) for details.

---

**Note:** This project is for educational purposes