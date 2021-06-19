//TODO: Fix init procedure
#include <FastLED.h>

#define DATA_PIN            2
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define BRIGHTNESS          96
#define LED_SEGMENTS        32
#define NUM_LEDS            144
CRGB leds[NUM_LEDS];


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

void showNewData() {
  if (newData == true) {
    if (inputLength % 4 == 0) {
      for (int i = 0; i < inputLength / 4; i++) {
        for (int j = 0; j < NUM_LEDS/LED_SEGMENTS; j++) {
          leds[(int)receivedChars[i*4] * NUM_LEDS/LED_SEGMENTS + j] = CRGB((int)receivedChars[i*4+1], (int)receivedChars[i*4+2], (int)receivedChars[i*4+3]);
        }
      }
      FastLED.show();
    }
    inputLength = 0;
    newData = false;
  }
}
