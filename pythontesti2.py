#!/usr/bin/python
import time
import serial

# avataan serial yhteys
ser = serial.Serial('/dev/ttyACM0',9600, 8, 'N', 1, timeout=1)
print(ser.name)



ser.write("[MO]")
time.sleep(2)
ser.write("[RE]")
