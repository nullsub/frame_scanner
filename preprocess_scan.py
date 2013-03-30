#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import getopt
import os
import subprocess

nr_of_steps = (48*3*3)/8

def main(argv):
	global nr_of_steps
	outputfile = 'scan_coords.txt'

	out = []

	for i in range(0, nr_of_steps):
		print "pic nr", i
 		subprocess.call(["jpegtran -rotate 90 " + os.getcwd() + "/scans/" + str(i) + ".jpg > " + os.getcwd() + "/scans/" + str(i) + "_r.jpg"], shell=True)
		try:
			distance = subprocess.check_output(["ocr/ssocr/ssocr -o debug.jpg crop 240 1465 1000 445 remove_isolated -D -i5 -d-1 -n4 -r4 -t61.9 " + os.getcwd() + "/scans/" + str(i) + "_r.jpg"], shell=True)
		except:
			subprocess.call(["shotwell " + os.getcwd() + "/scans/" + str(i) + "_r.jpg"], shell=True)
			while True:
				distance = raw_input("enter distance: ")
				try:
					distance = float(distance)
				except ValueError:
					continue
				break

		distance = float(distance)
		print "distance is ", distance

		distance *= 1000
		distance -= 45

		out.append(str(int(distance)) + "\n")

	output = open(outputfile, 'w')
	for line in out:
		output.write(line)
	output.close()
	
if __name__ == "__main__":
	main(sys.argv[1:])
