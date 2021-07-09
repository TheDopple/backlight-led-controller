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
const unsigned int MAX_INPUT = LED_SEGMENTS * 4 + 1; // RGB/LED_Segments + EOL chr //{index}RGB = pld/segment
unsigned int segmentUpdateCt = 0;

void setup () {
  Serial.begin (115200);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void process_data (const char * data) {
  
  Serial.print("First segment to data: ");
  Serial.print(data[0], DEC);
  Serial.print(data[1], DEC);
  Serial.print(data[2], DEC);
  Serial.println(data[3], DEC);
  
  for (int s = 0; s < segmentUpdateCt; s++) {
    int segmentIndex = data[s * 4 + 0];
    Serial.print("Segment Index: ");
    Serial.println(segmentIndex, DEC);

    int r = data[s * 4 + 1];
    int g = data[s * 4 + 2];
    int b = data[s * 4 + 3];
    Serial.print("R: ");
    Serial.println(r, DEC);
    
    Serial.print("G: ");
    Serial.println(g, DEC);
    
    Serial.print("B: ");
    Serial.println(b, DEC);

    for (int j = 0; j < LEDS_PER_SEGMENT; j++) {
      int ledIndex = segmentIndex * LEDS_PER_SEGMENT + j;
      //Serial.print("LED index: ");
      //Serial.println(ledIndex, DEC);
      leds[ledIndex] = CRGB(r, g, b);
    }
    FastLED.show();
  }
}

void processIncomingByte (const byte inByte) {
  static char input_line[MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte) {
    //THIS IS THE ISSUE '\n' == 10
    case '\n':   // end of text 
      if (input_pos % 4 == 0) {
        input_line[input_pos] = 0;          // terminating null byte
        segmentUpdateCt = input_pos / 4;
        process_data(input_line);           // terminator reached! process input_line here ...
        input_pos = 0;                      // reset buffer for next time
        break;
      }
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
