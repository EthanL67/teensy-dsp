// Freeverb - High quality reverb effect
//
//  Teensy 3.5 or higher is required to run this example
//
// The SD card may connect to different pins, depending on the
// hardware you are using.  Uncomment or configure the SD card
// pins to match your hardware.
//
// Data files to put on your SD card can be downloaded here:
//   http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html
//
// This example code is in the public domain.

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2s_in;          // xy=582,483
AudioEffectFreeverb freeverb1; // xy=787,447
AudioMixer4 mixer1;            // xy=1006,477
AudioOutputI2S i2s_out;        // xy=1212.75,482
AudioConnection patchCord1(i2s_in, 0, freeverb1, 0);
AudioConnection patchCord2(i2s_in, 0, mixer1, 1);
AudioConnection patchCord3(freeverb1, 0, mixer1, 0);
AudioConnection patchCord4(mixer1, 0, i2s_out, 0);
AudioControlSGTL5000 sgtl5000_1; // xy=692.75,726
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN 10
#define SDCARD_MOSI_PIN 7 // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN 14 // Teensy 4 ignores this, uses pin 13

// Use these with the Teensy 3.5 & 3.6 & 4.1 SD card
// #define SDCARD_CS_PIN    BUILTIN_SDCARD
// #define SDCARD_MOSI_PIN  11  // not actually used
// #define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
// #define SDCARD_CS_PIN    4
// #define SDCARD_MOSI_PIN  11
// #define SDCARD_SCK_PIN   13

#define POT1_PIN A10
#define POT2_PIN A11
#define POT3_PIN A12
#define POT4_PIN A13
#define POT5_PIN A14
#define POT6_PIN A15

void setup()
{
  Serial.begin(9600);

  pinMode(POT1_PIN, INPUT_DISABLE);
  pinMode(POT2_PIN, INPUT_DISABLE);
  pinMode(POT3_PIN, INPUT_DISABLE);
  pinMode(POT4_PIN, INPUT_DISABLE);
  pinMode(POT5_PIN, INPUT_DISABLE);
  pinMode(POT6_PIN, INPUT_DISABLE);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN)))
  {
    // stop here, but print a message repetitively
    while (1)
    {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
}

void loop()
{
  elapsedMillis msec;

  float knob_1 = 0.9;
  float knob_2 = 0.5;
  float knob_3 = 0.5;

  // Uncomment these lines to adjust parameters with analog inputs
  knob_1 = (float)analogRead(POT1_PIN) / 1023.0;
  knob_2 = (float)analogRead(POT2_PIN) / 1023.0;
  knob_3 = (float)analogRead(POT3_PIN) / 1023.0;

  mixer1.gain(0, knob_1);
  mixer1.gain(1, 1.0 - knob_1);

  freeverb1.roomsize(knob_2);
  freeverb1.damping(knob_3);

  if (msec > 250)
  {
    msec = 0;

    Serial.print("Reverb: mix=");
    Serial.print(knob_1 * 100.0);
    Serial.print("%, roomsize=");
    Serial.print(knob_2 * 100.0);
    Serial.print("%, damping=");
    Serial.print(knob_3 * 100.0);
    Serial.print("%, CPU Usage=");
    Serial.print(freeverb1.processorUsage());
    Serial.println("%");
  }
}