import board
import digitalio
from PIL import Image, ImageDraw, ImageFont
from adafruit_rgb_display import st7789  # pylint: disable=unused-import

from dadou_utils.lcd import LCD_2inch


class Display:

    def __init__(self):
        cs_pin = digitalio.DigitalInOut(board.CE0)
        dc_pin = digitalio.DigitalInOut(board.D25)
        reset_pin = digitalio.DigitalInOut(board.D24)
        spi = board.SPI()
        BAUDRATE = 24000000
        #self.display = LCD_2inch.LCD_2inch(spi=SPI.SpiDev(bus, device), spi_freq=10000000, rst=RST, dc=DC, bl=BL)

        self.display = st7789.ST7789(
            spi, rotation=270, width=240, height=320,
            cs=cs_pin,
            dc=dc_pin,
            rst=reset_pin,
            baudrate=BAUDRATE,
        )

        if self.display .rotation % 180 == 90:
            self.height = self.display .width  # we swap height/width to rotate it to landscape!
            self.width = self.display .height
        else:
            self.width = self.display .width  # we swap height/width to rotate it to landscape!
            self.height = self.display .height

        self.background = None
        self.draw = None

        self.font1 = ImageFont.truetype("/home/didier/deploy/dadouglove/tests/lcd/Font/Font00.ttf", 30)
        self.font2 = ImageFont.truetype("/home/didier/deploy/dadouglove/tests/lcd/Font/Font01.ttf", 15)
        self.font3 = ImageFont.truetype("/home/didier/deploy/dadouglove/tests/lcd/Font/Font02.ttf", 25)

        self.clear()

    def clear(self):
        self.background = Image.new("RGB", (self.width, self.height))

        # Get drawing object to draw on image.
        self.draw = ImageDraw.Draw(self.background)
        self.draw.rectangle((0, 0, self.width, self.height), fill=(255, 255, 255))
    def show_menu(self):

        #menu speratation line
        self.draw.line([(0, 40), (320, 40)], fill="BLACK", width=4)

        #battery indication
        self.draw.rectangle([(5, 5), (60, 35)], fill="WHITE", outline="BLACK")
        self.draw.rectangle([(6, 6), (59, 34)], fill="GREEN", outline="GREEN")

        #wifi indication
        self.draw.line([(70, 28), (70, 35)], fill="BLUE", width=4)
        self.draw.line([(75, 20), (75, 35)], fill="BLUE", width=4)
        self.draw.line([(80, 13), (80, 35)], fill="BLUE", width=4)
        self.draw.line([(85, 5), (85, 35)], fill="BLUE", width=4)

        #robot connection indicator
        self.draw.text((90, 1), 'R', fill="GREEN", font=self.font1)

        #glove indicator
        self.draw.text((115, 1), 'GL', fill="BLACK", font=self.font1)

        #mode instruction
        self.draw.text((150, 1), 'CONFIG', fill="BLACK", font=self.font1)

        #last instruction
        self.draw.text((270, 1), 'DH', fill="BLACK", font=self.font1)

    def show_playlist(self, selected_index, playlist_data):
        self.clear()
        self.show_menu()
        selected_index -= 1
        if selected_index > 2:
            start = selected_index - 2
        else:
            start = 0

        if len(playlist_data) < (selected_index + 2):
            end = len(playlist_data)
        else:
            end = start + 5

        pos = 45
        for i in range(start, end):
            if i == selected_index:
                self.draw.rectangle([(0, pos+5), (315, pos + 40)], fill="WHITE", outline="RED", width=4)

            self.draw.text((5, pos), playlist_data[i], fill="BLACK", font=self.font1)
            pos += 30

        self.display.image(self.background)

