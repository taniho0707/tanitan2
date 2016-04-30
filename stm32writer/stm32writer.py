#!/usr/bin/python2
# coding=utf-8

import sys, optparse, datetime
import stm32_serial, stm32_cmd, stm32_data, stm32_prm

def print_time(t):
	seconds = t.days * 3600 * 24 + t.seconds + t.microseconds * 1.E-6
	print "        time: %.3f s" % seconds

try:
	# バージョン情報
	version="120318py"
	print "stm32writer (version: " + version + ")"


	# オプションの解析
	parser = optparse.OptionParser()

	parser.add_option(
		"-p", "--port",
		action="store",
		dest="port",
		default="",
		help="set serial port")
	parser.add_option(
		"-r", "--baudrate",
		action="store",
		dest="baudrate",
		default=115200,
		help="set baudrate")
	parser.add_option(
		"-l", "--show-command-list",
		action="store_true",
		dest="command_list_flag",
		default=False,
		help="show command list")
	parser.add_option(
		"-o", "--show-option-bytes",
		action="store_true",
		dest="option_byte_flag",
		default=False,
		help="show option bytes")
	parser.add_option(
		"-u", "--unprotect",
		action="store_true",
		dest="unprotect_flag",
		default=False,
		help="write unprotect")
	parser.add_option(
		"-c", "--compare",
		action="store_true",
		dest="compare_flag",
		default=False,
		help="compare written data with original mot data")
	parser.add_option(
		"-g", "--go",
		action="store_true",
		dest="jump_flag",
		default=False,
		help="jump to start address after writing")
	parser.add_option(
		"-e", "--show-erase-page",
		action="store_true",
		dest="erase_page_flag",
		default=False,
		help="show page/sector numbers erased before writing to FLASH")
	parser.add_option(
		"-t", "--time",
		action="store_true",
		dest="time_flag",
		default=False,
		help="print time after the start of connection with a device")
	parser.add_option(
		"-b", "--binary",
		action="store_true",
		dest="binary_flag",
		default=False,
		help="analyze input file as binary file")

	(options, args) = parser.parse_args()
	if not args:
		print "no mot file"
		sys.exit(1)
	
	# ポートを開く
	ser = stm32_serial.open_port(options.port, options.baudrate)

	# STM32へ接続
	stm32_serial.connect_to_device(ser)

	t0 = datetime.datetime.now()
	if options.time_flag:
		print_time(datetime.timedelta(0))

	# Bootloaderのバージョンと使用できるコマンドリストを取得する
	(BL_version, commands) = stm32_cmd.Get(ser)

	# Bootloaderのバージョンを表示する
	print "Bootloader version: V%d.%d" % (BL_version>>4, BL_version&0x0f)

	# Product IDを取得する
	PID = stm32_cmd.Get_ID(ser)

	# Product IDに基づきデバイス情報を表示する
	stm32_prm.show_device_info(PID)

	# コマンドリストの表示
	if options.command_list_flag:
		print "Supported commands:"
		for x in commands:
			print "0x%02x" % x

	# option byteを読み込む
	if options.option_byte_flag:
		(BL_version, option_bytes) = stm32_cmd.Get_Version_And_Read_Protection_Status(ser)
		print "Bootloader version: V%d.%d" % (BL_version>>4, BL_version&0x0f)
		print "Option Byte 1: %d" % option_bytes[0]
		print "Option Byte 2: %d" % option_bytes[1]

	# 書き込みプロテクト解除
	if options.unprotect_flag:
		stm32_cmd.Write_Unprotect(ser)
		print "Write protect was unprotected"

	for filename in args:
                if options.binary_flag:
                        (addr, data, start_addr) = stm32_data.analyze_bin_file(filename, 0x08000000)

                        if options.time_flag:
			        print_time(datetime.datetime.now() - t0)

		        # 書き込み予定領域のページ番号，セクター番号を取得
			(pages, sectors) = stm32_data.make_erase_page_list(addr, data, PID)
			if options.erase_page_flag:
				if pages:
					print "Pages to be erased:",
					for x in pages:
						print " " + str(x),
					print
				if sectors:
					print "Sectors to be erased:",
					for x in sectors:
						print " " + str(x),
					print
			
			# 消去する
			stm32_data.erase_flash(ser, pages, sectors, commands)
	
			if options.time_flag:
				print_time(datetime.datetime.now() - t0)
	
			# 書き込み
			stm32_data.write_data(ser, addr, data)
	
			if options.time_flag:
				print_time(datetime.datetime.now() - t0)
	
			# 書き込んだデータとオリジナルの比較
			if options.compare_flag:
				stm32_data.compare_data(ser, addr, data)
	
				if options.time_flag:
					print_time(datetime.datetime.now() - t0)

                else :
		        # motファイルからデータ抽出
		        (addr, data, start_addr) = stm32_data.analyze_mot_file(filename)
		        # データレコード再構成
		        (addr, data) = stm32_data.reconstruct_records(addr, data)

		        if options.time_flag:
			        print_time(datetime.datetime.now() - t0)

		        # 書き込み予定領域のページ番号，セクター番号を取得
			(pages, sectors) = stm32_data.make_erase_page_list(addr, data, PID)
			if options.erase_page_flag:
				if pages:
					print "Pages to be erased:",
					for x in pages:
						print " " + str(x),
					print
				if sectors:
					print "Sectors to be erased:",
					for x in sectors:
						print " " + str(x),
					print
			
			# 消去する
			stm32_data.erase_flash(ser, pages, sectors, commands)
	
			if options.time_flag:
				print_time(datetime.datetime.now() - t0)
	
			# 書き込み
			stm32_data.write_data(ser, addr, data)
	
			if options.time_flag:
				print_time(datetime.datetime.now() - t0)
	
			# 書き込んだデータとオリジナルの比較
			if options.compare_flag:
				stm32_data.compare_data(ser, addr, data)
	
				if options.time_flag:
					print_time(datetime.datetime.now() - t0)

		print

	# スタートアドレスにジャンプ
	if options.jump_flag:
		stm32_cmd.Go(ser, start_addr)
		print "Jumped to 0x%08x" % start_addr

	# ポートを閉じる
	stm32_serial.close_port(ser)

except KeyboardInterrupt:
	print
	sys.exit(1)

