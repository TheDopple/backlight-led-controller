
"""
    
"""
import argparse
import serial
import time
import d3dshot

class ledController:
    def __init__(self, serialPort, xSegs, ySegs, refreshRate_s, baud=115200, resetSer_s=300, monitorIndex=0):
        self.xSegs = int(xSegs)
        self.ySegs = int(ySegs)
        self.refreshRate_s = int(refreshRate_s)
        self.serialPort = serialPort.strip().upper()
        self.baud = int(baud)
        self.resetSer_s = int(resetSer_s)

        self.d = d3dshot.create()
        self.d.display = self.d.displays[int(monitorIndex)]
        self.pixelColorState = {}

        self.resetTimer = time.time()
        self._connectSer()

    def _connectSer(self):
        self.ser = serial.Serial(port=self.serialPort, baudrate=self.baud)
        return

    def _resetSer(self):
        print("\n\n\nResetting Serial...\n\n\n")

        self.ser.close()
        time.sleep(.1)
        self._connectSer()

        self.resetTimer = time.time()
        self.start()
        return

    def _serialDataHandler(self, img):
        #Build serial data
        serialData = [60] #< to start serial data
        for pixelIndex in range(self.xSegs):
            r = img[pixelIndex,0][0] #1B Red value uint_8
            g = img[pixelIndex,0][1] #1B Blue value uint_8
            b = img[pixelIndex,0][2] #1B Green value uint_8

            # Only add pixels with changed RGB values to serial payload
            if self.pixelColorState.get(pixelIndex, None) != [r,g,b]:
                if r == 62: r+=1
                if g == 62: g+=1
                if b == 62: b+=1

                serialData.append(pixelIndex)
                serialData.append(r) 
                serialData.append(g) 
                serialData.append(b) 
                self.pixelColorState.update({pixelIndex:[r,g,b]})
        serialData.append(62) #> to complete serial data

        if len(serialData) > 2:
            self.ser.write(bytes(serialData))
            self.ser.reset_input_buffer()
        return

    def _takeCurrentScreenshot(self):
        #Screenshots primary monitor, reduces pixels to xSegs,ySegs for color averaging
        return self.d.screenshot().resize(size=(self.xSegs, self.ySegs)).load()

    def start(self):
        try:
            while self.ser.isOpen():
                cycleTimeStart = time.time()
                self._serialDataHandler(self._takeCurrentScreenshot())
                print("Cycle runtime: {} ms".format(round(time.time() - cycleTimeStart, 2)))

                if time.time() - self.resetTimer >= self.resetSer_s: 
                    self._resetSer()
                    break

                #time.sleep(.5)  #testing
        except:
            self._exit()

    def _exit(self):
        #Set all LEDs to 0,0,0 and close serial cxn
        pixelClearPld = []
        pixelClearPld.append(60)
        for i in range(0,self.xSegs):
            pixelClearPld.append(i)
            pixelClearPld.append(0)
            pixelClearPld.append(0)
            pixelClearPld.append(0)
        pixelClearPld.append(62)
        self.ser.write(bytes(pixelClearPld))
        self.ser.close()

if __name__=="__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('serialPort', help="Serial port")
    parser.add_argument('--xSegs', '-x', default=30, help='X LED count.')
    parser.add_argument('--ySegs', '-y', default=8, help='Y screen segment count. Used for sampling resolution.')
    parser.add_argument('--refreshRate', '-z', default=240, help='Refresh rate Hz') #2x monitor Hz cuz why not
    parser.add_argument('--baud', '-b', default=115200, help='Red RGB value bias')
    parser.add_argument('--resetSer_s', '-r', default=300, help='Green RGB value bias')
    parser.add_argument('--monitorIndex', '-m', default=0, help='Blue RGB value bias')
    args = parser.parse_args()
    
    l = ledController(args.serialPort, int(args.xSegs), int(args.ySegs), int(args.refreshRate), int(args.baud), int(args.resetSer_s), int(args.monitorIndex))
    
    l.start()