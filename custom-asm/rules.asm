#once

#subruledef r8
{
	a => 0b000
	b => 0b001
	c => 0b010
	d => 0b011
	e => 0b100
	f => 0b101
	g => 0b110
	h => 0b111
}

#subruledef r16
{
	ab => 0b00
	cd => 0b01
	ef => 0b10
	sp => 0b11
}

#ruledef
{
	halt                            => 0x00
	mov {rd: r8}, {rs: r8}          => 0x10 @ rd @ rs @ 0b00
	mov {pd: r16}, {ps: r16}        => 0x11 @ pd @ ps @ 0b0000
	mov {rd: r8}, {value: u8}       => 0x12 @ rd @ 0b00000 @ value
	mov {pd: r16}, {value: u16}     => 0x13 @ pd @ 0b000000 @ $le(value)
	mov {rd: r8}, [{ps: r16}]       => 0x14 @ rd @ ps @ 0b000
	mov {pd: r16}, [{ps: r16}]      => 0x15 @ pd @ ps @ 0b0000
	mov {rd: r8}, [{value: u16}]    => 0x17 @ rd @ 0b00000 @ $le(value)
	mov {pd: r16}, [{value: u16}]   => 0x18 @ pd @ 0b000000 @ $le(value)
	mov [{pd: r16}], {rs: r8}       => 0x19 @ pd @ rs @ 0b000
	mov [{pd: r16}], {ps: r16}      => 0x1A @ pd @ ps @ 0b000
	mov.b [{pd: r16}], {value: u8}  => 0x1B @ pd @ 0b000000 @ value
	mov.w [{pd: r16}], {value: u16} => 0x1C @ pd @ 0b000000 @ $le(value)
}
