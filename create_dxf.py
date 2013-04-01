#!/usr/bin/python
from dxfwrite import DXFEngine as dxf

import sys
import getopt
import os
import math

crrnt_angle = -90
axis_offset = 25
step_angle = 0.8333333333333333333333333333333333333333333333

def convert_to_coords(distance):
	
	offset_x = -math.sin((crrnt_angle-90)*(math.pi/180))*axis_offset
	offset_y = math.cos((crrnt_angle-90)*(math.pi/180))*axis_offset

	x = -math.sin(crrnt_angle*(math.pi/180))*distance
	y = math.cos(crrnt_angle*(math.pi/180))*distance

	x -= offset_x
	y -= offset_y
	return (x,y)

def main(argv):
	global crrnt_angle
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

	 #read all the lines into a list.
	lines = f.readlines()	

	if len(lines) <= 3:
		print 'error: not enough points in ', inputfile
		sys.exit(2)
	
	coords = []

	for line in lines:
		distance = int(line)
		(x,y) = convert_to_coords(distance)	
		crrnt_angle += step_angle
		coords.append(str(int(x)) + " " + str(int(y)))
	
	drawing = dxf.drawing(outputfile)

	prev_x = 0
	prev_y = 0
	start = 1
	start_x = 0
	start_y = 0
	for coord in coords:
		numbers = coord.split()
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
	drawing.save()
	
if __name__ == "__main__":
	main(sys.argv[1:])
