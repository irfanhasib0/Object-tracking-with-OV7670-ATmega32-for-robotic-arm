import serial
import cv2
import numpy as np
import time
ser = serial.Serial('COM6',1000000,timeout=.1)
# open serial port
print(ser.name)         # check which port was really used
ser.write(chr(97))     # write a string

h=60
w=80
size=h*w
#img=np.zeros(320,240)
while(ser.readable()):
        T1=time.time()
    #print("Ok")
    #cv2.waitKey(0)
        data=np.zeros(size,np.uint8)
        
    
    #if(ser.read(1)=='R'):
        #print((ser.read(1)))
        #print((ser.read(1)))
     #if(ser.read(1)=='D'):
      #if(ser.read=='Y'):
        #try:print(ord(ser.read(1)))
        #except:0
        while(ser.read()!='R'):
           ser.write('A')
        for i in range(1,size):
           try:data[i]=ord(ser.read(1))
           except:print("E")
        
        print(time.time()-T1)   
        #print(data)
        print(len(data))
        data.resize((h,w))
        #img=cv2.cv.fromarray(data)
        img= cv2.resize(data,(2*w, 2*h), interpolation = cv2.INTER_CUBIC)
        cv2.imshow('data',img)
        key=cv2.waitKey(10)
        if(key==26):
            break
ser.close()             # close port
print("done")


