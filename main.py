#!/bin/python3
'''Transforms images into SS14 markup text.'''

from sys import argv
from PIL import Image

cli = False

try:
    # Try importing PyQt6 for GUI.
    from PyQt6.QtWidgets import QApplication, QMainWindow, QWidget, QPushButton, QVBoxLayout, QHBoxLayout, QTextEdit, QLabel, QCheckBox, QFileDialog
    from PyQt6.QtCore import Qt
except:
    # Create QMainWindow class so python doesnt get angry when creating Window class
    class QMainWindow:
        def __init__(self):
            pass
    print("Forced CLI Mode: Cant load PyQt6.")
    cli = True

# Standard SS14 paper symbol limit is 10000 (at least in RMC-14)
symbol_limit = 10050
use_limit = True

full_color = False

image_size = {"w":0, "h":0}

path = ""
pixel = "██"


class color:
    def __init__(self, given):

        self.r = f'{given[0]:02x}'
        self.g = f'{given[1]:02x}'
        self.b = f'{given[2]:02x}'

        if len(given) > 3:
            self.a = f'{given[3]:02x}'
        else:
            self.a = "ff"

        if not full_color:
            self.r = self.r[0]
            self.g = self.g[0]
            self.b = self.b[0]
            self.a = self.a[0]

    def get_color(self):
        '''returns rgb or rrggbb color'''
        local = f"{self.r}{self.g}{self.b}"
        if self.a not in ["f", "ff"]:
            local += f"{self.a}"
        return local

def transform():
    '''Opens image and transforms it into ss14 markup text.'''
    text = ""
    line = ""

    pr_pixel = ""

    try:
        im = Image.open(path)
    except:
        return f"Error while loading {path}."

    pixels = list(im.getdata())
    image_size["w"], image_size["h"] = im.size

    ind = 1
    # TODO rewrite that
    for i in pixels:
        cur_pixel = color(i)

        # Add one pixel (two boxes)
        if cur_pixel.a in ['0', '00']:
            line += '  '
        elif cur_pixel.get_color() == pr_pixel:
            line += pixel
        else:
            line += "[color=#" + cur_pixel.get_color() + "]" + pixel
            pr_pixel = cur_pixel.get_color()

        # Compare next pixel pos with img width
        if ind != image_size["w"]:
            ind += 1
            continue

        # Break for loop if text bigger then limit (6000 symbols)
        if len(text + line) > symbol_limit and use_limit:
            break

        text += line + "\n"

        line = ""
        ind = 1

    return text


class Window(QMainWindow):
    '''Main and only window'''
    def __init__(self):
        super().__init__()

        #Buttons layout
        layout_button = QHBoxLayout()

        button_reload = QPushButton("Reload")
        button_reload.clicked.connect(self.button_reload_clicked)
        layout_button.addWidget(button_reload)

        checkbox_limit = QCheckBox("Use limit")
        checkbox_limit.setChecked(use_limit)
        checkbox_limit.checkStateChanged.connect(self.checkbox_limit_checked)
        layout_button.addWidget(checkbox_limit)

        checkbox_fullcolor = QCheckBox("Use #RRGGBB")
        checkbox_fullcolor.setChecked(full_color)
        checkbox_fullcolor.checkStateChanged.connect(self.checkbox_fullcolor_checked)
        layout_button.addWidget(checkbox_fullcolor)

        button_open = QPushButton("Open file")
        button_open.clicked.connect(self.button_open_clicked)
        layout_button.addWidget(button_open)

        #Main layout
        layout_main = QVBoxLayout()
        layout_main.addLayout(layout_button)

        self.text_edit = QTextEdit()
        self.text_edit.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)
        layout_main.addWidget(self.text_edit)

        self.label_info = QLabel("Image is not loaded, yet")
        self.label_info.setAlignment(Qt.AlignmentFlag.AlignCenter)
        layout_main.addWidget(self.label_info)

        widget = QWidget()
        widget.setLayout(layout_main)
        self.setCentralWidget(widget)
        self.setWindowTitle("SS14fyImage")
        self.setGeometry(0, 0, 350, 500)
        self.show()

    def update_label(self, length):
        '''Updates info label with image size and text length'''
        self.label_info.setText(f"{image_size['w']}x{image_size['h']} {length}/{symbol_limit}")

    def update_textedit(self, text):
        '''Updates text edit with transformed image.'''
        self.text_edit.setText(text)

    def button_reload_clicked(self):
        text = transform()
        self.update_label(len(text))
        self.update_textedit(text)

    def checkbox_limit_checked(self, state):
        global use_limit
        use_limit = state == Qt.CheckState.Checked

    def checkbox_fullcolor_checked(self, state):
        global full_color
        full_color = state == Qt.CheckState.Checked

    def button_open_clicked(self):
        global path
        path = QFileDialog.getOpenFileName(self)[0]

        text = transform()
        self.update_label(len(text))
        self.update_textedit(text)


if __name__ == "__main__":
    if len(argv) > 1:
        path = argv[1]
    if cli:
        if path == "":
            path = input('Path to image: ')
        text = transform()
        print(f"{path}: {image_size['w']}x{image_size['h']} {len(text)}/{symbol_limit}\n\n{text}")
        input() # For Windows compatibility probably
        exit()

    App = QApplication(argv)
    window = Window()
    exit(App.exec())
