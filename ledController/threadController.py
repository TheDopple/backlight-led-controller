"""
Completely untested, no idea if any of this works
"""

# Builtins
import argparse
import time
import threading

# Pypis
import ctypes
import win32gui
from keyboard import add_hotkey

# Local
import ledController


# Globals
PRIMARY_MONITOR_WIDTH = ctypes.windll.user32.GetSystemMetrics(0)
PRIMARY_MONITOR_HEIGHT = ctypes.windll.user32.GetSystemMetrics(1)

LED_CONTROLLER = ledController.ledController("COM8",30,8,240)
LED_CONTROLLER_THREAD = threading.Thread(target=LED_CONTROLLER.start)
LED_ACTIVATOR = False

def auto_activator_toggle():
    LED_ACTIVATOR = not LED_ACTIVATOR

add_hotkey("windows+5", lambda: auto_activator_toggle())

while True:
    try:
        if LED_ACTIVATOR:
            fg_window = win32gui.GetWindowRect(ctypes.windll.user32.GetForegroundWindow()) #returns (x,y,w,h)
            if fg_window[0] == 0 and fg_window[1] == 0 and fg_window[2] == PRIMARY_MONITOR_WIDTH and fg_window[2] == PRIMARY_MONITOR_HEIGHT:
                LED_CONTROLLER_THREAD.start()
            else:
                LED_CONTROLLER_THREAD.stop()
                time.sleep(1)
        else:
            time.sleep(1)
    except:
        print("Exiting")