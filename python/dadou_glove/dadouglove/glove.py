import time

import board

from dadouglove.glove_config import IH, IM, IB, MH, MM, MB, AH, AM, AB, OH, OM, OB, DUAL_GLOVE_9DOF_RIGHT, config
from dadouglove.glove_keypad import GloveKeys
from dadouglove.menu import BUTTONS_LAYOUT
from dadou_utils.utils_static import LONG, SHORT, ROW_GLOVE_PINS, COLUMN_GLOVE_PINS


class Glove:

    glove_keys = GloveKeys(((IH, IM, IB), (MH, MM, MB), (AH, AM, AB), (OH, OM, OB)),
                         (config[ROW_GLOVE_PINS]),
                         (config[COLUMN_GLOVE_PINS]))
    mode = DUAL_GLOVE_9DOF_RIGHT
    buttons_layout = BUTTONS_LAYOUT[mode]
    last_key = None

    def get_action(self):
        key = self.glove_keys.check()
        if key:
            if isinstance(key, list):
                key = key[0]

            action = self.buttons_layout[key]
            if len(action) == 1:
                return action
            else:
                if key != self.last_key:
                    self.last_key = key
                    time.sleep(0.5)
                    if self.glove_keys.check() and self.glove_keys.check() == key:
                        return action[LONG]
                    else:
                        return action[SHORT]
                else:
                    return action[LONG]

