#include "AudioController.h"

AudioController::AudioController(void) {};

int AudioController::GetAudioFromString(const std::string NewSound)
{
    for (int Index = 0; Index < this->SoundLibrary.size(); Index++)
    {
        if (this->SoundLibrary[Index].NewAudioID == NewSound) {return Index;}
    }
    return -1;
}

void AudioController::AddSoundToLibrary(const std::string NewSound,AudioSettings NewAudioSettings)
{
    if (this->GetAudioFromString(NewSound) != -1) {SysPrint("Sound with ID: "+NewSound+" already exists!",2); return;}
    AudioGroup NewAudioGroup;
    NewAudioGroup.NewAudio.CreateAudio();
    NewAudioGroup.NewAudioID    = NewSound;
    NewAudioGroup.CreatedBuffer = NewAudioGroup.NewAudioBuffer.AddSoundEffect(NewSound);
    NewAudioSettings.Buffer     = NewAudioGroup.CreatedBuffer;
    NewAudioGroup.NewAudioSource.Create(&NewAudioSettings); 
    if (NewAudioSettings.Buffer != -1) {this->SoundLibrary.push_back(NewAudioGroup);}
}

void AudioController::Play(const std::string NewSound)
{
    int SoundIndex = this->GetAudioFromString(NewSound);
    if (SoundIndex == -1) {SysPrint("Sound with ID "+NewSound+" does not exist yet!",2); return;}

    this->SoundLibrary[SoundIndex].NewAudioSource.Play(this->SoundLibrary[SoundIndex].CreatedBuffer);
}

