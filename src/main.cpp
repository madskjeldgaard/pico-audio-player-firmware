#include "Arduino.h"
#include "AudioTools.h"
#include "SD.h"

#include "AudioCodecs/CodecMP3Helix.h"
#include "AudioCodecs/CodecWAV.h"
#include "AudioLibs/AudioSourceSDFAT.h"
#include "Pins.h"

// Performance settings
constexpr auto i2s_buffer_size = 4096;
// #define DEFAULT_BUFFER_SIZE i2s_buffer_size
// #define I2S_BUFFER_COUNT 10

constexpr auto sample_rate = 44100;
constexpr auto channels = 2;
constexpr auto bits_per_sample = 16;
constexpr auto baudrate = 115200;

// SDFat
const char *audioFileFolder = "/44khz16bit";
const char *ext = "wav";

AudioSourceSDFAT source(audioFileFolder, ext);

// Audio setup
AudioInfo info(sample_rate, channels, bits_per_sample);
I2SStream i2sOutput;

WAVDecoder wavDecoder;
// MP3DecoderHelix mp3Decoder;

AudioPlayer player(source, i2sOutput, wavDecoder);

// Use the Pico's BOOTSEL button as a trigger to play the next file
auto bootselButtonState = false;
void updateBootSelButton() {
  if (bootselButtonState != BOOTSEL) {
    LOGI("Bootsel button pressed");
    const auto active = player.next();

    if (!active) {
      LOGI("No more files to play");
      player.begin();
    }

    bootselButtonState = BOOTSEL;
  }
}

void callbackPrintMetaData(MetaDataType type, const char *str, int len) {
  LOGI("==> ");
  LOGI(toStr(type));
  LOGI(": ");
  LOGI(str);
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // setup I2S output
  auto config = i2sOutput.defaultConfig(TX_MODE);

  config.copyFrom(info);
  config.buffer_size = i2s_buffer_size;

  auto result = i2sOutput.begin(config);

  if (!result) {
    LOGE("Failed to start I2S output");
    while (1)
      ;
  } else {
    LOGI("Successfully started I2S");
  }
  source.begin();
  wavDecoder.begin();
  // mp3Decoder.begin();

  LOGI("Starting audio player");

  // Start audio player
  // source.setFileFilter("EX001*");
  player.setBufferSize(i2s_buffer_size);
  player.setAudioSource(source);
  player.setMetadataCallback(callbackPrintMetaData);
  player.setAutoNext(true);
  player.setAutoFade(true);
  player.setVolume(0.75f);
  player.setPath(audioFileFolder);
  player.begin();

  bootselButtonState = BOOTSEL;
  LOGI("Player started :))))))))))))");
}

void loop() {
  player.copy();

  updateBootSelButton();
}
