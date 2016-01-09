# coding=utf-8

import serial

cmd_code = {}
cmd_code["Get"] = chr(0x00)
cmd_code["Get_Version_And_Read_Protection_Status"] = chr(0x01)
cmd_code["Get_ID"] = chr(0x02)
cmd_code["Read_Memory"] = chr(0x11)
cmd_code["Go"] = chr(0x21)
cmd_code["Write_Memory"] = chr(0x31)
cmd_code["Erase"] = chr(0x43)
cmd_code["Extended_Erase"] = chr(0x44)
cmd_code["Write_Protect"] = chr(0x63)
cmd_code["Write_Unprotect"] = chr(0x73)
cmd_code["Readout_Protect"] = chr(0x82)
cmd_code["Readout_Unprotect"] = chr(0x92)

for x in cmd_code.keys():
	code = (~ord(cmd_code[x])) & 0xFF
	cmd_code[x] += chr(code)

ACK = chr(0x79)
NACK = chr(0x1f)
N_OF_SUPPORTED_COMMANDS = 11
TRANSFER_BYTES_MAX = 256

def Connect(ser):
	ser.write(chr(0x7f))
	ret = ser.read()
	if not ret:
		return 1
	elif ret == ACK:
		return 0
	else:
		return 2


def wait_for_ACK(ser):
	ret = ser.read()
	if not ret:
		raise Exception("No response from the device when waiting for ACK")
	elif ret == NACK:
		raise Exception("NACKed")
	elif ret == ACK:
		return
	else:
		raise Exception("Received a different letter when waiting for ACK: " + str(ord(ret)))


