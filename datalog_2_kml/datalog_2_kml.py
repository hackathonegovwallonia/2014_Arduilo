
import codecs
import os
import sys
import re

Str_path_base = '/home/patrick2/PycharmProjects/arduilo/'
Str_filename = '2014-04-04_DATALOG'

def read_file():
    #print('read_file')
    _str_file = None
    with codecs.open(os.path.join(Str_path_base, Str_filename + '.TXT'), 'r', encoding='utf-8') as _file:
        _str_file = _file.read()
        #print(_str_file)
    return _str_file

def save_file(str_file):
    #print('read_file')
    _str_file = None
    with codecs.open(os.path.join(Str_path_base, Str_filename + '.kml'), 'w', encoding='utf-8') as _file:
        _str_file = _file.write(str_file)


def clean_file(str_file):
    #print('clean_file')
    _lst_line_out = []
    _lst_line_in = str_file.split('\r\n')
    for _str_line in _lst_line_in:
        #_str_line.replace('\t', '#')
        #print(_str_line)
        if _str_line.find('Time: ') != -1:
            #print(_str_line)
            #if _str_line.find('0/0/0') != -1 \
            #   or _str_line.find('/0/') != -1 \
            #    or _str_line.find('''0* 0' 0.0"''') != -1:
            if _str_line.find("0* 0' 0.0") != -1:
                continue
            _lst_line_out.append(_str_line)
    return '\n'.join(_lst_line_out)


def kml_head():
    _str_head = '''<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://www.opengis.net/kml/2.2">
  <Placemark>
    <name>Simple placemark</name>
    <description>Attached to the ground. Intelligently places itself
       at the height of the underlying terrain.</description>
    <Point>
'''
    return _str_head

def kml_tail():
    _str_tail = '''
    </Point>
  </Placemark>
</kml>'''
    return _str_tail



def lines_to_kml(str_lines):
    print(lines_to_kml)
    _lst_lines_out = []
    _lst_lines = str_lines.split('\n')
    for _str_line in _lst_lines:
        print(_str_line)
        _re_lat = "Lat: ([+-]?\d\d?)\* (\d\d?)' (\d\d?\.\d\d?)"
        #_re_lat = 'Lat: ([+-]\d\d?)\* (\d\d?)''.*?(\d)'
        _re_lon = "Long: ([+-]?\d\d?)\* (\d\d?)' (\d\d?\.\d\d?)"
        _re_search_lat = re.search(_re_lat, _str_line)
        _int_lat_deg = int(_re_search_lat.group(1))
        _int_lat_min = int(_re_search_lat.group(2))
        _flt_lat_sec = float(_re_search_lat.group(3))
        _re_search_lon = re.search(_re_lon, _str_line)
        _int_lon_deg = int(_re_search_lon.group(1))
        _int_lon_min = int(_re_search_lon.group(2))
        _flt_lon_sec = float(_re_search_lon.group(3))

        _flt_lat = _int_lat_deg + _int_lat_min / 60 + _flt_lat_sec / 3600
        _flt_lon = _int_lon_deg + _int_lon_min / 60 + _flt_lon_sec / 3600
        _str_line_out = '      <coordinates>%(lat)f,%(lon)f</coordinates>' % {'lat': _flt_lat, 'lon':_flt_lon}
        _lst_lines_out.append(_str_line_out)
    _str_kml = kml_head() + '\n'.join(_lst_lines_out) + kml_tail()
    return _str_kml


def lines_to_geojson():
    pass


def main():
    _str_file = read_file()
    _str_file_clean = clean_file(_str_file)
    print(_str_file_clean)
    _str_coord = lines_to_kml(_str_file_clean)
    print(_str_coord)
    save_file(_str_coord)

main()