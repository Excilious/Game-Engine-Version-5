#ifndef __AUDIO_BUFFER_HEADER__
#define __AUDIO_BUFFER_HEADER__

#include <string>
#include <vector>
#include <inttypes.h>
#include "../Dependancies/OpenAL/al.h"
#include "../Dependancies/OpenAL/alext.h"
#include "../Dependancies/OpenAL/sndfile.h"
#include "../Dependancies/OpenAL/alext.h"

class AudioBuffer
{
    public:
                                AudioBuffer(void);
        ALuint                  AddSoundEffect(const std::string FileName);
        bool                    RemoveSoundEffect(const ALuint& NewBuffer);
        void                    Destroy(void);
    private:
        std::vector<ALuint>     SoundEffectBuffer;
};

#endif