import time

import board
import digitalio


FLASHRATE = 0.1

class BuildinLed:
    led = digitalio.DigitalInOut(board.LED)
    led.direction = digitalio.Direction.OUTPUT
    lastBlink = float(0)
    blinkRate = float(0.5)

    # print("Start Blink!")

    def blink(self):
        if (time.monotonic() > self.lastBlink + self.blinkRate):
            self.led.value = not self.led.value
            self.lastBlink = time.monotonic()

    def flash(self):
    	for i in range(3):
        #if time.monotonic() > (self.lastBlink + self.flashRate):
            self.led.value = not self.led.value
            time.sleep(FLASHRATE)
            self.led.value = not self.led.value
            time.sleep(FLASHRATE)
