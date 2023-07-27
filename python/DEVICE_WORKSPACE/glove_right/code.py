from buildinLed import BuildinLed
import busio
import board
#from anglometer import Anglometer
from glove_keys import GloveKeys
#from vibrator import Vibrator

#rpi violet usb C
keyboard = GloveKeys((("l", "k", "j"), ("f", "e", "d"), ("c", "b", "a"), ("i", "h", "g")),
                     (board.GP10, board.GP11, board.GP12),
                     (board.GP6, board.GP7, board.GP8, board.GP9))

#rpi
#keyboard = GloveKeys((("c", "b", "a"), ("f", "e", "d"), ("i", "h", "g"), ("l", "k", "j")),
#                     (board.GP7, board.GP8, board.GP9),
#                     (board.GP10, board.GP11, board.GP12, board.GP13))

#keyboard = GloveKeys((("y ", "w", "v"), ("r", "q", "p"), ("o", "n", "m"), ("u", "t", "s")),
#                     (board.GP17, board.GP10, board.GP11),
#                     (board.GP13, board.GP16, board.GP14, board.GP15))

buildinLed = BuildinLed()
uart = busio.UART(board.GP0, board.GP1, baudrate=115200, timeout=10)
#anglometer = Anglometer()
#vibrator = Vibrator()

print("start glove right")

while True:
    keys = keyboard.check()
    if len(keys) > 0:
        msg = "<"+keys[0]+">"
        print(msg)
        uart.write(bytes(msg, "ascii"))
        #uart.write(bytes(f"<L,{keys[0]}>", "ascii"))
        buildinLed.flash()
    buildinLed.blink()
    #anglometer.process()
    #vibrator.execute()
