#ifndef __MAIN_HEADER__
#define __MAIN_HEADER__

#include <thread>
#include "Enviroments/Engine.h"
//#include "Enviroments/Physics/Physics.h"

class Main
{
    public:
        Main();
        void StartGame();
        void EndGame();
        Engine          NewEngine;
        //Physics         NewPhysics;
    private:
        std::thread     NewThread;
};

#endif