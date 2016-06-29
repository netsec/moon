#ifndef MOON_AUDIO_H
#define MOON_AUDIO_H

#include <soundio/soundio.h>
#define MOON_AUDIO_BACKEND "libsoundio"

namespace Moon {
  class Audio {
    public:
      enum ErrorCode {
        MOON_AUDIO_OK,
        // SoundIO failed to create
        MOON_AUDIO_CREATE_ERROR,
        // could not establish a connection to the audio server/device
        MOON_AUDIO_CONNECTION_ERROR,
        // There is no default device present
        MOON_AUDIO_NO_DEVICE,
        // The device specified by the index, has gone away or does not exist
        MOON_AUDIO_DEVICE_MISSING,
        // Could not open a stream for audio output
        MOON_AUDIO_COULD_NOT_OPEN_STREAM,
        // The specified channel layout is not supported
        MOON_AUDIO_STREAM_CHANNEL_LAYOUT_ERROR,
        // Stream could not be started
        MOON_AUDIO_STREAM_START_ERROR
      };

      static ErrorCode Initialize();
      static void Update();
      static void Terminate();
    protected:
      static bool m_initialized;
      static struct SoundIo* m_soundIO;
      static struct SoundIoDevice* m_device;
      static struct SoundIoOutStream* m_outStream;
  };
};

#endif