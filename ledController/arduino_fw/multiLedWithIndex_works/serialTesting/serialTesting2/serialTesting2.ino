//TODO: Fix init procedure


#include <FastLED.h>

#define DATA_PIN            2
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define BRIGHTNESS          96
#define LED_SEGMENTS        8
#define NUM_LEDS            144
CRGB leds[NUM_LEDS];

const int leds_per_segment = NUM_LEDS / LED_SEGMENTS;
const byte numChars = NUM_LEDS * 4; //LED count * indexRGB (4B)
char receivedChars[numChars];

boolean newData = false;
int inputLength = 0;

void setup() {
  Serial.begin(115200);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0,0,0);
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
    if (inputLength % 4 == 0) {
      for (int i = 0; i < inputLength / 4; i++) {
        int segment_index = (int)receivedChars[i*4];
        int r = (int)receivedChars[i*4+1];
        int g = (int)receivedChars[i*4+2];
        int b = (int)receivedChars[i*4+3];

        Serial.print("Segment Index: ");
        Serial.println(segment_index);
        Serial.println(String("RGB: ") + r + " " + g + " " + b);

        for (int j = 0; j < leds_per_segment; j++) {
          leds[segment_index * leds_per_segment + j] = CRGB(r, g, b);
        }
      }
      FastLED.show();
    }
    inputLength = 0;
    newData = false;
  }
}
