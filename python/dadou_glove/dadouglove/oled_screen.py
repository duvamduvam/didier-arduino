import displayio
import adafruit_displayio_ssd1306
import terminalio
from adafruit_bitmap_font import bitmap_font
from adafruit_display_text import label
from busio import I2C

WIDTH = 256
HEIGHT = 64

#Fonts : https://github.com/olikraus/u8g2/wiki/fntgrp
# https://github.com/olikraus/u8g2/tree/master/tools/font/bdf

FONTS_FOLDER = "display/fonts/"
FONT1 = "Helvetica-Bold-36.bdf"


class OledScreen:
    def __init__(self, i2c):

        self.helvetica36 = bitmap_font.load_font(FONTS_FOLDER+FONT1)

        display_bus = displayio.I2CDisplay(i2c, device_address=0x3C)

        display = adafruit_displayio_ssd1306.SSD1306(display_bus, width=WIDTH, height=HEIGHT)
        self.splash = displayio.Group()
        display.show(self.splash)

        color_bitmap = displayio.Bitmap(WIDTH, HEIGHT, 1)  # Full screen white
        color_palette = displayio.Palette(1)
        color_palette[0] = 0xFFFFFF  # White

        bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)
        self.splash.append(bg_sprite)

        # Draw a smaller inner rectangle
        inner_bitmap = displayio.Bitmap(118, 54, 1)
        inner_palette = displayio.Palette(1)
        inner_palette[0] = 0x000000  # Black
        inner_sprite = displayio.TileGrid(inner_bitmap, pixel_shader=inner_palette, x=5, y=4)
        self.splash.append(inner_sprite)

        # Draw a label
        text = "Didier"
        text_area = label.Label(self.helvetica36, text=text, color=0xFFFF00, x=28, y=25)
        self.splash.append(text_area)

        #text = "Brinquedos"
        #text_area = label.Label(terminalio.FONT, text=text, color=0xFFFF00, x=32, y=5)
        #self.splash.append(text_area)

    def info_bar(self):
        pass

    def playlist_frame(self):
        pass

    def anglo_frame(self):
        pass

    def repliques_frame(self):
        pass

    def write(self, msg):
        text_area = label.Label(self.helvetica36, text=msg, color=0xFFFF00, x=28, y=25)
        self.splash.pop()
        self.splash.append(text_area)

    def update(self, instructions):
        pass

    def process(self):
        pass
