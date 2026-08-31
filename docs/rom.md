# YETI-8 ROM/Monitor

The ROM contains an operating system called monitor, which provides useful functions and
a simple command line (not implemented yet)

Its calls start at the address 0x04.

Parameters are stored in A, B, C, D, E. A result may be stored in A. All registers are used
as scratch registers.

## Calls
| Address | Function         | A          | B          | C          | D          | E          | Result  |
| ------- | ---------------- | ---------- | ---------- | ---------- | ---------- | ---------- | ------- |
| 0x0004  | `print_at`       | AB=addr    |            | CD=string  |            | length     |         |
| 0x0006  | `key_load_state` | AB=dest    |            |            |            |            |         |
