#assembles programs for z80 cpu
#currently doesn't support all opcodes
#sample source files at bottom
#I should probably redo most of this so it's actually usable and supports all opcodes
import sys

def text_to_hex(text):
  return int('0x'+''.join(text),16)

def read_word(line):
  w = []
  while line[0] != ' ' and line[0] != '\n':
    w.append(line[0])
    line = line[1:]
  line = line[1:]
  return [''.join(w),line]

def read_line(line):
  ws = []
  while line != '\n' and line != '':
    ws.append(read_word(line)[0])
    line = read_word(line)[1]
  return ws

def read_file(filename):
  f = open(filename)
  lines = f.readlines()
  f.close()
  for line in lines:
	  if line == '\n' or line[0:2] == '//':
		  lines.pop(lines.index(line))
  if lines[-1][-1] != '\n':
	  lines[-1] = lines[-1] + '\n'
  lines.append('.stop')
  return lines
  
def read_sections(filename):
	code = read_file(filename)
	s = []
	for line in code:
		if line[0:1] == '.':
			section = [line[1:-1],code.index(line)]
			s.append(section)
	return s
	
def parse_sections(filename):
	code = read_file(filename)
	sections = read_sections(filename)
	for s in sections:
		if s[0] == 'title':
			write_title(code[s[1]+1])
		elif s[0] == 'non-default destination':
			program[0x14a] = 1
		elif s[0] == 'start':
			info = []
			i = s[1]+1
			while code[i][0] != '.':
				info.append(code[i])
				i += 1
			write_code(info,0x100)
		elif s[0][0:2] == '0x':
			info = []
			i = s[1]+1
			while code[i][0] != '.':
				info.append(code[i])
				i += 1
			write_code(info,text_to_hex(s[0][2:]))
		elif s[0] == 'stop':
			pass
	logo = [0xce,0xed,0x66,0x66,0xcc,0x0d,0x00,0x0b,0x03,0x73,0x00,0x83,0x00,0x0c,0x00,0x0d,0x00,0x08,0x11,0x1f,0x88,0x89,0x00,0x0e,0xdc,0xcc,0x6e,0xe6,0xdd,0xdd,0xd9,0x99,0xbb,0xbb,0x67,0x63,0x6e,0x0e,0xec,0xcc,0xdd,0xdc,0x99,0x9f,0xbb,0xb9,0x33,0x3e]
	i = 0
	for x in logo:
		program[0x104+i] = x
		i += 1
	x = 0
	for i in range(0x134,0x14d):
		x += program[i] + 1
	x = x & 0xff
	x = (x ^ 0xff) + 1
	program[0x14d] = x
			
def write_title(info):
	pc = 0x134
	if info[0:2] == '0x':
		info = info[2:]
	while info != '\n':
		program[pc] = text_to_hex(info[0:2])
		info = info[2:]
		pc += 1

