import serial

class LedController():
    """
        PARAMS:
            port(str)
                Microcontroller serial com port
            baud(int)
                serial baud rate

    """
    def __init__(self, port="COM6", baud=115200):
        #TODO: Add remaining serial port communication controls
        self.ser = serial.Serial(port=port, baudrate=baud)
        
    def setLed(index, r, g, b):
        """
            PARAMS:
                index(int)
                    led index
                r(int)
                    serial baud rate

        """
        ser.write()