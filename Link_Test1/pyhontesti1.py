#!/usr/bin/python
import time
import serial

# avataan serial yhteys
ser = serial.Serial('/dev/ttyACM0',9600, 8, 'N', 1, timeout=1)
print(ser.name)

#ser.write(unicode('liiku\n'))
#time.sleep(1)
#ser.write(unicode("kaanny"))

ser.write("kaanny")
#ser.flushInput()
#ser.write("mitamita\0")
time.sleep(2)
ser.write("liiku")
#ser.flushInput()
time.sleep(2)
ser.write("tila")
time.sleep(2)          
sana = ser.readline()
print(sana)

#ser.write('tila\r')
#time.sleep(1)
#print(ser.read())
