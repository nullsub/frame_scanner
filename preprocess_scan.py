#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import getopt
import os
import subprocess

#nr_of_steps = (48*3*3)
nr_of_steps = (71)

def main(argv):
	global nr_of_steps
	outputfile = 'scan_coords.txt'

	out = []

	for i in range(0, nr_of_steps):
		distance = None
		try:
			distance = subprocess.check_output(["ocr/ssocr/ssocr -o debug.jpg crop 240 1440 1000 445 remove_isolated -D -i5 -d-1 -n4 -r6   -t61 " + os.getcwd() + "/scans/" + str(i) + ".jpg"], shell=True)
		except:
			subprocess.call(["shotwell " + os.getcwd() + "/scans/" + str(i) + ".jpg"], shell=True)
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
		distance -= 0

		out.append(str(int(distance)) + "\n")

	output = open(outputfile, 'w')
	for line in out:
		output.write(line)
	output.close()
	
if __name__ == "__main__":
	main(sys.argv[1:])
