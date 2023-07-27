#include <MIDI.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
  MIDI.begin();
  Serial.begin(115200);
}

void loop()
{


  MIDI.sendNoteOn(42, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
  delay(1000);                // Wait for a second
  MIDI.sendNoteOff(42, 0, 1);
  delay(1000);

}
