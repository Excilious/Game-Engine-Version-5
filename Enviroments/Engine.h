#ifndef __ENGINE_HEADER__
#define __ENGINE_HEADER__

#define VERSION_MINOR 3
#define VERSION_MAJOR 3
#define MSAA_SAMPLES  16

#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include <tchar.h>
#include <thread>

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"

#include "Rendering/Skybox.h"
#include "World/World.h"
#include "Common/Console.h"
#include "Audio/AudioController.h"
#include "Dynamic/Player.h"
#include "Modules/BinarySpacePartition.h"
#include "Modules/BinarySpaceCollisions.h"
#include "Rendering/Interface/Core/Frame.h"
#include "Rendering/Interface/Core/Label.h"
#include "Interpreter/VirtualMachine.h"

class Engine
{
    public:
                                    Engine(void);
        void                        SetEngineTheme(void);
        void                        DrawElements(void);
        void                        DrawHUD(void);
        void                        CalculateDeltaTime(void);
        void                        Render(void);
        void                        Release(void);
        void                        Initilise(GLFWwindow* EngineWindow);
        void                        LoadWorld(std::string WorldDirectory);
        void                        ShowMenu(void);
        void                        CheckExecutionStatus(void);
        void                        Draw(void);
        void                        Destroy(void);

        bool                        InitilisationComplete;
        bool                        ShowCloseMenu;
        double                      DeltaTime;
        double                      XPosition;
        double                      YPosition;
        GLFWwindow*                 EngineWindow;
        Label                       FPSCounter;
        Label                       PlayerHealthText;

        VirtualMachine              EngineVirtualMachine;
        BinarySpacePartition        BinarySpaceScene;
        BinarySpaceCollisions       BinaryCollision;
        Shader                      SkyboxShader;
        Shader                      BSPShader;
        AudioController             EngineAudioController;
        Parser                      EngineParser;
        Player                      EnginePlayer;
        Camera                      EngineCamera;
        Skybox                      EngineSkybox;
        World                       EngineWorld;
        Frame                       MenuFrame;
    private:
        int                         Width                   = 1920;
        int                         Height                  = 1080;
        float                       FramesPerSecond         = 0.0f;
        float                       NearView                = 0.1f;
        float                       FarView                 = 100.0f;
        float                       FieldOfView             = 90.0f;
        double                      PreviousTime            = 0.0;
        double                      CurrentTime             = 0.0;
        unsigned int                Counter                 = 0;
        bool                        StartWorld              = false;
        float                       ConsoleInputLast        = 0.0f;
        std::string                 LastWorldInstance;
};

#endif
