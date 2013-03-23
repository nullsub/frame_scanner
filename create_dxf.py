#!/usr/bin/python
from dxfwrite import DXFEngine as dxf

import sys
import getopt
import os

def main(argv):
	inputfile = ''
	outputfile = ''
	try:
		opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
	except getopt.GetoptError:
		print sys.argv[0], '.py -i <inputfile> -o <outputfile>'
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print 'test.py -i <inputfile> -o <outputfile>'
			sys.exit()
		elif opt in ("-i", "--ifile"):
			inputfile = arg
		elif opt in ("-o", "--ofile"):
			outputfile = arg
	if inputfile == '':
		print sys.argv[0], '.py -i <inputfile> -o <outputfile>'
		sys.exit(2)
	if outputfile == '':
		outputfile = os.path.splitext(inputfile)[0] + '.dxf'

	try:
		f = open(inputfile, "r")
	except IOError:
		print 'can not open input file', inputfile
		sys.exit(2)

	drawing = dxf.drawing(outputfile)

	 #read all the lines into a list.
	lines = f.readlines()	

	if len(lines) <= 3:
		print 'error: not enough points in ', inputfile
		sys.exit(2)

	prev_x = 0
	prev_y = 0
	start = 1
	start_x = 0
	start_y = 0
	for line in lines:
		numbers = line.split()
		x = numbers[0]
		y = numbers[1]
		
		if start:
			start_x = x
			start_y = y
		else:
			drawing.add(dxf.line((x, y), (prev_x, prev_y), color=7))
		prev_x = x
		prev_y = y
		start = 0

	drawing.add(dxf.line((prev_x, prev_y), (start_x, start_y), color=7))

	#drawing.add_layer('TEXTLAYER', color=2)
	#drawing.add(dxf.text('Test', insert=(0, 0.2), layer='TEXTLAYER'))
	drawing.save()
	
if __name__ == "__main__":
	main(sys.argv[1:])
