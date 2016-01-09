# coding=utf-8

prm = {}

prm[0x412]={
	"Device" : "Low-density",
	"Flash erase unit" : "page",
	"Flash page size" : 0x400,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08008000}

prm[0x410]={
	"Device" : "Medium-density",
	"Flash erase unit" : "page",
	"Flash page size" : 0x400,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08020000}

prm[0x414]={
	"Device" : "High-density",
	"Flash erase unit" : "page",
	"Flash page size" : 0x800,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08080000}

prm[0x418]={
	"Device" : "Connectivity line",
	"Flash erase unit" : "page",
	"Flash page size" : 0x800,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08040000}

prm[0x420]={
	"Device" : "Medium-density value line",
	"Flash erase unit" : "page",
	"Flash page size" : 0x400,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08020000}

prm[0x428]={
	"Device" : "High-density value line",
	"Flash erase unit" : "page",
	"Flash page size" : 0x800,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08080000}

prm[0x430]={
	"Device" : "XL-density",
	"Flash erase unit" : "page",
	"Flash page size" : 0x800,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08100000}

prm[0x416]={
	"Device" : "Medium-density ultralow power line",
	"Flash erase unit" : "page",
	"Flash page size" : 0x100,
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08020000}

prm[0x411]={
	"Device" : "STM32F2xx device",
	"Flash erase unit" : "sector",
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08100000}

prm[0x413]={
	"Device" : "STM32F4xx device",
	"Flash erase unit" : "sector",
	"Flash start addr" : 0x08000000,
	"Flash end addr"   : 0x08100000}

sector=[
	{"start" : 0x08000000, "size" :  16<<10}, # sector 0
	{"start" : 0x08004000, "size" :  16<<10}, # sector 1
	{"start" : 0x08008000, "size" :  16<<10}, # sector 2
	{"start" : 0x0800C000, "size" :  16<<10}, # sector 3
	{"start" : 0x08010000, "size" :  64<<10}, # sector 4
	{"start" : 0x08020000, "size" : 128<<10}, # sector 5
	{"start" : 0x08040000, "size" : 128<<10}, # sector 6
	{"start" : 0x08060000, "size" : 128<<10}, # sector 7
	{"start" : 0x08080000, "size" : 128<<10}, # sector 8
	{"start" : 0x080A0000, "size" : 128<<10}, # sector 9
	{"start" : 0x080C0000, "size" : 128<<10}, # sector 10
	{"start" : 0x080E0000, "size" : 128<<10}, # sector 11
]

def show_device_info(PID):
	print "Device: %s (PID=0x%04x)" % (prm[PID]["Device"], PID)
	print "    Flash erase unit: %s" % prm[PID]["Flash erase unit"]
	if prm[PID]["Flash erase unit"] == "page":
		print "    Flash page size: %d bytes" % prm[PID]["Flash page size"]

def in_flash(PID, addr):
	start_addr = prm[PID]["Flash start addr"]
	end_addr = prm[PID]["Flash end addr"] - 1
	if start_addr <= addr and addr <= end_addr:
		return True
	else:
		return False

def page_num(PID, addr):
	page_size = prm[PID]["Flash page size"]
	start_addr = prm[PID]["Flash start addr"]
	end_addr = prm[PID]["Flash end addr"] - 1
	if addr < start_addr:
		return 0
	elif addr > end_addr:
		return (end_addr - start_addr) // page_size
	else:
		return (addr - start_addr) // page_size

def sector_num(PID, addr):
	start_addr = prm[PID]["Flash start addr"]
	end_addr = prm[PID]["Flash end addr"] - 1
	if addr < start_addr:
		return 0
	elif addr > end_addr:
		return len(sector) - 1
	for i in range(len(sector)):
		if sector[i]["start"] <= addr and \
		   addr < sector[i]["start"] + sector[i]["size"]:
			return i

