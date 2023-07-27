import time
import unittest

from dadouglove.display.display import Display

display = Display()


class TestDisplay(unittest.TestCase):
    def test_menu(self):
        display.show_menu()

        display.display.background(display.background)
        time.sleep(60)

    def test_playlist(self):
        playlist = ['intro premier truc',
                    'deuxime truc',
                    'troisième truc',
                    'quatrième truc',
                    'cinquième truc',
                    'sixième truc',
                    'septième truc',
                    'huitième truc',
                    'neuvième truc',
                    'dixème truc']

        display.show_playlist(5, playlist)
        time.sleep(5)
        display.show_playlist(1, playlist)
        time.sleep(5)
        display.show_playlist(10, playlist)
        time.sleep(5)
        display.show_playlist(9, playlist)
        time.sleep(5)
        display.show_playlist(2, playlist)
        time.sleep(5)


if __name__ == '__main__':
    unittest.main()
