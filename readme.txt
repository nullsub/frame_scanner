./stm/
	Contains the firmware for the stm32F0-Discovery board and the Sharp GP2Y0A02YK0F IR sensor.
	the stm32 controlls the stepper motor, the sensor and sends measured coordinates over the serial port

./do_scan.py
	do a scan. move motor make a photo at every angle

./preprocess_scan.py
	read images into a list of distances

./create_dxf.py
	convert points to coords and create dxf.
