import serial

class serialController():
    """
        PARAMS:
            port(str)
                Microcontroller serial com port
            baud(int)
                serial baud rate
    """
    def __init__(self, port="COM6", baud=115200):
        #TODO: Add remaining serial port communication controls
        try:
            self.ser = serial.Serial(port=port, baudrate=baud)
            print("Serial port open")
        except:
            print("Serial port issues")