def Get(ser):
	ser.write(cmd_code["Get"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Receive the number of bytes (version + commands)
	n = ord(ser.read())
	if n != N_OF_SUPPORTED_COMMANDS:
		raise Exception("Number of supported commands is wrong: " + str(n))

	# Receive the bootloader version
	version = ord(ser.read())
	
	# Receive the supported commands
	c = ser.read(n)
	commands = []
	for x in c:
		commands.append(ord(x))

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	return (version, commands)


def Get_Version_And_Read_Protection_Status(ser):
	ser.write(cmd_code["Get_Version_And_Read_Protection_Status"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Receive the bootloader version
	version = ord(ser.read())
	# Receive the number of times the read protection was disabled
	option_bytes = []
	option_bytes.append(ord(ser.read()))
	# Receive the number of times the read protection was enabled
	option_bytes.append(ord(ser.read()))

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Get_ID(ser):
	ser.write(cmd_code["Get_ID"])
	
	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Receive number of bytes
	n = ord(ser.read())
	if n != 1:
		raise Exception("N is not equal to 1" + str(n))

	# Receive PID (Product ID)
	byte0 = ord(ser.read())
	byte1 = ord(ser.read())
	PID = (byte0 << 8) + byte1

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	return PID

def Read_Memory(ser, addr, n):
	# データバイト数が仕様の範囲内にあるかチェック
	if n < 1 or n > 256:
		raise Exception("number of data is invalid" + str(n))

	ser.write(cmd_code["Read_Memory"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Send the start address (4bytes) with checksum
	b = []
	b.append((addr&0xff000000)>>24) # MSB
	b.append((addr&0x00ff0000)>>16)
	b.append((addr&0x0000ff00)>>8)
	b.append((addr&0x000000ff)) # LSB
	checksum = 0
	c = ""
	for x in b:
		checksum ^= x
		c += chr(x)
	c += chr(checksum)
	ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Send the number of bytes to be read (1byte) and a checksum (1byte)
	checksum = (~(n-1)) & 0xff
	c = chr(n-1)
	c += chr(checksum)
	ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)
	
	# Receive data from the BL (BootLoader)
	data = ser.read(n)

	return data


def Go(ser, addr):
	ser.write(cmd_code["Go"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Send the Start Address (4bytes) & checksum
	b = []
	b.append((addr&0xff000000)>>24) # MSB
	b.append((addr&0x00ff0000)>>16)
	b.append((addr&0x0000ff00)>>8)
	b.append((addr&0x000000ff)) # LSB
	checksum = 0
	c = ""
	for x in b:
		checksum ^= x
		c += chr(x)
	c += chr(checksum)
	ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)
	# Wait for ACK or NACK
	#wait_for_ACK(ser)


def Write_Memory(ser, addr, data):
	n = len(data)
	# データバイト数が仕様の範囲内にあるかチェック
	if n < 1 or n > 256:
		raise Exception("number of data is invalid: " + str(n))
	# アドレスは32-bit alignedでなければならない
	if (addr & 0x3) != 0:
		raise Exception("write address must be 32-bit aligned" + str(addr))
	# 書き込みサイズは4の倍数でなければならない
	if (n % 4) != 0:
		raise Exception("write size must be a multiple of 4" + str(n))

	ser.write(cmd_code["Write_Memory"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Send the start address (4bytes) & checksum
	b = []
	b.append((addr&0xff000000)>>24) # MSB
	b.append((addr&0x00ff0000)>>16)
	b.append((addr&0x0000ff00)>>8)
	b.append((addr&0x000000ff)) # LSB
	checksum = 0
	c = ""
	for x in b:
		checksum ^= x
		c += chr(x)
	c += chr(checksum)
	ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Send the number of bytes to be written (1byte), the data (n bytes) and checksum
	checksum = n-1
	c = chr(n-1)
	for x in data:
		checksum ^= ord(x)
		c += x
	c += chr(checksum)
	ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Erase_Memory(ser, pages, n=0):
	if n == 0:
		n = len(pages)
	# 消去するページ数が仕様の範囲内にあるかチェック
	if n<1 or n>256:
		raise Exception("number of pages is invalid: " + str(n))
	
	ser.write(cmd_code["Erase"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)
	
	# Global Erase?
	if n == 256:
		ser.write(chr(0xFF))
		ser.write(chr(0x00))
	else:
		# Send the number of pages to be erased (1byte)
		checksum = n-1
		c = chr(n-1)
		# Send the page numbers
		for x in pages:
			checksum ^= x
			c += chr(x)
		# Send checksum
		c += chr(checksum)
		ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Extended_Erase_Memory(ser, pages, n=0):
	if n == 0:
		n = len(pages)
	# 消去するページ数が仕様の範囲内にあるかチェック
	if n<1 or n>0xFFFF:
		raise Exception("number of pages is invalid" + str(n))
	
	ser.write(cmd_code["Extended_Erase"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)
	
	# Special Erase?
	if (n & 0xFFF0) == 0xFFF0:
		# Send Special Erase cmd
		# Send the checksum of the two bytes
		b = []
		b.append((n&0xFF00)>>8)
		b.append((n&0x00FF))
		checksum = 0
		c = ""
		for x in b:
			checksum ^= x
			c += chr(x)
		c += chr(checksum)
		ser.write(c)
	else:
		checksum = 0
		# Send the number of pages to be erased N (on two bytes) MSB first
		b = []
		b.append(((n-1)&0xFF00)>>8)
		b.append(((n-1)&0x00FF))
		c = ""
		for x in b:
			checksum ^= x
			c += chr(x)
		# Send the page numbers (each on two bytes, MSB first)
		for page in pages:
			b = []
			b.append((page&0xFF00)>>8)
			b.append((page&0x00FF))
			for x in b:
				checksum ^= x
				c += chr(x)
		# send byte checksum of all sent bytes
		c += chr(checksum)
		ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Write_Protect(ser, sectors):
	n = len(sectors)

	ser.write(cmd_code["Write_Protect"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Send the number of sectors to be protected (1byte)
	checksum = n-1
	c = chr(n-1)
	# Send the sector codes
	for x in sectors:
		checksum ^= x
		c += chr(x)
	# Send checksum
	c += chr(checksum)
	ser.write(c)

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Write_Unprotect(ser):
	ser.write(cmd_code["Write_Unprotect"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Readout_protect(ser):
	ser.write(cmd_code["Readout_Protect"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Wait for ACK or NACK
	wait_for_ACK(ser)


def Readout_Unprotect(ser):
	ser.write(cmd_code["Readout_Unprotect"])

	# Wait for ACK or NACK
	wait_for_ACK(ser)

	# Wait for ACK or NACK
	wait_for_ACK(ser)

