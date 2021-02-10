#include "MIDIUSB.h"
#include "ArduinoLog.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

char input[3];

void setup()
{

  Serial.begin(115200);
  Serial2.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);

  delay(100);
}

void sendNote(char mod, int note) {


  midiEventPacket_t noteOn = {0x09, 0x90 | 0, note, 64};
  MidiUSB.sendMIDI(noteOn);

  midiEventPacket_t noteOff = {0x08, 0x80 | 0, note, 64};
  MidiUSB.sendMIDI(noteOff);

  Log.notice("send note %i\n", note);


  //Log.notice("send midi %d, %d\n", note);
  /*int channel = 1;
    if (mod == 'A') {
    channel = 3;
    } else if (mod == 'B') {
    channel = 4;
    } else if (mod == 'C') {
    channel = 5;
    } else if (mod == 'D') {
    channel = 6;
    }

    MIDI.sendNoteOn(note, 127, channel);    // Send a Note (pitch 42, velo 127 on channel 1)
    delay(10);                // Wait for a second
    MIDI.sendNoteOff(note, 0, channel);
    Log.notice("midi note: %d on channel %d\n", note, channel );*/
}

void loop()
{

  // test monitor
  if (Serial2.available()) {
    String esp32Input = Serial2.readStringUntil('\n');
    esp32Input.toCharArray(input, 4);
    Serial.println(input);

    char note[2];
    note[0] = input[0];
    note[1] = input[1];
    //sendNote(input[1], atoi(note));
    sendNote(input[1], note[1]);

  }

}
