#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import os
import serial
import subprocess
import time

scale_step = 4
nr_of_steps = (48*3*3)/scale_step

def main(argv):
	global nr_of_steps
	ip_addr = ''
	port = '/dev/ttyS0'
	if len(sys.argv) <= 1:
		print sys.argv[0], '.py <ip_addr>'
		sys.exit(2)
	ip_addr = sys.argv[1]	
	if ip_addr == '':
		print sys.argv[0], '.py <ip_addr>'
		sys.exit(2)
	try:
		ser_dev = serial.Serial(port, 9600, timeout=1)
		print "start"
	except:
		print "can not open serial port", port
		sys.exit(2)

	start_time = time.time()

	for i in range(0, nr_of_steps):

		crrnt_time = time.time()
		
		if crrnt_time - start_time > 60*14:
			raw_input("Reenable device and press enter")
			start_time = time.time()
			
		distance = "0.00"

		#wait until distance is updated
		subprocess.call(["sleep", "1s"])

		#get distance from camera
		if subprocess.call(["wget " + ip_addr + ":8080/photo.jpg -O scans/" + str(i) + ".jpg"], shell=True) != 0: 
			print 'can not connect to ip ', ip_addr
			sys.exit(2)

		#step the motor
		for x in range(0, scale_step):
			ser_dev.write("step_motor 1\n")
			subprocess.call(["sleep", "0.2s"])
	
if __name__ == "__main__":
	main(sys.argv[1:])
