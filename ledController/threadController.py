"""
    Seems to cap out around 16 segments
"""
import argparse
import serial
import time
import os
import d3dshot

def main(serialPort, xSegs, ySegs, refreshRate):
    print("Starting LED Controller...")

    #Serial cxn to Arduino
    resetSerial = False
    resetSerialTimer = 600 # resetSerial cxn every 600s Not implemented yet
    
    while not resetSerial:
        ser = serial.Serial(port=serialPort, baudrate=115200)

        d = d3dshot.create()
        d.display = d.displays[0]
    
        pixelColorState = {}
        while ser.isOpen():
            #Take screenshot
            img = d.screenshot()
            #Use resize for color averaging
            resizedImg = img.resize(size=(xSegs, ySegs)).load()

            #Build serial data
            serialData = []
            serialData.append(60) #< to start serial data
            for pixelIndex in range(xSegs):
                r = resizedImg[pixelIndex,0][0]
                g = resizedImg[pixelIndex,0][1]
                b = resizedImg[pixelIndex,0][2]

                if pixelColorState.get(pixelIndex, None) != [r,g,b]:
                    serialData.append(pixelIndex)
                    serialData.append(r) #1B Red value uint_8
                    serialData.append(g) #1B Blue value uint_8
                    serialData.append(b) #1B Green value uint_8
                    pixelColorState.update({pixelIndex:[r,g,b]})
                    print({pixelIndex:[r,g,b]})
            serialData.append(62) #> to complete serial data

            ser.write(bytes(serialData))
            ser.reset_input_buffer() #Dump rx buffer.  We don't care about serial returns

            #print("Cycle runtime: {} ms".format(round(time.time() - cycleTimeStart, 2))) # .1 s           
            time.sleep(1/refreshRate)
    
if __name__=="__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('serialPort', help="Serial port")
    parser.add_argument('--xSegs', '-x', default=16, help='X LED segment count. NOTE: LED Ct/Segments must be an int')
    parser.add_argument('--ySegs', '-y', default=8, help='Y screen segment count. Used for sampling resolution.')
    parser.add_argument('--refreshRate', '-z', default=120, help='Refresh rate Hz')
    #parser.add_argument('--redBias', '-r', default=16, help='Red RGB value bias') #Maybe?
    #parser.add_argument('--greenBias', '-g', default=16, help='Green RGB value bias')
    #parser.add_argument('--blueBias', '-b', default=120, help='Blue RGB value bias')
    args = parser.parse_args()
    
    main(args.serialPort, int(args.xSegs), int(args.ySegs), int(args.refreshRate))