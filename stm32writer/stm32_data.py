# coding=utf-8

import sys
import stm32_cmd, stm32_prm

def analyze_mot_file(filename):
	print 'Reading "%s"' % filename
	f = open(filename)

	addr = []
	data = []
	start_addr = 0
	
	for line in f:
		line = line.rstrip()
		record_type = line[0:2]

		if record_type == "S0":
			continue

		elif record_type == "S1":
			n_bytes = int(line[2:4], 16)
			addr.append(int(line[4:8], 16))
			line = line[8:]
			data_line = ""
			for i in range(n_bytes-2-1):
				byte = line[i*2:i*2+2]
				data_line += chr(int(byte, 16))
			data.append(data_line)

		elif record_type == "S2":
			n_bytes = int(line[2:4], 16)
			addr.append(int(line[4:10], 16))
			line = line[10:]
			data_line = ""
			for i in range(n_bytes-3-1):
				byte = line[i*2:i*2+2]
				data_line += chr(int(byte, 16))
			data.append(data_line)

		elif record_type == "S3":
			n_bytes = int(line[2:4], 16)
			addr.append(int(line[4:12], 16))
			line = line[12:]
			data_line = ""
			for i in range(n_bytes-4-1):
				byte = line[i*2:i*2+2]
				data_line += chr(int(byte, 16))
			data.append(data_line)

		elif record_type == "S4":
			continue

		elif record_type == "S5":
			continue

		elif record_type == "S6":
			continue

		elif record_type == "S7":
			n_bytes = int(line[2:4], 16)
			start_addr = (int(line[4:12], 16))

		elif record_type == "S8":
			n_bytes = int(line[2:4], 16)
			start_addr = (int(line[4:10], 16))

		elif record_type == "S9":
			n_bytes = int(line[2:4], 16)
			start_addr = (int(line[4:8], 16))
		else:
			continue

	print "    Number of data records: %d" % len(addr)
	print "    Start address: 0x%08x" % start_addr
	f.close()

	return (addr, data, start_addr)


def analyze_bin_file(filename, staddr):
	print 'Reading "%s"' % filename
	f = open(filename)

        addr = []
	data = []
	start_addr = staddr

	buf = f.read()

        for i in range(len(buf)/16):
                line = buf[16*i:16*i+16]
                data.append(line)
                addr.append(start_addr+16*i)

	print "    Number of data bytes: %d" % len(data)
	print "    Start address: 0x%08x" % start_addr
	f.close()
	return (addr, data, start_addr)


def reconstruct_records(addr, data):
	print "Reconstructing data records..."

	i = 0;
	while i < len(addr) - 1:
		# アドレスが連続しているときはレコードをまとめる
		if addr[i] + len(data[i]) == addr[i+1]:
			# 次のレコードを現在のレコードに付け加える
			# 次のレコード全て現在のレコードに付け加えられるとき
			if len(data[i]) + len(data[i+1]) <= stm32_cmd.TRANSFER_BYTES_MAX:
				data[i] += data[i+1]
				del addr[i+1]
				del data[i+1]
			# 次のレコードの一部を現在のレコードに付け加える
			else:
				n = stm32_cmd.TRANSFER_BYTES_MAX - len(data[i])
				data[i] += data[i+1][:n]
				data[i+1] = data[i+1][n:]
				addr[i+1] += n
				i += 1
		else:
			i += 1

	print "    Number of data records: %d" % len(addr)
	return (addr, data)


def make_erase_page_list(addr, data, PID):
	pages = []
	sectors = []
	# 消去がページ単位のデバイスの場合
	if stm32_prm.prm[PID]["Flash erase unit"] == "page":
		# 消去が必要なページをチェックする
		for i in range(len(addr)):
			start_addr = addr[i]
			end_addr = addr[i] + len(data[i]) - 1
			# データ書き込み先がflash領域ではないとき
			if (not stm32_prm.in_flash(PID, start_addr)) and \
			   (not stm32_prm.in_flash(PID, end_addr)):
				continue
			# レコード1バイト目が属するページ番号
			page_first = stm32_prm.page_num(PID, start_addr)
			# レコード最後のバイトが属するページ番号
			page_last = stm32_prm.page_num(PID, end_addr)
			# 2つの番号間のページを消去対象にする
			for x in [page_first + j for j in range(page_last - page_first + 1)]:
				if not x in pages:
					pages.append(x)
	# 消去がセクター単位のデバイス
	elif stm32_prm.prm[PID]["Flash erase unit"] == "sector":
		# 消去が必要なセクターをチェックする
		for i in range(len(addr)):
			start_addr = addr[i]
			end_addr = addr[i] + len(data[i]) - 1
			# データ書き込み先がflash領域ではないとき
			if (not stm32_prm.in_flash(PID, start_addr)) and \
			   (not stm32_prm.in_flash(PID, end_addr)):
				continue
			# レコード1バイト目が属するセクター番号
			sector_first = stm32_prm.sector_num(PID, start_addr)
			# レコード最後のバイトが属するセクター番号
			sector_last = stm32_prm.sector_num(PID, end_addr)
			# 2つの番号間のページを消去対象にする
			for x in [sector_first + j for j in range(sector_last - sector_first + 1)]:
				if not x in sectors:
					sectors.append(x)
	return pages, sectors


def erase_flash(ser, pages, sectors, commands):
	if pages:
		print "Erasing %d FLASH pages..." % len(pages)
		if ord(stm32_cmd.cmd_code["Erase"][0]) in commands:
			stm32_cmd.Erase_Memory(ser, pages)
		elif ord(stm32_cmd.cmd_code["Extended_Erase"][0]) in commands:
			stm32_cmd.Extended_Erase_Memory(ser, pages)
		else:
			raise Exception("Erase command not available")

	if sectors:
		print "Erasing %d FLASH sectors ..." % len(sectors)
		if ord(stm32_cmd.cmd_code["Erase"][0]) in commands:
			stm32_cmd.Erase_Memory(ser, sectors)
		elif ord(stm32_cmd.cmd_code["Extended_Erase"][0]) in commands:
			stm32_cmd.Extended_Erase_Memory(ser, sectors)
		else:
			raise Exception("Erase command not available")


def write_data(ser, addr, data):
	print "Writing data..."

	written = 0;
	# 書き込み
	for i in range(len(addr)):
		stm32_cmd.Write_Memory(ser, addr[i], data[i])
		written += len(data[i])
		sys.stdout.write("\r    %d bytes written" % written)
		sys.stdout.flush()
	print


def compare_data(ser, addr, data):
	print "Comparing data..."

	compared = 0
	# 読み込みと比較
	for i in range(len(addr)):
		read = stm32_cmd.Read_Memory(ser, addr[i], len(data[i]))
		if read != data[i]:
			raise Exception("Written and original data are different")
		compared += len(data[i])
		sys.stdout.write("\r    %d bytes compared" % compared)
		sys.stdout.flush()
	print

