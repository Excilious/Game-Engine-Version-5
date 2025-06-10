#ifndef __AUDIO_HEADER__
#define __AUDIO_HEADER__

#include <windows.h>
#include <iostream>
#include <fstream>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/OpenAL/alc.h"

#include "../Common/Console.h"

class Audio
{
    public:
                            Audio(void);
        void                CreateAudio(void);
        void                DestroyAudio(void);
    private:
        ALCdevice*          ALCNewDevice;
        ALCcontext*         ALCNewContext;
        const ALCchar*      NewName;
};

#endif