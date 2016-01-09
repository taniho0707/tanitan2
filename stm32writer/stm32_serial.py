# coding=utf-8

import sys, time
import serial
import stm32_cmd

def open_port(port, baudrate):
	target_port = port
	while True:
		try:
			# 8ビット，1ストップビット，偶数パリティ，
			ser = serial.Serial(target_port, baudrate,
				bytesize=serial.EIGHTBITS,
				parity=serial.PARITY_EVEN,
				stopbits=serial.STOPBITS_ONE)
			if not ser.isOpen():
				raise Exception("Port was not open")
			break
		except serial.SerialException:
			print "Could not open port: " + target_port
			target_port = raw_input("Input another port> ")
			if not target_port:
				target_port = port

	print "Opened %s (baudrate: %d)" % (ser.portstr, ser.baudrate)
	ser.flushInput()
	ser.flushOutput()
	return ser


def close_port(ser):
	ser.close()
	if ser.isOpen():
		raise Exception("Port was not closed")
	print "Closed %s" % ser.portstr


def connect_to_device(ser):
	print "Connecting...",
	sys.stdout.flush()
	# 複数回接続を試みるためにタイムアウトを0.5sにする
	timeout_orig = ser.timeout
	parity_orig = ser.parity
	ser.timeout = 0.5
	ser.parity = serial.PARITY_NONE
	while True:
		ret = stm32_cmd.Connect(ser)
		if ret == 0:
			break
		elif ret == 1:
			sys.stdout.write(".")
			sys.stdout.flush()
		elif ret == 2:
			sys.stdout.write("x")
			sys.stdout.flush()
			time.sleep(0.1)
			ser.flushInput()
	print "OK"
	# タイムアウトを元に戻す
	ser.timeout = timeout_orig
	ser.parity = parity_orig

