#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import getopt
import os
import math

crrnt_angle = 0
x_transformation = 0
step_angle = 0.83333333333333333333333333333333333333334

def convert_to_coords(distance):
	
	#convert to voltage
	voltage = float(3.3/4095)*float(distance)

	#convert the Voltage to a distance:
	#16.2537 * x4 – 129.893 * x3 + 382.268 * x2 – 512.611 * x + 306.439
	a = voltage*voltage*voltage*voltage*16.2537
	b = -voltage*voltage*voltage*129.893
	c = voltage*voltage*382.268
	d = -voltage*512.611
	e = 306.439

	distance = a+b+c+d+e

	distance *= 10 #to mm
	
	#add offset from the axis
	#distance = distance + 70
	
	print "dist is ", distance	

	x = -math.sin(crrnt_angle*(math.pi/180))*distance
	print "x is ", x	
	y = math.cos(crrnt_angle*(math.pi/180))*distance
	print "y is ", y	
	x += x_transformation
	return (x,y)

def main(argv):
	global crrnt_angle
	global step_angle
	global x_transformation
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
		outputfile = os.path.splitext(inputfile)[0] + '_coords.txt'
	try:
		f = open(inputfile, "r")
	except IOError:
		print 'can not open input file', inputfile
		sys.exit(2)

	 #read all the lines into a list.
	lines = f.readlines()	
	output = open(outputfile, 'w')

	if len(lines) <= 3:
		print 'error: not enough points in ', inputfile
		sys.exit(2)

	for line in lines:
		if '#' in line:
			if 'transform_left' in line:
				print 'going left'
				x_transformation += 1000
			continue
		distance = int(line)
		(x,y) = convert_to_coords(distance)	
		output.write(str(int(x)))
		output.write(' ')
		output.write(str(int(y)))
		output.write("\n")
		crrnt_angle += step_angle
	
	output.close()
	
if __name__ == "__main__":
	main(sys.argv[1:])
