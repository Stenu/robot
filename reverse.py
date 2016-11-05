#!/usr/bin/python
import time
import serial

# avataan serial yhteys
ser = serial.Serial('/dev/ttyACM0',9600, 8, 'N', 1, timeout=1)

ser.write("[RVRS]")

