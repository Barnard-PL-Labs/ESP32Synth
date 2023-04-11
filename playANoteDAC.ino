#include "AudioTools.h"
#include "StkAll.h"

I2SStream i2s;
ArdStreamOut output(i2s, 1);
Clarinet clarinet(440);
Voicer voicer;
int note = 50; // starting midi note
int group = 0;
float amplitude = 64;
#define BUTTON_LEFT 0
#define BUTTON_RIGHT 35
bool scheduleNote = false;

void setup() {
  Serial.begin(115200);

  voicer.addInstrument(&clarinet, group);
  
  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.bits_per_sample = 16;
  cfg.sample_rate = Stk::sampleRate();
  cfg.channels = 1;
  cfg.pin_bck = 26;
  cfg.pin_ws = 25;
  cfg.pin_data = 22;
  i2s.begin(cfg);

  attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT), playANote, RISING);

}

void playANote() {
  scheduleNote = true;
}

void loop() {

  if (scheduleNote) {
    // play note
    voicer.noteOn( note, amplitude, group);
    long endTime = millis()+1000;
    while (millis()<endTime){
        output.tick( voicer.tick() );
    }
  
    // stop playing
    voicer.noteOff( note,amplitude, group);
    endTime = millis()+100;
    while (millis()<endTime){
        output.tick( voicer.tick() );
    }
    scheduleNote = false;
  }
}
