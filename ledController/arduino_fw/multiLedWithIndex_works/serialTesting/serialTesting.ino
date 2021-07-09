#define LED_SEGMENTS        16

const unsigned int MAX_INPUT = LED_SEGMENTS * 4 + 1; // {index}{r}{g}{b}...{eof}

void setup () {
  Serial.begin (115200);
}

void process_data (const char * data) {
  Serial.println(data);
}

void processIncomingByte (const byte inByte) {
  static char input_line[MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte) {
    case '\n':                            // end of text
      input_line[input_pos] = 0;          // terminating null byte
      process_data(input_line);           // terminator reached! process input_line here ...
      input_pos = 0;                      // reset buffer for next time
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
