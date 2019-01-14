from omxplayer.player import OMXPlayer
from pathlib import Path
from time import sleep

VIDEO_PATH = Path("/home/pi/Videos/smile.mp4")

player = OMXPlayer(VIDEO_PATH)



while True:
    if(player.is_playing != True):
        VIDEO_PATH = Path("/home/pi/Videos/anger.mp4")
        player = OMXPlayer(VIDEO_PATH)
    
    
    

sleep(5)

player.quit()