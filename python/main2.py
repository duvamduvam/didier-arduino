import os
import serial
import datetime


movieFolder = ("/home/pi/Videos/") 
queue = ""

videos = {
    "10000":"bored.mp4",
    "10001":"smile.mp4",
    "10002":"anger.mp4",
    "10004":"base.mp4"
    }

#find the right usb
def getUSB():
    usb = ['ttyACM0','ttyACM1','ttyACM2','ttyACM3']
    list = os.listdir("/dev")
    
    for u in usb:
        if u in list: 
            return '/dev/'+u

#clean serial output b'9991\r\n'-> 9991
def cleanOutput(out):
    out = out[0:(len(out)-2)]
    return out.decode("utf-8")
    

usb = getUSB()
ser = serial.Serial(usb,9600)
ser.flushInput()



try:
    while True:


        read_serial=ser.readline()
        cleanSerial = cleanOutput(read_serial)
        
        fileName = datetime.datetime.now().isoformat()
        bufferFile = open(fileName,  "a")
        bufferFile.write(cleanSerial)
        bufferFile.close()
        
        print(cleanSerial)
        
   
#exit with crtl+C
except KeyboardInterrupt:
    pass
    #ser.write('3333')
