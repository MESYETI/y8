#addr 0

_start:
	mov sp, 0xC000

	mov ab, 0xC000
	mov c,  64
	mov ef, header

.loop:
	mov g, [ef]
	mov [ab], g
	inc ef
	inc ab
	dec c
	jnz .loop
	halt

header: #d " * * * *  YETI-8 ROM * * * * *  "
        #d "         MONITOR V0.1           "