def write_code(info,pc):
	for opcode in info:
		op = read_line(opcode)
		if op == []:
			pass
		elif op[0] == 'ld':
			if (op[1][0] == '$' or op[1][0] == '#') and (op[2][0] == '$' or op[2][0] == '#'):
				basedict = {'$b':0x40,'$c':0x48,'$d':0x50,'$e':0x58,'$h':0x60,'$l':0x68,'#hl':0x70,'$a':0x78}
				offsetdict = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
				program[pc] = basedict[op[1]]+offsetdict[op[2]]
				pc += 1
			elif (op[1][0] == '$' or op[1][0] == '#') and op[2][0:2] == '0x':
				dictionary = {'$bc':0x01,'$de':0x11,'$hl':0x21,'$sp':0x31,'$b':0x06,'$d':0x16,'$h':0x26,'#hl':0x36,'$c':0x0e,'$e':0x1e,'$l':0x2e,'$a':0x3e}
				program[pc] = dictionary[op[1]]
				pc += 1
				op[2] = op[2][2:][::-1]
				while op[2] != '':
					program[pc] = text_to_hex(op[2][0:2][::-1])
					pc += 1
					op[2] = op[2][2:]
		elif op[0] == 'rst':
			dictionary = {'0x00':0xc7,'0x08':0xcf,'0x10':0xc7,'0x18':0xdf,'0x20':0xe7,'0x28':0xef,'0x30':0xf7,'0x38':0xff}
			program[pc] = dictionary[op[1]]
			pc += 1
		elif op[0] == 'nop':
			program[pc] = 0x00
			pc += 1
		elif op[0] == 'stop':
			program[pc] = 0x10
			pc += 1
		elif op[0] == 'halt':
			program[pc] = 0x76
			pc += 1
		elif op[0] == 'ei':
			program[pc] = 0xfb
			pc += 1
		elif op[0] == 'di':
			program[pc] = 0xf3
			pc += 1
		elif op[0] == 'ret':
			program[pc] = 0xc9
			pc += 1
		elif op[0] == 'retz':
			program[pc] = 0xc8
			pc += 1
		elif op[0] == 'inc':
			dictionary = {'$bc' : 0x03, '$de' : 0x13, '$hl' : 0x23, '$sp' : 0x33, '$b' : 0x04, '$d' : 0x14, '$h' : 0x24, '#hl' : 0x34, '$c' : 0x0c, '$e' : 0x1c, '$l' : 0x2c, '$a' : 0x3c}
			program[pc] = dictionary[op[1]]
			pc += 1
		elif op[0] == 'dec':
			dictionary = {'$bc' : 0x0b, '$de' : 0x1b, '$hl' : 0x2b, '$sp' : 0x3b, '$b' : 0x05, '$d' : 0x15, '$h' : 0x25, '#hl' : 0x35, '$c' : 0x0d, '$e' : 0x1d, '$l' : 0x2d, '$a' : 0x3d}
			program[pc] = dictionary[op[1]]
			pc += 1
		elif op[0] == 'add':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0x80+dictionary[op[1]]
			pc += 1
		elif op[0] == 'adc':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0x88+dictionary[op[1]]
			pc += 1
		elif op[0] == 'sub':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0x90+dictionary[op[1]]
			pc += 1
		elif op[0] == 'sbc':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0x98+dictionary[op[1]]
			pc += 1
		elif op[0] == 'and':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0xa0+dictionary[op[1]]
			pc += 1
		elif op[0] == 'xor':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0xa8+dictionary[op[1]]
			pc += 1
		elif op[0] == 'or':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0xb0+dictionary[op[1]]
			pc += 1
		elif op[0] == 'cp':
			dictionary = {'$b':0,'$c':1,'$d':2,'$e':3,'$h':4,'$l':5,'#hl':6,'$a':7}
			program[pc] = 0xb8+dictionary[op[1]]
			pc += 1
		elif op[0] == 'pop':
			dictionary = {'$bc':0xc1,'$de':0xd1,'$hl':0xe1,'$af':0xf1}
			program[pc] = dictionary[op[1]]
			pc += 1
		elif op[0] == 'push':
			dictionary = {'$bc':0xc1,'$de':0xd1,'$hl':0xe1,'$af':0xf1}
			program[pc] = dictionary[op[1]] + 5
			pc += 1
		elif op[0] == 'jp':
			program[pc] = 0xc3
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'jpz':
			program[pc] = 0xca
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'jpnz':
			program[pc] = 0xc2
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'jpc':
			program[pc] = 0xda
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'jpnc':
			program[pc] = 0xd2
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'call':
			program[pc] = 0xcd
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'callz':
			program[pc] = 0xcc
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'callnz':
			program[pc] = 0xc4
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'callc':
			program[pc] = 0xdc
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'callnc':
			program[pc] = 0xd4
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]
		elif op[0] == 'jr':
			program[pc] = 0x18
			pc += 1
			if op[1][0:2] == '0x':
				op[1] = op[1][2:][::-1]
			while op[1] != '':
				program[pc] = text_to_hex(op[1][0:2][::-1])
				pc += 1
				op[1] = op[1][2:]

program = [0]*0x8000
def output(filename,out):
  parse_sections(filename)
  newfile = open(out,"wb")
  newfile.write(bytearray(program))
  newfile.close()

parse_sections(sys.argv[1])
newfile = open(sys.argv[2],"wb")
newfile.write(bytearray(program))
newfile.close()

