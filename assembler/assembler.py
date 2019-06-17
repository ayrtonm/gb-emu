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
  inhex = False
  if info[0:2] == '0x':
    info = info[2:]
    inhex = True;
  while info != '\n':
    if inhex:
      program[pc] = text_to_hex(info[0:2])
      info = info[2:]
    else:
      program[pc] = ord(info[0]);
      info = info[1:]
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
