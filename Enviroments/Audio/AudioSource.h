#ifndef __AUDIO_SOURCE_HEADER__
#define __AUDIO_SOURCE_HEADER__

#include <vector>
#include "../../Dependancies/glm/glm.hpp"
#include "../../Dependancies/OpenAL/al.h"

struct AudioSettings
{
    float Pitch         = 1.0f;
    float Gain          = 1.0f;
    bool LoopSound      = false;
    glm::vec3 Position  = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 Velocity  = glm::vec3(0.0f,0.0f,0.0f);
    ALuint Buffer       = 0;
    ALuint Source       = 0;
};

class AudioSource
{
    public:
                                        AudioSource(void);
        int                             GetAudioSettings(const ALuint SearchBuffer);
        void                            Create(AudioSettings *NewAudioSettings);
        void                            Play(const ALuint BufferToPlay);
        void                            Destroy(void);
    private:
        std::vector<AudioSettings>      SavedAudioBuffers;
};

#endif