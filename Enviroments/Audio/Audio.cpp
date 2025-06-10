#include "Audio.h"

Audio::Audio(void): NewName(nullptr) {}

void Audio::CreateAudio(void)
{
    this->ALCNewDevice  = alcOpenDevice(nullptr);
    if (!this->ALCNewDevice) {SysPrint("Failed to get audio device.",2);}

    this->ALCNewContext = alcCreateContext(this->ALCNewDevice,nullptr);
    if (!this->ALCNewContext)                           {SysPrint("Failed to load sound context.",2);}
    if (!alcMakeContextCurrent(this->ALCNewContext))    {SysPrint("Failed to make the context current.",2);}

    if (alcIsExtensionPresent(this->ALCNewDevice,"ALC_ENUMERATE_ALL_EXT")) {this->NewName = alcGetString(this->ALCNewDevice,ALC_ALL_DEVICES_SPECIFIER);}
    if (!this->NewName || alcGetError(this->ALCNewDevice) != ALC_NO_ERROR) {this->NewName = alcGetString(this->ALCNewDevice,ALC_DEVICE_SPECIFIER);}
    SysPrint("Opened "+(std::string)this->NewName,1);
}

void Audio::DestroyAudio(void)
{
    if (!alcMakeContextCurrent(nullptr)) {SysPrint("Failed to set context to null.",2);}
    alcDestroyContext(this->ALCNewContext);
    if (this->ALCNewContext)                    {SysPrint("Failed to unset during exit.",2);} 
    if (!alcCloseDevice(this->ALCNewDevice))    {SysPrint("Failed to close sound device.",2);}
}