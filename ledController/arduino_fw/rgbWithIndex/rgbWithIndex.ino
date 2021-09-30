//TODO: Fix init procedure

#include <FastLED.h>

#define DATA_PIN            2
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define BRIGHTNESS          255 //255 max
#define NUM_LEDS            30

CRGB leds[NUM_LEDS];
const byte maxChars = NUM_LEDS * 4; //LED count * indexRGB (4B)

char receivedChars[maxChars];
int inputLength = 0;
bool newData = false;

void setup() {
    Serial.begin(115200);
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 0);
    }
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
                if (ndx >= maxChars) {
                    ndx = maxChars - 1;
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
        if (inputLength % 4 == 0) {
            for (int i = 0; i < inputLength / 4; i++) {
                int ledIndex = (int)receivedChars[i * 4];
                int r = (int)receivedChars[i * 4 + 1];
                int g = (int)receivedChars[i * 4 + 2];
                int b = (int)receivedChars[i * 4 + 3];
                leds[ledIndex] = CRGB(r, g, b);
            }
            FastLED.show();
            Serial.println("ACK");
        }
        inputLength = 0;
        newData = false;
    }
}
