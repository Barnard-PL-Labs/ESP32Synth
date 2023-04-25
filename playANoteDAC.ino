#include "AudioTools.h"
#include "StkAll.h"
#include "AudioLibs/AudioKit.h"

I2SStream i2s;
ArdStreamOut output(i2s, 1);

#define BUTTON_LEFT 0
#define BUTTON_RIGHT 35
bool scheduleNote = false;

uint16_t sample_rate = 32000;
uint8_t channels = 2;
uint16_t max_amp = 32000;
SineFromTable<int16_t> sine1(max_amp);      // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sine1); // Stream generated from sine wave
AudioKitStream out;
int sound_len = 1024;
StreamCopy copier(out, sound, sound_len); // copies sound into i2s
int freq = 122;
uint16_t amp = 0;

volatile bool switchNote0 = false;
volatile bool switchNote1 = false;
volatile bool note0OnOff = false;
volatile bool note1OnOff = false;

int freq0 = 65.41;
int freq1 = 73.42;
int freq2 = 82.41;
int freq3 = 87.31;
int freq4 = 98.00;
int freq5 = 110.00;
int freq6 = 123.47;

void noteZero()
{
  note0OnOff = digitalRead(21);
  switchNote0 = true;
}

void noteOne()
{
  note1OnOff = digitalRead(2);
  switchNote1 = true;
}

void setup()
{
  Serial.begin(115200);

  pinMode(21, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(21), noteZero, CHANGE);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), noteOne, CHANGE);

  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.bits_per_sample = 16;
  cfg.sample_rate = Stk::sampleRate();
  cfg.channels = 1;
  cfg.pin_bck = 26;
  cfg.pin_ws = 25;
  cfg.pin_data = 22;
  i2s.begin(cfg);

  auto sinecfg = sine1.defaultConfig();
  sinecfg.channels = channels;
  sinecfg.sample_rate = sample_rate;
  sine1.setMaxAmplitudeStep(5000);
  sine1.begin(sinecfg, freq);
  sine1.setAmplitude(0);
}

void loop()
{

  if (switchNote0)
  {
    amp = ((int)!note0OnOff) * max_amp;
    sine1.setAmplitude(amp);
    sine1.setFrequency(freq0);
    switchNote0 = false;
  }
  if (switchNote1)
  {
    amp = ((int)!note1OnOff) * max_amp;
    sine1.setAmplitude(amp);
    sine1.setFrequency(freq1);
    switchNote1 = false;
    Serial.print("here");
  }
  Serial.print(digitalRead(2));

  copier.copy();
}