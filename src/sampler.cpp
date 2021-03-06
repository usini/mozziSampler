#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

#include <mozzi_midi.h>
#include <MozziGuts.h>
#include <mozzi_fixmath.h>
#include <Sample.h>

// set other constants
#define CONTROL_RATE 64 // low to save processor
#include "pad0.h"
#include "pad1.h"
#include "pad2.h"
#include "pad3.h"
#include "pad4.h"
#include "pad5.h"
#include "pad6.h"
#include "pad7.h"
#include "pad8.h"
#include "pad9.h"
#include "pad10.h"
#include "pad11.h"

Sample <pad0_NUM_CELLS, AUDIO_RATE> pad0(pad0_DATA);
Sample <pad1_NUM_CELLS, AUDIO_RATE> pad1(pad1_DATA);
Sample <pad2_NUM_CELLS, AUDIO_RATE> pad2(pad2_DATA);
Sample <pad3_NUM_CELLS, AUDIO_RATE> pad3(pad3_DATA);
Sample <pad4_NUM_CELLS, AUDIO_RATE> pad4(pad4_DATA);
Sample <pad5_NUM_CELLS, AUDIO_RATE> pad5(pad5_DATA);
Sample <pad6_NUM_CELLS, AUDIO_RATE> pad6(pad6_DATA);
Sample <pad7_NUM_CELLS, AUDIO_RATE> pad7(pad7_DATA);
Sample <pad8_NUM_CELLS, AUDIO_RATE> pad8(pad8_DATA);
Sample <pad9_NUM_CELLS, AUDIO_RATE> pad9(pad9_DATA);
Sample <pad10_NUM_CELLS, AUDIO_RATE> pad10(pad10_DATA);
Sample <pad11_NUM_CELLS, AUDIO_RATE> pad11(pad11_DATA);
int totalgain = 2;

struct gainstruct {
  byte gain0;
  byte gain1;
  byte gain2;
  byte gain3;
  byte gain4;
  byte gain5;
  byte gain6;
  byte gain7;
  byte gain8;
  byte gain9;
  byte gain10;
  byte gain11;
  byte gain12;
}
gains;

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {

  //Serial.begin(115200);
  //Serial.println("");
  pinMode(39, INPUT);
  pinMode(10, OUTPUT);

  //Serial.println("Drums M5StickC");

  if (!cap.begin(0x5A)) {
    //Serial.println("MPR121 not found, check wiring?");
    ESP.restart();
  }
  digitalWrite(10, LOW);

  pad0.setFreq((float) pad0_SAMPLERATE / (float) pad0_NUM_CELLS); // play at the speed it was recorded
  pad1.setFreq((float) pad1_SAMPLERATE / (float) pad1_NUM_CELLS); // play at the speed it was recorded
  pad2.setFreq((float) pad2_SAMPLERATE / (float) pad2_NUM_CELLS); // play at the speed it was recorded
  pad3.setFreq((float) pad3_SAMPLERATE / (float) pad3_NUM_CELLS); // play at the speed it was recorded
  pad4.setFreq((float) pad4_SAMPLERATE / (float) pad4_NUM_CELLS); // play at the speed it was recorded
  pad5.setFreq((float) pad5_SAMPLERATE / (float) pad5_NUM_CELLS); // play at the speed it was recorded
  pad6.setFreq((float) pad6_SAMPLERATE / (float) pad6_NUM_CELLS); // play at the speed it was recorded
  pad7.setFreq((float) pad7_SAMPLERATE / (float) pad7_NUM_CELLS); // play at the speed it was recorded
  pad8.setFreq((float) pad8_SAMPLERATE / (float) pad8_NUM_CELLS); // play at the speed it was recorded
  pad9.setFreq((float) pad9_SAMPLERATE / (float) pad9_NUM_CELLS); // play at the speed it was recorded
  pad10.setFreq((float) pad10_SAMPLERATE / (float) pad10_NUM_CELLS); // play at the speed it was recorded
  pad11.setFreq((float) pad11_SAMPLERATE / (float) pad11_NUM_CELLS); // play at the speed it was recorded
  startMozzi(CONTROL_RATE);
}

void Note(byte note)
{
  switch (note) {
    case 0: //F#1 X
      gains.gain0 = totalgain;
      pad0.start();
      break;
    case 1: //A#1
      gains.gain1 = totalgain;
      pad1.start();
      break;
    case 2: //B0 X
      gains.gain2 = totalgain;
      pad2.start();
      break;
    case 3: //C1 X
      gains.gain3 = totalgain;
      pad3.start();
      break;
    case 4: //C#1
      gains.gain4 = totalgain;
      pad4.start();
      break;
    case 5: //D1
      gains.gain5 = totalgain;
      pad5.start();
      break;
    case 6: //E1
      gains.gain6 = totalgain;
      pad6.start();
      break;
    case 7: //D#2
      gains.gain7 = totalgain;
      pad7.start();
      break;
    case 8: //C#2
      gains.gain8 = totalgain;
      pad8.start();
      break;
    case 9: //G1
      gains.gain9 = totalgain;
      pad9.start();
      break;
    case 10: //B1
      gains.gain10 = totalgain;
      pad10.start();
      break;
    case 11: //D2
      gains.gain11 = totalgain;
      pad11.start();
      break;
  }
}

void updateControl() {
  if (!digitalRead(39)) {
    ESP.restart();
  }
  currtouched = cap.touched();
  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Note(i);
      //Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      //Serial.print(i); Serial.println(" released");
    }
  }
  lasttouched = currtouched;
}


int updateAudio() {
  int asig = (int) ((long) pad0.next() * gains.gain0 +
                    pad1.next() * gains.gain1 +
                    pad2.next() * gains.gain2 +
                    pad3.next() * gains.gain3 +
                    pad4.next() * gains.gain4 +
                    pad5.next() * gains.gain5 +
                    pad6.next() * gains.gain6 +
                    pad7.next() * gains.gain7 +
                    pad8.next() * gains.gain8 +
                    pad9.next() * gains.gain9 +
                    pad10.next() * gains.gain10 +
                    pad11.next() * gains.gain11);
  return MonoOutput::from8Bit(asig << 2).clip();
}


void loop() {
  audioHook(); // required here
}

