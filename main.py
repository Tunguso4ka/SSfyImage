#!/bin/python3

# IMPORTS
from PIL import Image
from sys import argv

# DEFS
# translates rgba to hex code. Stolen from stackoverflow. Edited slighly by me.
def rgba2hex(r=0, g=0, b=0, a=0):
    if rgba: return '#{:02x}{:02x}{:02x}{:02x}'.format(r, g, b, a)
    return '#{:02x}{:02x}{:02x}'.format(r, g, b)

# translates text to bool (True, False). because python bool() works like shit
def make_bool(text):
    if text.lower() in ['1', 't', 'true']: return True
    return False

# CODE
# variables
force = False
rgba = False
ignore_size = False

# checks and sets arguments
for i in argv[1:]:
    arg = i.split('=', 1)
    if arg[0] in globals(): globals()[arg[0]] = make_bool(arg[1])

# debug info
print(f'force: {force}, rgba: {rgba}, ignore_size: {ignore_size}\n')

# imports image
image = Image.open('photo.png')
pixels = image.load()

# sets image size
max_w, max_h = 21, 26 # 21 26 on standard (not office) paper
image_w, image_h = image.size

# "trims" image if its too big. ignore_size disables this
if max_h > image_h and not ignore_size: max_h = image_h
if max_w > image_w and not ignore_size: max_w = image_w

# translates picture into ss14 rich text
h = 0
symb = 0
last_color = ''
while h < max_h:
    w = 0
    line = ''
    while w < max_w:
        r, g, b, a = pixels[w, h]
        color = rgba2hex(r, g, b, a)
        if last_color != color:
            line += f'[color={color}]'
            last_color = color
        line += '██'
        w += 1
    symb += len(line)
    if symb <= 6000 or force: print(line)
    else: print(f'\ntoo much symbols: {symb}')
    h += 1

# debug info
print(f'\nmaximum height: {max_h}, maximum width {max_w}, symbols: {symb}')
