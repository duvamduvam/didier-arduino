import RPi.GPIO as GPIO
import os
import sys
import time

from subprocess import Popen

movie1 = ("/home/pi/Videos/eyes_1.mp4")

#try:
#    while 1:
mxc = Popen(['omxplayer', '-o', 'hdmi',  '--win', '0,0,1024,768', '--no-osd', movie1])
        #time.sleep(100)
#except KeyboardInterrupt:
#sys.exit("program aborded")     



