#include <vector>
#include "Main.h"

Main::Main() {}

void Main::StartGame()
{
    NewEngine.Render();
    NewEngine.Initilise(NewEngine.EngineWindow);
    while (!glfwWindowShouldClose(NewEngine.EngineWindow))  
    {
        NewEngine.Draw();
    }
}

void Main::EndGame()
{
    NewEngine.Destroy();   
}

int main()
{
    Main Game;
    Game.StartGame();
    Game.EndGame();
}