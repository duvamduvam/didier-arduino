import os
import re
import serial

from subprocess import Popen

movieFolder = ("/home/pi/Videos/") 

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

def getVideo(number):
    for key,val in videos.items():
        if number == key:
            print(val)
            playVideo(val)
            break
        
def playVideo(name):
    file = movieFolder + name
    Popen(['omxplayer', '-o', 'hdmi',  '--win', '0,0,1024,768', '--no-osd', file])
    #file = movieFolder +"base.mp4"
    #Popen(['omxplayer', '-o', 'hdmi',  '--win', '0,0,1024,768', '--no-osd', file])
    
usb = getUSB()
ser = serial.Serial(usb,9600)
ser.flushInput()

while True:

    read_serial=ser.readline()
    cleanSerial = cleanOutput(read_serial)
    print(cleanSerial)
    
    getVideo(cleanSerial)
    
    #ser.write('3333')