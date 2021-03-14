import threading
import time

import serialController
from ScreenColorAnalyzer import screen_avg

def main():
    print("Starting LED Controller")
    sc = serialController.serialController(port="COM6", baud=115200)
    while sc.ser.isOpen():
        screenData = screen_avg(getTopRow=True)
        
        if screenData["topRow"] != None:
            print(sc.ser.write(screenData["topRow"]))

        time.sleep(1)
        #print(sc.ser.readlines())

if __name__=="__main__":
    main()