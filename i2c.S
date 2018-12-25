.section .text

.global i2c_write_byte_body
i2c_write_byte_body:
	#get the &i2c_state, we don't care at this point
	#though SCLK is not set
	#move.l %sp@(4), %a0
	#get the byte to write
	#move.b %sp@(0xb), %d0
	move.b %sp@(7), %d0
	#get the I2C addr to write
	move.l #0x8002, %a1

#bit 7
	#rotate so data is in bit #1 (different to the rest)
	rol.b #2, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 6
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 5
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 4
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 3
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 2
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 1
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#bit 0
	#rotate so data is in bit #1
	rol.b #1, %d0
	move.b %d0, %d1

	#get SDA bit and write
	andi.b #2, %d1
	move.b %d1, %a1@

	#set SCLK and write
	addq.b #1, %d1
	move.b %d1, %a1@

	#clear SCLK and write
	andi.b #0xfe, %d1
	move.b %d1, %a1@

#now the nack bit
#SDA, SCLK, !SCLK
	clr.b %d1
	addq.b #2, %d1
	move.b %d1, %a1@

	addq.b #1, %d1
	move.b %d1, %a1@

	subq.b #1, %d1
	move.b %d1, %a1@

	#store back to i2c_state (just SDA high)
	#move.b %d1, %a0@

	rts
