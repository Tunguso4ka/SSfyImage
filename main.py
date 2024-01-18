#!/bin/python3

# IMPORTS
from PIL import Image
from sys import argv
from os.path import exists

# DEFS
# translates rgba to hex code. Stolen from stackoverflow. Edited slighly by me.
def rgba2hex(r=0, g=0, b=0, a=0):
    if a != 255 or force_rgba: return '#{:02x}{:02x}{:02x}{:02x}'.format(r, g, b, a)
    else: return '#{:02x}{:02x}{:02x}'.format(r, g, b)

# translates text to bool (True, False). because python bool() works like shit
def make_bool(text):
    if text.lower() in ['1', 't', 'true']: return True
    return False

# activates with app start
def on_app_activate(app):
    # define elements

    # Main Window
    global main_window
    main_window = Gtk.ApplicationWindow(application=app)

    # Main box
    main_box = Gtk.Box(orientation=1, spacing=10)
    main_box.props.margin_top = main_box.props.margin_bottom = main_box.props.margin_start = main_box.props.margin_end = 10
    button_box = Gtk.Box(orientation=0, spacing=10)

    # Scroll view
    scrolled_window = Gtk.ScrolledWindow()

    # Text view
    global text_view
    text_view = Gtk.TextView()
    text_view.set_vexpand(True)

    # Size and Symbol count label
    global info_label
    info_label = Gtk.Label().new('Size: 0x0 Symbols: 0')

    # Use limit button
    limit_checkbutton = Gtk.CheckButton(label='Use limit')
    limit_checkbutton.set_active(use_limit)
    limit_checkbutton.connect('toggled', on_limit_checkbutton_toggle)

    # Force RGBA button
    rgba_checkbutton = Gtk.CheckButton(label='Force RGBA')
    rgba_checkbutton.set_active(force_rgba)
    rgba_checkbutton.connect('toggled', on_rgba_checkbutton_toggle)

    # Ignore size button
    ignoresize_checkbutton = Gtk.CheckButton(label='Ignore size')
    ignoresize_checkbutton.set_active(ignore_size)
    ignoresize_checkbutton.connect('toggled', on_ignoresize_checkbutton_toggle)

    # Choose image button
    file_button = Gtk.Button(label='Choose image')
    file_button.set_hexpand(True)
    file_button.connect('clicked', on_file_button_click)

    # set childs
    button_box.append(limit_checkbutton)
    button_box.append(rgba_checkbutton)
    button_box.append(ignoresize_checkbutton)
    button_box.append(file_button)

    scrolled_window.set_child(text_view)

    main_box.append(button_box)
    main_box.append(scrolled_window)
    main_box.append(info_label)

    main_window.set_child(main_box)

    # present
    main_window.present()

# activates on select image button click
def on_file_button_click(button):
    file_dialog = Gtk.FileDialog().new()

    # filters
    file_filter = Gtk.FileFilter()

    file_filter.set_name('Image files')
    file_filter.add_mime_type("image/png")
    file_filter.add_mime_type("image/jpeg")

    filters = Gio.ListStore.new(Gtk.FileFilter)
    filters.append(file_filter)

    file_dialog.set_filters(filters)
    file_dialog.set_default_filter(file_filter)

    # open
    file_dialog.open(main_window, None, file_dialog_open_callback)

# activates on selecting or exiting file chooser
def file_dialog_open_callback(dialog, result):
    global image_path
    try:
        file = dialog.open_finish(result)
        if file is not None:
            image_path = file.get_path()
            translate_image()
    except Exception as e: print(f"Error while opening file: {e}")

# activates on limit checkbutton toggle
def on_limit_checkbutton_toggle(button):
    global use_limit
    use_limit = button.get_active()
    translate_image()

# activates on force rgba checkbutton toggle
def on_rgba_checkbutton_toggle(button):
    global force_rgba
    force_rgba = button.get_active()
    translate_image()

# activates on ignore_size checkbutton toggle
def on_ignoresize_checkbutton_toggle(button):
    global ignore_size
    ignore_size = button.get_active()
    translate_image()

def translate_image():
    if not exists(image_path): return
    
    if not force_cli: buffer = text_view.get_buffer()

    # imports image
    image = Image.open(image_path)
    pixels = image.load()

    # sets image size
    max_w, max_h = 21, 26 # 21 26 on standard (not office) paper
    image_w, image_h = image.size

    # "trims" image if its too big. ignore_size disables this
    if max_h > image_h or ignore_size: max_h = image_h
    if max_w > image_w or ignore_size: max_w = image_w

    # translates picture into ss14 rich text
    h = 0
    last_color = ''
    text = ''
    a = 255
    while h < max_h:
        w = 0
        line = ''
        while w < max_w:
            try: r, g, b, a = pixels[w, h]
            except: r, g, b = pixels[w, h]
            color = rgba2hex(r, g, b, a)
            if last_color != color:
                line += f'[color={color}]'
                last_color = color
            line += '██'
            w += 1
        
        if len(text + line) > symbol_limit and use_limit: break

        text += line + '\n'
        h += 1

    if force_cli:
        print(text)
    else:
        buffer.set_text(text)
        info_label.set_label(f'Size: {max_w}x{max_h} Symbols: {len(text)}/{symbol_limit}')

# CODE
# variables
use_limit = True
force_rgba = False
ignore_size = False
force_cli = False

image_path = ''
symbol_limit = 6000

# checks and sets arguments
for i in argv[1:]:
    arg = i.split('=', 1)
    if arg[0] not in globals(): continue

    arg_type = type(globals()[arg[0]])
    if arg_type == int: globals()[arg[0]] = int(arg[1])
    elif arg_type == str: globals()[arg[0]] = arg[1]
    elif arg_type == bool: globals()[arg[0]] = make_bool(arg[1])

if force_cli:
    if image_path == '': image_path = input('Input image path: ')
    translate_image()
else:
    import gi
    gi.require_version("Gtk", "4.0")
    from gi.repository import Gtk, Gio

    app = Gtk.Application(application_id='ssfy.image')
    app.connect('activate', on_app_activate)
    app.run(None)
