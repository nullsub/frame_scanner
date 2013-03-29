#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import getopt
import os
import serial
import subprocess

nr_of_steps = (48*3*3)/3
#nr_of_steps = (20)

def main(argv):
	global nr_of_steps
	ip_addr = ''
	port = '/dev/ttyS0'
	outputfile = 'scan_coords.txt'
	if len(sys.argv) <= 1:
		print sys.argv[0], '.py <ip_addr> <outputfile>'
		sys.exit(2)
	ip_addr = sys.argv[1]	
	if len(sys.argv) >= 3:
		outputfile = sys.argv[2]
	if ip_addr == '':
		print sys.argv[0], '.py <ip_addr> <outputfile>'
		sys.exit(2)
	if subprocess.call(["ping", ip_addr, "-c2"]) != 0:
		print 'can not connect to ip ', ip_addr
		#sys.exit(2)

	ser_dev = serial.Serial(port, 9600, timeout=1)

	 #read all the lines into a list.
	output = open(outputfile, 'w')

	for i in range(0, nr_of_steps):
		distance = "0.00"

		#wait until distance is updated
		subprocess.call(["sleep", "1s"])

		#get distance from camera
		if subprocess.call(["wget", ip_addr + ":8080/photo.jpg", "-O" + os.getcwd() + "/photo.jpg"]) != 0: 
			print 'can not connect to ip ', ip_addr
			sys.exit(2)
		subprocess.call(["jpegtran -rotate 90 photo.jpg >" + str(i) + ".jpg"], shell=True)
		#distance = subprocess.check_output(["ocr/ssocr/ssocr -o debug.jpg -d-1 -i1 -n4 -t70 crop 240 1450 1000 445 " + os.getcwd() + "/photo1.jpg"], shell=True)

		#somehow a point is recognized at the end
		#i#f distance[len(distance)-2] is '.':
		#	distance = distance[:len(distance)-2]
		#distance = float(distance)
		#distance *= 10
		#distance -= 0

		#print "distance is ", distance
		#output.write(str(int(distance)) + "\n")

		#step the motor
		ser_dev.write("step_motor 1\n")
		subprocess.call(["sleep", "0.2s"])
		ser_dev.write("step_motor 1\n")
		subprocess.call(["sleep", "0.2s"])
		ser_dev.write("step_motor 1\n")
		subprocess.call(["sleep", "0.2s"])
	
	output.close()
	
if __name__ == "__main__":
	main(sys.argv[1:])
