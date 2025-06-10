#include "AudioSource.h"
#include "../Common/Console.h"

AudioSource::AudioSource(void) {}

int AudioSource::GetAudioSettings(const ALuint SearchBuffer)
{
    for (int Index = 0; Index < this->SavedAudioBuffers.size(); Index++)
    {
        if (this->SavedAudioBuffers[Index].Buffer == SearchBuffer)
        {return Index;}
    }
    SysPrint("Cannot find within SavedSoundSettings!",2);
    return -1;
}

void AudioSource::Create(AudioSettings *NewAudioSettings)
{
    alGenSources(1,&NewAudioSettings->Source);
    alSourcef(NewAudioSettings->Source,AL_PITCH,NewAudioSettings->Pitch);
    alSourcef(NewAudioSettings->Source,AL_GAIN,NewAudioSettings->Gain);
    alSource3f(NewAudioSettings->Source,AL_POSITION,NewAudioSettings->Position.x,NewAudioSettings->Position.y,NewAudioSettings->Position.z);
    alSource3f(NewAudioSettings->Source,AL_VELOCITY,NewAudioSettings->Velocity.x,NewAudioSettings->Velocity.y,NewAudioSettings->Velocity.z);
    alSourcei(NewAudioSettings->Source,AL_LOOPING,NewAudioSettings->LoopSound);
    alSourcef(NewAudioSettings->Source,AL_BUFFER,NewAudioSettings->Buffer);
    this->SavedAudioBuffers.push_back(*NewAudioSettings);
}

void AudioSource::Play(const ALuint BufferToPlay)
{
    int AudioIndex                  = this->GetAudioSettings(BufferToPlay);
    if (AudioIndex == -1)           {SysPrint("Cannot play audio!",3); return;}
    AudioSettings CurrentSettings   = this->SavedAudioBuffers[AudioIndex];
    alSourcei(CurrentSettings.Source,AL_BUFFER,(ALint)CurrentSettings.Buffer);
    alSourcePlay(CurrentSettings.Source);
}

void AudioSource::Destroy(void)
{
    for (int Index = 0; Index < this->SavedAudioBuffers.size(); Index++)
    {alDeleteSources(1,&this->SavedAudioBuffers[Index].Source);}
}