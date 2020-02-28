import serial
import cv2
import numpy as np
ser = serial.Serial('COM6',1000000,timeout=1)
# open serial port
print(ser.name)         # check which port was really used
ser.write(chr(97))     # write a string
data=np.zeros(76800,np.uint8)
img=np.zeros((320,240),np.uint8)
while(ser.readable()):
    print("Ok")
    #cv2.waitKey(0)
    
    if(ser.read()=='R'):
     #if(ser.read()=='D'):
      #if(ser.read=='Y'):
        print("Start")
        for i in range(1,76800):
           try:data[i]=ord(ser.read(1))
           except:0
           
        print(data)
        print(len(data))
        data.resize((320,240))
        cv2.imshow('data',data)
        key=cv2.waitKey(10)
        if(key==26):
            break
ser.close()             # close port
print("done")


