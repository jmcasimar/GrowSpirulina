#!/usr/bin/env python
# encoding: utf-8
import os
import io
import base64
from time import sleep
import PySimpleGUI as sg
from PIL import Image, ImageTk

# Pendiente integrar serialController to manage connection between devices
class GUI:
    def __init__(self, logger = None, serialControl = None):
        # Logger
        self.log = logger
        # Serial controller
        self.ser = serialControl

        # Colors
        self.black90 = '#3c3c3b'
        self.blue1 = '#009e99'
        self.blue2 = '#00c4b3'
        self.blue3 = '#22d3c5'
        self.blue4 = '#7aded4'
        self.disable_color = (self.black90, self.black90)

        # Tables variables
        actualDirectory = os.getcwd()
        if actualDirectory.endswith('src'): self.str2log('Running from src', 0)
        else: self.str2log('Running from root', 0)

        # UI Variables
        self.heatResistor = True;
        self.airPump2 = True;
        self.airPumpEnable2 = True;
        self.airPump3 = True;
        self.airPumpEnable3 = True;

        # Main Window
        self.window = None
        self.isOpen = True

        self.DEF_BUTTON_COLOR = ('red', self.black90)

        # Add your new theme colors and settings
        sg.LOOK_AND_FEEL_TABLE['GrowSpirulina'] = {'BACKGROUND': self.black90,
                                                'TEXT': self.blue4,
                                                'INPUT': '#d2d2d2',
                                                'TEXT_INPUT': 'black',
                                                'SCROLL': '#d2d2d2',
                                                'BUTTON': ('black', self.blue1),
                                                'PROGRESS': ('white', self.blue2),
                                                'BORDER': 3, 'SLIDER_DEPTH': 3, 'PROGRESS_DEPTH': 3,
                                                }
        # Switch to use your newly created theme
        sg.theme('GrowSpirulina')


    def str2log(self, msg, level = 'DEBUG'):
        if self.log!=None:
            if (level==0 or level=='DEBUG'): self.log.debug(msg)
            elif (level==1 or level=='INFO'): self.log.info(msg)
            elif (level==2 or level=='WARNING'): self.log.warning(msg)
            elif (level==3 or level=='ERROR'): self.log.error(msg)
            elif (level==4 or level=='CRITICAL'): self.log.critical(msg)
        else:
            print(msg)

    def serialMsg(self, msg):
        if self.ser != None:
            self.ser.write(self.ser.generalControl, msg)
        else:
            self.str2log(msg, 3)

    def get_img_data(self, f, maxsize=(450, 300), first=False):
        """Generate image data using PIL"""
        img = Image.open(f)
        img.thumbnail(maxsize)
        if first:                     # tkinter is inactive the first time
            bio = io.BytesIO()
            img.save(bio, format="PNG")
            del img
            return bio.getvalue()
        return ImageTk.PhotoImage(img)

    def GraphicButton(self,text, key, image_data, maxsize=(200, 50)):
        '''
        A user defined element.  Use this function inside of your layouts as if it were a Button element (it IS a Button Element)
        Only 3 parameters are required.

        :param text: (str) Text you want to display on the button
        :param key:  (Any) The key for the button
        :param image_data: (str) The Base64 image to use on the button
        :param color: Tuple[str, str] Button color
        :param size: Tuple[int, int] Size of the button to display in pixels (width, height)
        :return: (PySimpleGUI.Button) A button with a resized Base64 image applied to it
        '''
        return sg.Button(text, image_data=self.get_img_data(image_data, maxsize,first=True), button_color=self.DEF_BUTTON_COLOR, font='Any 15', pad=(0, 0), key=key, border_width=0)

    def begin(self):
         layout = [
                      [sg.Text('Grow Spirulina', size=(13, 1), justification='center', font=("Helvetica 30 bold"), relief=sg.RELIEF_RIDGE, border_width=5)],
                      [sg.Text('_' * 45)],
                      [sg.Text('Resistencia:   '), sg.Checkbox('Habilitar', key='heatResistor', default=True, text_color='white')],
                      [sg.Text('Bomba Aire 2:'), sg.Checkbox('Automático', key='airPump2', default=True, text_color='white'),
                        sg.Radio('On', "air2", key='airPump2On', default=True, text_color='white', disabled=True),
                        sg.Radio('Off', "air2", key='airPump2Off', text_color='white', disabled=True)
                      ],
                      [sg.Text('Bomba Aire 3:'), sg.Checkbox('Automático', key='airPump3', default=True, text_color='white'),
                        sg.Radio('On', "air3", key='airPump3On', default=True, text_color='white', disabled=True),
                        sg.Radio('Off', "air3", key='airPump3Off', text_color='white', disabled=True )
                      ],
               ]

         self.window = sg.Window('GG GUI', layout, no_titlebar=False,
                            auto_size_text=True, finalize=True)

         self.str2log('GUI started correctly', level = 1)

    def run(self):
        try:
            event, values = self.window.read(timeout=0, timeout_key='timeout')
            if event!='timeout' and event!=None: self.str2log("{} - {}".format(event, values), 0)

            if values['airPump2']!=self.airPumpEnable2:
                self.airPumpEnable2 = values['airPump2']
                if self.airPumpEnable2:
                    self.window['airPump2On'].Update(disabled=True)
                    self.window['airPump2Off'].Update(disabled=True)
                    self.serialControl.write(self.serialControl.arduino, "e\n")
                    self.str2log('Bomba aire 2: Modo automático', 1)
                else:
                    self.window['airPump2On'].Update(disabled=False)
                    self.window['airPump2Off'].Update(disabled=False)
                    if values['airPump2On']:
                        self.serialControl.write(self.serialControl.arduino, "a\n")
                        self.str2log('Bomba aire 2: Modo manual (Encendida)', 1)
                    else:
                        self.serialControl.write(self.serialControl.arduino, "c\n")
                        self.str2log('Bomba aire 2: Modo manual (Apagada)', 1)

            if values['airPump3']!=self.airPumpEnable3:
                self.airPumpEnable3 = values['airPump3']
                if self.airPumpEnable3:
                    self.window['airPump3On'].Update(disabled=True)
                    self.window['airPump3Off'].Update(disabled=True)
                    slef.serialControl.write(self.serialControl.arduino, "f\n")
                    self.str2log('Bomba aire 3: Modo automático', 1)
                else:
                    self.window['airPump3On'].Update(disabled=False)
                    self.window['airPump3Off'].Update(disabled=False)
                    if values['airPump3On']:
                        self.serialControl.write(self.serialControl.arduino, "b\n")
                        self.str2log('Bomba aire 3: Modo manual (Encendida)', 1)
                    else:
                        self.serialControl.write(self.serialControl.arduino, "d\n")
                        self.str2log('Bomba aire 3: Modo manual (Apagada)', 1)

            if values['heatResistor']!=self.heatResistor:
                self.heatResistor = values['heatResistor']
                if self.heatResistor:
                    self.serialControl.write(self.serialControl.arduino, "4\n")
                    self.str2log('Resistencia habilitada', 1)
                else:
                    self.serialControl.write(self.serialControl.arduino, "3\n")
                    self.str2log('Resistencia deshabilitada', 1)

            if values['airPump2On'] and not self.airPump2:
                self.airPump2 = True
                self.serialControl.write(self.serialControl.arduino, "a\n")
                self.str2log('Bomba aire 2 encendida', 1)
            elif values['airPump2Off'] and self.airPump2:
                self.airPump2 = False
                self.serialControl.write(self.serialControl.arduino, "c\n")
                self.str2log('Bomba aire 2 apagada', 1)

            if values['airPump3On'] and not self.airPump3:
                self.airPump3 = True
                self.serialControl.write(self.serialControl.arduino, "b\n")
                self.str2log('Bomba aire 3 encendida', 1)
            elif values['airPump3Off'] and self.airPump3:
                self.airPump3 = False
                self.serialControl.write(self.serialControl.arduino, "d\n")
                self.str2log('Bomba aire 3 apagada', 1)

        except Exception as e:
            self.str2log("GUI Closed: {}".format(e), 2)
            self.isOpen = False
            #self.str2log(e, 3)

# Debug
def main():
    gui = GUI()
    gui.begin()
    while gui.isOpen:
        gui.run()

if __name__ == '__main__':
    main()
