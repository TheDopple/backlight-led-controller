//TODO: Fix init procedure

#include <FastLED.h>

#define DATA_PIN            2
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define BRIGHTNESS          96
#define LED_SEGMENTS        36
#define NUM_LEDS            144

CRGB leds[NUM_LEDS];

const byte rxByteLength = LED_SEGMENTS * 3; //LED count * RGB (3B), max pld len

char receivedBytes[rxByteLength];
boolean newData = false;

void setup() {
  //This is the programming USB port
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  recv();
  showNewData();
}

void recv() {
  int serialReadIndex = 0;
  char rc;
 
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    
    receivedBytes[serialReadIndex] = rc;
    serialReadIndex++;
    
    if (serialReadIndex == rxByteLength){
      newData = true;
    }
  }
}

void showNewData() {
  int ledsPerSegment = NUM_LEDS / LED_SEGMENTS;
  if (newData == true) {
    for (int segmentIndex = 0; segmentIndex < rxByteLength / 3; segmentIndex++) {
      int r = (int)receivedBytes[segmentIndex * 3 + 0];
      int g = (int)receivedBytes[segmentIndex * 3 + 1];
      int b = (int)receivedBytes[segmentIndex * 3 + 2];

      for (int j = 0; j < ledsPerSegment; j++) {
        leds[segmentIndex * ledsPerSegment + j] = CRGB(r, g, b);
      }
    }
    FastLED.show();
  }
  newData = false;
}
