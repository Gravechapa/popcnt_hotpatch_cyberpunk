import ida_idc
import ida_bytes
import ida_search
from idautils import DecodeInstruction
from idaapi import get_reg_name

import sys
import os
import ctypes

def getRegStr(num):
	result = "ctx->"
	if num == 0:
		result += "Rax"
	elif num == 1:
		result += "Rcx"
	elif num == 2:
		result += "Rdx"
	elif num == 3:
		result += "Rbx"
	elif num == 4:
		result += "Rsp"
	elif num == 5:
		result += "Rbp"
	elif num == 6:
		result += "Rsi"
	elif num == 7:
		result += "Rdi"
	elif num == 8:
		result += "R8"
	elif num == 9:
		result += "R9"
	elif num == 10:
		result += "R10"
	elif num == 11:
		result += "R11"
	elif num == 12:
		result += "R12"
	elif num == 13:
		result += "R13"
	elif num == 14:
		result += "R14"
	elif num == 15:
		result += "R15"
	else:
		print("Parsing error: unknown register")
		result = "error"
	return result

class SecondOperand:
	def __init__(self, size):
		self.is64bit = True
		self.isPtr = True
		self.base = -1
		self.index = -1
		self.scale = 1
		self.displ = 0
		self.instSize = size

	def __eq__(self, other):
		return other and self.is64bit == other.is64bit \
					 and self.isPtr == other.isPtr \
					 and self.base == other.base \
					 and self.index == other.index \
					 and self.scale == other.scale \
					 and self.displ == other.displ \
					 and self.instSize == other.instSize

	def __ne__(self, other):
		return not self.__eq__(other)

	def __key(self):
		return (self.is64bit, self.isPtr, self.base, self.index, self.scale, self.displ, self.instSize)
		
	def __hash__(self):
		return hash(self.__key())

outputPath = 'C:\ida output\output_popcnt_parser.txt'
outputInfoPath = 'C:\ida output\output_popcnt_parser_info.txt'
if os.path.exists(outputPath):
	os.remove(outputPath)
output = open(outputPath, 'w')
if os.path.exists(outputInfoPath):
	os.remove(outputInfoPath)
output_info = open(outputInfoPath, 'w')

imagebase = idaapi.get_imagebase()
popcntAddr = ida_search.find_text(0, 0, 0, "popcnt", SEARCH_DOWN)
db = {}
counter = 0
while popcntAddr != BADADDR:
	try:
		popcntSize = get_item_size(popcntAddr)
		print (counter, hex(popcntAddr).rstrip("L"), GetDisasm(popcntAddr), hex(ctypes.c_longlong(get_operand_value(popcntAddr, 1)).value).rstrip("L"), hex(popcntSize).rstrip("L"), file = output_info) 
		
		###Second operand parsing
		if get_operand_type(popcntAddr, 0) == o_reg:
			secondOp = SecondOperand(popcntSize)
			inst = DecodeInstruction(popcntAddr)
			if inst.Op2.dtype != idaapi.dt_qword:
				if inst.Op2.dtype == idaapi.dt_dword:
					secondOp.is64bit = False
				else:
					raise RuntimeError("Parsing error: unsupported data type")
					
			if inst.Op2.type == o_reg:
				secondOp.isPtr = False
				secondOp.base = inst.Op2.reg
			elif inst.Op2.type == o_phrase or inst.Op2.type == o_displ:
				if inst.Op2.specflag1 and inst.Op2.phrase != 4:
					extendedBase = 0
					extendedIndex = 0
					rex = ida_bytes.get_byte(popcntAddr + 1)
					if rex != 0x0f:
						if rex >> 4 == 4:
							extendedBase = rex & 1
							extendedIndex = (rex >> 1) & 1
						else:
							raise RuntimeError("Parsing error: bad rex prefix")
							
					sib = inst.Op2.specflag2 
					#print(rex)
					#print extendedBase, extendedIndex
					#print(sib)
					base = (sib & 7) | (extendedBase << 3)
					if base == 12 and not extendedIndex:
						secondOp.base = base
					else:
						index = ((sib >> 3) & 7) | (extendedIndex << 3)
						scale = (sib >> 6) & 3
						secondOp.base = base
						secondOp.index = index
						secondOp.scale = 2 ** scale
				else:
					secondOp.base = inst.Op2.phrase
				if inst.Op2.type == o_displ:
					secondOp.displ = ctypes.c_longlong(inst.Op2.addr).value
			else:
				raise RuntimeError("Parsing error: unsupported operand type")
			#print counter, secondOp
			###
			
			firstOp = get_operand_value(popcntAddr, 0)
			db.setdefault(firstOp,{}).setdefault(secondOp, []).append(popcntAddr - imagebase)
		else:
			raise RuntimeError("Parsing error: bad first operand type ", counter)
	except RuntimeError as e:
		print(e)

	popcntAddr = ida_search.find_text(popcntAddr + popcntSize, 0, 0, "popcnt", SEARCH_DOWN)
	#popcntAddr = BADADDR
	counter += 1

counter = 0
code = ""
for firstOp in db:
	secondOps = db[firstOp]
	for secondOp in secondOps:
		for addr in secondOps[secondOp]:
			code += "case " + hex(addr).rstrip("L") + ":\n"
			counter += 1
		code += "    HOTFIX(" + getRegStr(firstOp) + ", "
		if secondOp.isPtr == False:
			code += getRegStr(secondOp.base)
			if secondOp.is64bit == False:
				code += " & 0xFFFFFFFFULL"
			code += ", "
		else:
			if secondOp.is64bit == False:
				code += "*reinterpret_cast<uint32_t*>("
			else:
				code += "*reinterpret_cast<uint64_t*>("
			code += getRegStr(secondOp.base)
			if secondOp.index != -1:
				code += " + " + getRegStr(secondOp.index) + " * " + str(secondOp.scale)
			if secondOp.displ != 0:
				if secondOp.displ > 0:
					code += " + "
				else:
					code += " - "
				code += hex(abs(secondOp.displ)).rstrip("L")
			code += "), "
		code += str(secondOp.instSize) + ")\n"

code += "Finished " + str(counter)
print (code, file = output)

output.close()
output_info.close()
