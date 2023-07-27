import logging

import jsonpath_rw_ext

# '{}_{}_{}_{}'.format(s1, i, s2, f)
from dadou_utils.files.abstract_json_manager import AbstractJsonManager
from dadou_utils.utils_static import COLOR
from dadou_utils.utils_static import JSON_AUDIOS, JSON_COLORS, JSON_MAPPINGS, \
    JSON_LIGHTS


class GloveJsonManager(AbstractJsonManager):
    logging.info("start json manager")

    colors = None
    lights = None
    lights_seq = None

    def __init__(self, config):
        self.config = config
        component = [self.config[JSON_COLORS], self.config[JSON_LIGHTS]]

        super().__init__(config, component)

    def get_lights(self, key):
        return self.get_dict_from_list(self.json_files[self.config[JSON_LIGHTS]], "keys", key)

    def get_color(self, key):
        result = jsonpath_rw_ext.match('$.colors[?name~' + key + ']', self.json_files[self.config[JSON_COLORS]])
        logging.debug(result)
        if len(result) > 0:
            json_color = result[0][COLOR]
            return (int(json_color['red']), int(json_color['green']), int(json_color['blue']))
        else:
            logging.error("no color" + key)
            return None

    @staticmethod
    def get_attribut(json_object, key):
        if key in json_object:
            return json_object[key]
        else:
            return None

    def get_mappings(self, key: str, mapping_type: str) -> str:
        if key:
            result = jsonpath_rw_ext.match("$."+mapping_type+"[?(keys[*]~'"+key+"')]", self.json_files[self.config[JSON_MAPPINGS]])
            return self.standard_return(result, True, 'value')
        else:
            logging.error("input str None")
