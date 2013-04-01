#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import getopt
import os
import subprocess

nr_of_steps = (43*3*3)
#nr_of_steps = 100
path = os.getcwd() + "/scans/" 

def main(argv):
	global nr_of_steps
	outputfile = 'scan_coords.txt'

	out = []
	ask = []

	prev = '0.0'
	for i in range(0, nr_of_steps):
	
		print "pic nr", i
 		subprocess.call(["jpegtran -crop 940x410+290+1480 -rotate 90 -grayscale " + path + str(i) + ".jpg > " + path + str(i) + "_r.jpg"], shell=True)
		try:
			#distance = subprocess.check_output(["ocr/ssocr/ssocr -o debug.jpg -D -i5 -d-1 -n4 -r4 -t41.3  remove_isolated " + path + str(i) + "_r.jpg"], shell=True)
			distance = subprocess.check_output(["display_reader/display_reader " + path + str(i) + "_r.jpg"], shell=True)
		except:
			print "asking later. got", distance
			ask.append(i)
			distance = "0.0"

		try:
			distance = float(distance)
		except ValueError:
			print "asking later. got", distance
			ask.append(i)
			distance = 0.0

		print "distance is ", distance

		distance *= 1000
		distance -= 45

		out.append(str(int(distance)) + "\n")

	for i in ask:
		subprocess.call(["shotwell " + path + str(i) + "_r.jpg"], shell=True)
		while True:
			distance = raw_input("enter distance: ")
			try:
				distance = float(distance)
			except ValueError:
				continue
			break
		distance *= 1000
		distance -= 45
		out[i] = str(int(distance)) + "\n"

	output = open(outputfile, 'w')
	for line in out:
		output.write(line)
	output.close()
	
if __name__ == "__main__":
	main(sys.argv[1:])
