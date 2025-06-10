#ifndef __VIEWPORT_HEADER__
#define __VIEWPORT_HEADER__

#include <map>
#include "../Common/Parser.h"
#include "../Rendering/Camera.h"
#include "../World/Instance.h"
#include "../Audio/Audio.h"
#include "../Audio/AudioBuffer.h"
#include "../Audio/AudioSource.h"
#include "../Dynamic/Player.h"

struct WeaponInfomation
{
    int     MaxAmmo;
    int     CurrentAmmo;
    float   ReloadDelay;
    float   FireDelay;
};

class Viewport
{
    public:
                                            Viewport(void);
        void                                CreateViewport(GLFWwindow* EngineWindow,Parser* EngineParser,nlohmann::json NewJson);
        void                                RenderViewport(GLFWwindow* EngineWindow,Player* EnginePlayer,Camera* EngineCamera,float Near,float Far,float FieldOfView,float PlayerSpeed);
        void                                Destroy(void);
        float                               WeaponDelay;
        float                               FireWait;
        bool                                FireWeapon;
        int                                 WeaponIndex;
        unsigned int                        WeaponFireSound;
        unsigned int                        WeaponReloadSound;
        std::map<int,WeaponInfomation>      WeaponDictionary;
        AudioSource                         NewAudioSource;
        
    private:
        int                                 ZAxisMovement;
        glm::vec3                           PlayerOldPosition;
        Instance                            WeaponInstances;
        nlohmann::json                      ViewportJSON;

        AudioSettings                       AudioSystemSettings;
        Audio                               NewAudioSystem;
        AudioBuffer                         NewAudioBuffer;
};

#endif