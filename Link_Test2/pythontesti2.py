#!/usr/bin/python
import time
import serial

# avataan serial yhteys
ser = serial.Serial('/dev/ttyACM0',9600, 8, 'N', 1, timeout=1)
print(ser.name)



ser.write("[MO]")
ser.write("[RE]")
ser.write("[TS]")
while ser.inWaiting > 0:
    sana = ser.read()
    print(sana)
    #print(ser.inWaiting());

#ser.write('tila\r')
#time.sleep(1)
#print(ser.read())
