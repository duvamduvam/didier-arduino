import logging
import time
import unittest

from PIL import Image, ImageDraw, ImageFont

from dadou_utils.lcd import LCD_2inch

disp = LCD_2inch.LCD_2inch()


class TestLCD(unittest.TestCase):

    def init_background(self):
        disp.Init()
        # Clear display.
        disp.clear()

        # Create blank image for drawing.
        image1 = Image.new("RGB", (disp.height, disp.width), "WHITE")


        logging.info("draw point")
        return image1

    def test_point(self):
        image = self.init_background()
        draw = ImageDraw.Draw(image)

        draw.rectangle((5,10,6,11), fill = "BLACK")
        draw.rectangle((5,25,7,27), fill = "BLACK")
        draw.rectangle((5,40,8,43), fill = "BLACK")
        draw.rectangle((5,55,9,59), fill = "BLACK")

        disp.ShowImage(image)
        time.sleep(30)

    def test_line(self):
        image = self.init_background()
        draw = ImageDraw.Draw(image)

        draw.line([(20, 10), (70, 60)], fill="RED", width=1)
        draw.line([(70, 10), (20, 60)], fill="RED", width=1)
        draw.line([(170, 15), (170, 55)], fill="RED", width=1)
        draw.line([(150, 35), (190, 35)], fill="RED", width=1)

        disp.ShowImage(image)
        time.sleep(30)

    def test_rectangle(self):
        image = self.init_background()
        draw = ImageDraw.Draw(image)

        draw.rectangle([(20, 10), (70, 60)], fill="WHITE", outline="BLUE")
        draw.rectangle([(85, 10), (130, 60)], fill="BLUE")

        disp.ShowImage(image)
        time.sleep(30)

    def test_circle(self):
        image = self.init_background()
        draw = ImageDraw.Draw(image)

        draw.arc((150, 15, 190, 55), 0, 360, fill=(0, 255, 0))
        draw.ellipse((150, 65, 190, 105), fill=(0, 255, 0))

        disp.ShowImage(image)
        time.sleep(30)

    def test_text(self):
        image = self.init_background()
        draw = ImageDraw.Draw(image)

        Font1 = ImageFont.truetype("/home/didier/deploy/dadouglove/tests/lcd/Font/Font01.ttf", 25)
        Font2 = ImageFont.truetype("/home/didier/deploy/dadouglove/tests/lcd/Font/Font01.ttf", 35)
        Font3 = ImageFont.truetype("/home/didier/deploy/dadouglove/tests/lcd/Font/Font02.ttf", 32)

        draw.rectangle([(0, 65), (140, 100)], fill="WHITE")
        draw.text((5, 68), 'Hello world', fill="BLACK", font=Font1)
        draw.rectangle([(0, 115), (190, 160)], fill="RED")
        draw.text((5, 118), 'WaveShare', fill="WHITE", font=Font2)
        draw.text((5, 160), '1234567890', fill="GREEN", font=Font3)
        text = u"微雪电子"
        draw.text((5, 200), text, fill="BLUE", font=Font3)
        image = image.rotate(180)
        disp.ShowImage(image)

        time.sleep(30)

    def test_image(self):
        self.init_background()
        image = Image.open('/home/didier/tmp/LCD_Module_RPI_code/RaspberryPi/python/pic/LCD_2inch.jpg')
        image = image.rotate(180)
        disp.ShowImage(image)

        time.sleep(30)