#addr 0
#include "../rules.asm"

mov ab, 0x5000
mov.b [ab], "A"
halt
