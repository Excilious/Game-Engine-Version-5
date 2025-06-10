#include "Engine.h"

Engine::Engine(void): InitilisationComplete(false), ShowCloseMenu(false)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,MSAA_SAMPLES);

    //glfwGetPrimaryMonitor()
    GLFWwindow* EngineWindow = glfwCreateWindow(this->Width,this->Height,"Fusion",NULL,NULL);
    glfwMakeContextCurrent(EngineWindow);
    gladLoadGL();
    glViewport(0,0,this->Width,this->Height);

    UpdateVariables("");
    glCullFace(GL_FRONT);
    glfwSwapInterval(1);
    glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    if (!glfwInit())                                            {MessageBox(NULL,TEXT("Failed to load GLFW window."),TEXT("Fusion Engine"),MB_ICONERROR|MB_OK); glfwTerminate();}
    if (EngineWindow == NULL)                                   {glfwTerminate();} 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    {glfwTerminate();}
    this->EngineWindow = EngineWindow;
}

void OnWindowResizedCallback(GLFWwindow* EngineWindow,int Width,int Height)     {glViewport(0,0,Width,Height);}
void Engine::Release(void)                                                      {if (std::get<bool>(GetFromRegister("exit"))) {this->ShowCloseMenu = true;}}

void Engine::SetEngineTheme(void)
{
    ImGuiStyle &NewStyle    = ImGui::GetStyle();
    ImVec4 *Colours         = NewStyle.Colors;

    Colours[ImGuiCol_WindowBg]              = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
    Colours[ImGuiCol_MenuBarBg]             = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    Colours[ImGuiCol_PopupBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    Colours[ImGuiCol_Header]                = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    Colours[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    Colours[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    Colours[ImGuiCol_Button]                = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
    Colours[ImGuiCol_ButtonHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    Colours[ImGuiCol_ButtonActive]          = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    Colours[ImGuiCol_FrameBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    Colours[ImGuiCol_FrameBgHovered]        = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    Colours[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    Colours[ImGuiCol_Tab]                   = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    Colours[ImGuiCol_TabHovered]            = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    Colours[ImGuiCol_TabActive]             = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    Colours[ImGuiCol_TabUnfocused]          = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    Colours[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    Colours[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    Colours[ImGuiCol_TitleBgActive]         = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    Colours[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    Colours[ImGuiCol_Border]                = ImVec4(0.15f, 0.15f, 0.15f, 0.50f);
    Colours[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    Colours[ImGuiCol_Text]                  = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    Colours[ImGuiCol_TextDisabled]          = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    Colours[ImGuiCol_CheckMark]             = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    Colours[ImGuiCol_SliderGrab]            = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    Colours[ImGuiCol_SliderGrabActive]      = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    Colours[ImGuiCol_ResizeGrip]            = ImVec4(0.65f, 0.65f, 0.65f, 0.50f);
    Colours[ImGuiCol_ResizeGripHovered]     = ImVec4(0.55f, 0.55f, 0.55f, 0.75f);
    Colours[ImGuiCol_ResizeGripActive]      = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    Colours[ImGuiCol_ScrollbarBg]           = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    Colours[ImGuiCol_ScrollbarGrab]         = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    Colours[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    Colours[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);

    NewStyle.WindowRounding     = 5.0f;
    NewStyle.FrameRounding      = 5.0f;
    NewStyle.GrabRounding       = 5.0f;
    NewStyle.TabRounding        = 5.0f;
    NewStyle.PopupRounding      = 5.0f;
    NewStyle.ScrollbarRounding  = 5.0f;
    NewStyle.WindowPadding      = ImVec2(10, 10);
    NewStyle.FramePadding       = ImVec2(6, 4);
    NewStyle.ItemSpacing        = ImVec2(8, 6);
    NewStyle.PopupBorderSize    = 0.f;
}

void Engine::CalculateDeltaTime(void)
{
    this->CurrentTime   = glfwGetTime();
    this->DeltaTime     = this->CurrentTime - this->PreviousTime;
    this->Counter++;
    if (this->DeltaTime >= 1.0f/30.0f) {this->FramesPerSecond = (1.0f/this->DeltaTime) * this->Counter; this->Counter = 0; this->PreviousTime = this->CurrentTime;}
}

void Engine::LoadWorld(std::string WorldDirectory)
{
    SetFromRegister("world",WorldDirectory);
    this->LastWorldInstance = WorldDirectory;
    std::ifstream BaseFile(std::get<std::string>(GetFromRegister("world")));
    if (!BaseFile.good())
    {
        SysPrint("Cannot find "+WorldDirectory+"! Exiting Fusion...",3);
        MessageBox(NULL,TEXT("Cannot find wrdx file! Ensure that the file is within the correct directory."),TEXT("Fusion Engine"),MB_ICONERROR|MB_OK);
        this->Destroy();
    }
    else {BaseFile.close();}
    this->EngineParser.FetchFromDatapack(std::get<std::string>(GetFromRegister("world")));
}

void Engine::Initilise(GLFWwindow* EngineWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &InterfaceInputOutput = ImGui::GetIO();
    ImGuiStyle& NewStyle = ImGui::GetStyle();
    ImGui_ImplGlfw_InitForOpenGL(EngineWindow,true);
    ImGui_ImplOpenGL3_Init();
    CommandGenerateValues();;

    this->Width     = std::get<float>(GetFromRegister("width"));
    this->Height    = std::get<float>(GetFromRegister("height")); 
    this->EngineAudioController.AddSoundToLibrary("../../Fusion/Audio/Evil.wav",{1.0f,1.0f,false,glm::vec3(0.0f),glm::vec3(0.0f),0,0});
    this->EngineAudioController.Play("../../Fusion/Audio/Evil.wav");
    
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (std::get<std::string>(GetFromRegister("world")) != "-1")
    {
        this->EngineCamera.InitiliseCamera(this->Width,this->Height,glm::vec3(0.0f,0.0f,0.0f));
        this->EngineSkybox.InitiliseSkybox(this->Width,this->Height,this->NearView,this->FarView,&this->EngineParser,this->FieldOfView);
        this->SkyboxShader.CreateNewShader(
            this->EngineParser.GetGlobalDatapackData("Skybox.vert").c_str(),
            this->EngineParser.GetGlobalDatapackData("Skybox.frag").c_str(),
            true
        );
        this->BSPShader.CreateNewShader(
            this->EngineParser.GetGlobalDatapackData("BSP.vert").c_str(),
            this->EngineParser.GetGlobalDatapackData("BSP.frag").c_str(),
            true
        );
      
        this->BinarySpaceScene.LoadBinarySpacePartition(std::get<std::string>(GetFromRegister("bsp")),&this->BSPShader);
        this->BinaryCollision.GetCollisionData(std::get<std::string>(GetFromRegister("bsp")));
        this->BinarySpaceScene.SetupBinarySpacePartition(&this->EngineParser);
        this->EnginePlayer.LoadPlayer(&this->EngineParser,&this->EngineCamera);
        this->EngineWorld.GenerateWorld(this->EngineWindow,&this->EngineParser,this->NearView,this->FarView);
        this->FPSCounter.CreateLabel("-1",glm::vec2(0.0f,0.0f),1.25f,glm::vec3(1.0f));
        this->PlayerHealthText.CreateLabel("-1",glm::vec2(0.0f,18.0f),1.25f,glm::vec3(1.0f));

        this->MenuFrame.CreateFrame(glm::vec2(0.0f,0.0f),glm::vec2(Width*4,Height*4),Width,Height,glm::vec3(0.0f,0.0f,0.0f),0.0f,this->EngineParser.GetGlobalDatapackData("Frame.vert"),this->EngineParser.GetGlobalDatapackData("Frame.frag"));
        this->MenuFrame.AddTransparancy(0.75f);

        glfwSetWindowSizeCallback(this->EngineWindow,OnWindowResizedCallback);
        this->SkyboxShader.ActivateShader();
        this->SetEngineTheme();
        this->InitilisationComplete             = true;
        this->EnginePlayer.PlayerPosition       = this->BinarySpaceScene.SpawnPlayerInBSP();
        this->EngineCamera.Position             = this->EnginePlayer.PlayerPosition;
        this->EnginePlayer.PlayerPosition.y     += 2.0f;
    }
}

void Engine::ShowMenu(void)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    if (ConsoleOpen || std::get<bool>(GetFromRegister("exit")))                     {this->MenuFrame.RenderInstance();}
    if (std::get<bool>(GetFromRegister("showfps")))                                 {this->FPSCounter.SetNewText(std::to_string(this->FramesPerSecond)); this->FPSCounter.RenderText();}
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Engine::DrawHUD(void)
{ 
    bool HudBool    = std::get<bool>(GetFromRegister("showhud"));
    ImVec2 Center   = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(ImVec2(Center.x,(Center.y+ImGui::GetMainViewport()->WorkSize.y/2)-10),ImGuiCond_Appearing,ImVec2(0.5f, 0.5f));
    ImGui::Begin("BasicHUD",&(HudBool),ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove);
    ImGui::Text(std::to_string((int)this->EnginePlayer.LocalPlayerAttribute.PlayerHealth).c_str());
    ImGui::End();
}

void Engine::CheckExecutionStatus(void)
{
    if (std::string(ConsoleInputBuffer).find("+exec") != std::string::npos && (glfwGetKey(this->EngineWindow,GLFW_KEY_ENTER) == GLFW_PRESS))
    {
        std::string         NewString = std::string(ConsoleInputBuffer);
        std::istringstream  StringStream(NewString);
        std::string         Key,Value;
        StringStream >> Key >> Value; 
        ConsoleInputBuffer[0] = '\0';
        this->EngineVirtualMachine.Execute(Value,&this->EngineParser);
    }
}

void Engine::DrawElements(void)
{
    if (ConsoleOpen || std::get<bool>(GetFromRegister("exit"))) {glfwSetInputMode(this->EngineWindow,GLFW_CURSOR,GLFW_CURSOR_NORMAL);}
    else                                                        {glfwSetInputMode(this->EngineWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);}
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (ConsoleOpen) {CommandRenderConsoleFrame(this->EngineWindow); this->CheckExecutionStatus(); CommandShowExitMessage();}
    this->DrawHUD();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    float ConsoleCurrentInput = glfwGetTime();
    if (glfwGetKey(this->EngineWindow,GLFW_KEY_ESCAPE) == GLFW_PRESS && (ConsoleCurrentInput-this->ConsoleInputLast) > 0.25f) 
    {if (std::get<bool>(GetFromRegister("exit"))) {ConsoleOpen = true;} else {ConsoleOpen = !ConsoleOpen;} this->ConsoleInputLast = ConsoleCurrentInput;}

    if (std::get<std::string>(GetFromRegister("world")) != "-1" && this->InitilisationComplete) 
    {
        this->EngineCamera.UpdateMatrix(this->FieldOfView,this->NearView,this->FarView);
        this->EngineCamera.UpdateCameraPosition(this->XPosition,this->YPosition);
        this->EngineSkybox.RenderSkybox(this->EngineCamera.Position,this->EngineCamera.Orientation,this->EngineCamera.UpPosition,&this->SkyboxShader);
        //this->EnginePlayer.Movement(this->EngineWindow,this->EngineCamera.Orientation,this->EngineCamera.Position,&this->BinaryCollision,&this->BinarySpaceScene,this->DeltaTime);
        this->BinarySpaceScene.RenderAllFaces(&this->BSPShader,this->EngineCamera.Position,this->EngineCamera.ViewMatrix,this->EngineCamera.ProjectionMatrix);
        this->EngineWorld.RenderWorld(this->EngineWindow,&this->EnginePlayer,&this->EngineCamera,this->NearView,this->FarView,this->FieldOfView,this->EngineSkybox.CubemapTexture,this->DeltaTime,this->EnginePlayer.PlayerSpeed);
        
        if (this->EngineCamera.Position != this->EnginePlayer.PlayerPosition)
        {
            BoundingBoxActor NewBounds          = this->EnginePlayer.GetMinimumMaximum();
            this->EnginePlayer.ModelPosition    = this->EngineCamera.Position;
            this->EnginePlayer.UpdateActorCollider(&this->BinaryCollision,NewBounds.Minimum,NewBounds.Maximum,this->EnginePlayer.PlayerPosition,this->DeltaTime);
            this->EngineCamera.Position         = this->EnginePlayer.ModelPosition;
            this->EnginePlayer.PlayerPosition   = this->EnginePlayer.ModelPosition;
        }
    }

    if (this->Width != std::get<float>(GetFromRegister("width")) || this->Height != std::get<float>(GetFromRegister("height")))
    {
        this->Width     = std::get<float>(GetFromRegister("width"));
        this->Height    = std::get<float>(GetFromRegister("height"));
        OnWindowResizedCallback(this->EngineWindow,this->Width,this->Height);
    }

    if (this->LastWorldInstance != std::get<std::string>(GetFromRegister("world")))
    {
        this->LastWorldInstance = std::get<std::string>(GetFromRegister("world"));
        this->LoadWorld(std::get<std::string>(GetFromRegister("world")));
    }

    if (std::get<bool>(GetFromRegister("exitconfirm"))) {this->Destroy();}

    this->EnginePlayer.PlayerFriction       = std::get<float>(GetFromRegister("frict"));
    this->EnginePlayer.PlayerSpeed          = std::get<float>(GetFromRegister("speed"));
    this->EnginePlayer.PlayerAcceleration   = std::get<float>(GetFromRegister("accel"));
    this->EnginePlayer.PlayerDeceleration   = std::get<float>(GetFromRegister("decel"));
    this->FieldOfView                       = std::get<float>(GetFromRegister("fov"));
    this->NearView                          = std::get<float>(GetFromRegister("near"));
    this->FarView                           = std::get<float>(GetFromRegister("far"));

    this->ShowMenu();
    this->DrawElements();
    this->Release();
    this->CalculateDeltaTime();
    glfwGetCursorPos(this->EngineWindow,&XPosition,&YPosition);
    glfwSwapBuffers(this->EngineWindow);
    glfwPollEvents();
}

void Engine::Render(void)
{
    this->LoadWorld("../../Fusion/Base.zip");

    SysPrint("This application is in beta!",2);
    SysPrint("Fusion Engine 5, GL Version "+std::to_string(VERSION_MAJOR)+"|"+std::to_string(VERSION_MINOR),1);
    SysPrint("Resolution: "+std::to_string(this->Width)+"x"+std::to_string(this->Height),1);
}

void Engine::Destroy(void)
{
    this->BinarySpaceScene.DestroyScene();
    this->SkyboxShader.DestroyShader();
    this->EngineWorld.DestroyWorld();
    this->EnginePlayer.Destroy();
    this->MenuFrame.DestroyInstance();

    glfwDestroyWindow(this->EngineWindow);
    glfwTerminate();
    exit(0);
}