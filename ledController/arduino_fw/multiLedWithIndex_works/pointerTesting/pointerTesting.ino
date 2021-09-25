#define LED_SEGMENTS        8
#define NUM_LEDS            144


const int leds_per_segment = NUM_LEDS / LED_SEGMENTS;
const byte numChars = NUM_LEDS * 4; //LED count * indexRGB (4B)
char receivedChars[numChars];

boolean newData = false;
int inputLength = 0;


void setup() {

}

void pointerInit() {
  
}

void loop() {
    recvWithStartEndMarkers();
    showNewData();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                inputLength++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//<0000>

void showNewData() {
    if (newData == true) {
        Serial.println(receivedChars);
        inputLength = 0;
        newData = false;
    }
}
