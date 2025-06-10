#ifndef __AUDIO_CONTROLLER_HEADER__
#define __AUDIO_CONTROLLER_HEADER__

#include "Audio.h"
#include "AudioBuffer.h"
#include "AudioSource.h"

struct AudioGroup
{
    Audio           NewAudio;
    AudioBuffer     NewAudioBuffer;
    AudioSource     NewAudioSource;
    std::string     NewAudioID;
    unsigned int    CreatedBuffer;
};

class AudioController
{
    public:
                                    AudioController(void);
        int                         GetAudioFromString(const std::string NewSound);
        void                        AddSoundToLibrary(const std::string NewSound,AudioSettings NewAudioSettings);
        void                        Play(const std::string NewSound);
        std::vector<AudioGroup>     SoundLibrary = {};
    private:
};

#endif