##joyp.asm
##//should've been joypad test
##//turned out to be almost everything beside joypad
##
##.non-default destination
##
##.title
##0x435553544f4d20524f4d
##
##.start
##jp 0x0150
##
##.0x0150
##//load sprite tile data
##ld $hl 0x8002
##ld #hl 0x3c
##inc $hl
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x4a
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x42
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x3c
##//load sprite tile data
##ld $hl 0x8012
##ld #hl 0x3c
##inc $hl
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x4a
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x42
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x3c
##
##//load palette (obp0)
##ld $hl 0xff48
##ld #hl 0x1b
##
##//disable bkg, win/enable sprites
##ld $hl 0xff40
##ld #hl 0x86
##
##.0x1000
##//place sprite
##ld $hl 0xfe00
##ld #hl 0x10
##inc $hl
##ld #hl 0x08
##
##ld $hl 0xfe00
##ld #hl 0x14
##call 0x2000
##ld $hl 0xfe00
##ld #hl 0x18
##call 0x2000
##ld $hl 0xfe00
##ld #hl 0x1c
##call 0x2000
##ld $hl 0xfe00
##ld #hl 0x20
##call 0x2000
##ld $hl 0xfe00
##ld #hl 0x24
##call 0x2000
##ld $hl 0xfe00
##ld #hl 0x28
##
##jp 0x1000
##
##.0x2000
##rst 0x00
##rst 0x00
##rst 0x00
##rst 0x00
##rst 0x00
##ret
##
##.0x0000
##ld $hl 0xff44
##ld $a #hl
##ld $b 0x91
##cp $b
##jpnz 0x0000
##ld $hl 0xff40
##ld #hl 0x06
##ret

##oam.asm
##//testing oam stuff
##
##.non-default destination
##
##.title
##0x435553544f4d20524f4d
##
##.start
##jp 0x0150
##
##.0x0150
##//load sprite tile data
##ld $hl 0x8002
##ld #hl 0x3c
##inc $hl
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x4a
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x42
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x3c
##//load sprite tile data
##ld $hl 0x8012
##ld #hl 0x3c
##inc $hl
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x4a
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x42
##inc $hl
##ld #hl 0x3c
##inc $hl
##ld #hl 0x66
##inc $hl
##ld #hl 0x18
##inc $hl
##ld #hl 0x3c
##
##//load palette (obp0)
##ld $hl 0xff48
##ld #hl 0x1b
##
##//disable bkg, win/enable sprites
##ld $hl 0xff40
##ld #hl 0x86
##
##ld $hl 0xfe00
##ld #hl 0x10
##inc $hl
##ld #hl 0x08
##
##ld $hl 0xfe04
##ld #hl 0x28
##inc $hl
##ld #hl 0x20
##
##ld $hl 0xfe08
##ld #hl 0x38
##inc $hl
##ld #hl 0x30
##jp 0x0200
##
##.0x0200
##nop
##jp 0x0200

##tiles.asm
##//example for syntax
##//lines starting with '//' are ignored
##//hexdecimal strings are written two numbers at a time
##//numbers not starting in 0x are decimal(currently ignored?)
##//$register directly accesses the register
##//#register uses the register as a pointer to memory
##//new sections start with '.'
##//valid sections are any valid memory address, start, title, and non-japanese for destination code
##//assembler probably will add .stop internally to make things easier to keep track of
##//default destination code is japan
##//this test file copies a specific tile pattern over the screen and scrolls horizontally
##.non-default destination
##
##.title
##0x435553544f4d20524f4d
##
##.start
##//pc initialized to 100
##ld $hl 0x8000
##ld $de 0x423c
##rst 0x00
##ld $de 0x00ff
##rst 0x00
##rst 0x08
##rst 0x08
##rst 0x08
##rst 0x08
##ld $de 0x00ff
##rst 0x00
##ld $de 0x423c
##rst 0x00
##jp 0x0150
##
##.0x0000
##ld #hl $d
##inc $l
##ld #hl $e
##inc $l
##ret
##
##.0x0008
##ld $de 0x4281
##rst 0x00
##ret
##
##.0x0150
##ld $hl 0xff43
##ld $b #hl
##inc $b
##ld #hl $b
##jp 0x0150
