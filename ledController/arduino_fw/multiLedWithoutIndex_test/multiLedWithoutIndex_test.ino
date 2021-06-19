//TODO: Fix init procedure

#include <FastLED.h>
#define DATA_PIN            2
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
#define BRIGHTNESS          96
#define LED_SEGMENTS        16
#define NUM_LEDS            144

CRGB leds[NUM_LEDS];

const int LEDS_PER_SEGMENT = NUM_LEDS / LED_SEGMENTS;
const unsigned int MAX_INPUT = LED_SEGMENTS * 3 + 1; // RGB/LED_Segments + EOL chr

void setup () {
  Serial.begin (115200);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void process_data (const char * data) {
  for (int segmentIndex = 0; segmentIndex < LED_SEGMENTS; segmentIndex++) {
    for (int j = 0; j < LEDS_PER_SEGMENT; j++) {
      leds[segmentIndex * LEDS_PER_SEGMENT + j] = CRGB((int)data[segmentIndex * 3 + 0], (int)data[segmentIndex * 3 + 1], (int)data[segmentIndex * 3 + 2]);
    }
    FastLED.show();
  }
}

void processIncomingByte (const byte inByte) {
  static char input_line[MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte) {
    case '\n':   // end of text
      input_line[input_pos] = 0;          // terminating null byte
      process_data(input_line);           // terminator reached! process input_line here ...
      input_pos = 0;                      // reset buffer for next time
      break;
      
    case '\r':                            // discard carriage return
      break;
      
    default:
      if (input_pos < (MAX_INPUT - 1))    // keep adding if not full ... allow for terminating null byte
        input_line[input_pos++] = inByte;
      break;
  }
}

void loop() {
  while (Serial.available () > 0) {       // if serial data available, process it
    processIncomingByte(Serial.read());
  }
}
