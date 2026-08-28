#addr 0

jmp start

#d 0x00 ; padding
ref print_at

start:
	; set up stack
	mov sp, 0xC000

	; set up keyboard
	mov   ab,   0xD002
	mov.b [ab], 0

	mov ab, 0xC000
	mov c,  64
	mov ef, header

.printHeader:
	mov g, [ef]
	mov [ab], g
	inc ef
	inc ab
	dec c
	jnz .printHeader

.printRam:
	; TODO: detect RAM expansion
	mov ab, 0xC060
	mov cd, ram_4k
	mov e,  6
	call print_at

	mov   ab, 0xC080
	mov.b [ab], ">"

loop:
	mov cd, 0xD000
	mov a, 0
	mov [cd], a
	mov a, [cd]
	jz loop

	mov a, "A"
	mov cd, 0xC0A0
	mov [cd], a
	
	jmp loop

halt

header: #d " * * * *  YETI-8 ROM * * * * *  "
        #d "         MONITOR V0.1           "

ram_4k:  #d "4K RAM"
ram_16k: #d "16K EXPANDED RAM"

; API
print_at:
	; AB = addr
	; CD = string
	; E  = len
	mov g, [cd]
	mov [ab], g
	inc ab
	inc cd
	dec e
	jnz print_at
	ret
