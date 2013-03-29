#!/usr/bin/python
# vim: set fileencoding=utf-8:
import sys
import os
import serial
import subprocess

nr_of_steps = (48*3*3)

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
	except:
		print "can not open serial port", port
		sys.exit(2)

	for i in range(0, nr_of_steps):
		distance = "0.00"

		#wait until distance is updated
		subprocess.call(["sleep", "1s"])

		#get distance from camera
		if subprocess.call(["wget", ip_addr + ":8080/photo.jpg", "-O scans/"  + os.getcwd() +  + str(i) + ".jpg"]) != 0: 
			print 'can not connect to ip ', ip_addr
			sys.exit(2)

		subprocess.call(["jpegtran -rotate 90 scans/" + str(i) + ".jpg > scans/ " + str(i) + ".jpg"], shell=True)

		#step the motor
		ser_dev.write("step_motor 1\n")
	
	output.close()
	
if __name__ == "__main__":
	main(sys.argv[1:])
