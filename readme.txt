./stm/
	Contains the firmware for the stm32F0-Discovery board and the Sharp GP2Y0A02YK0F IR sensor.
	the stm32 controlls the stepper motor, the sensor and sends measured coordinates over the serial port

./convert_to_coords.py
	calculates coordinates to the measured distances.

./create_dxf.py
	converts points to dxf with create_dxf.py:
	needs dxfwrite library
