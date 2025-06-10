#ifndef __SHADER_HEADER_FILE__
#define __SHADER_HEADER_FILE__
#define GLM_ENABLE_EXPERIMENTAL

#define VERTEX_REPORT_ID        0x000001
#define FRAGMENT_REPORT_ID      0x000002
#define PROGRAM_REPORT_ID       0x000003

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/gtx/vector_angle.hpp"

std::string GetShaderContents(const char* ShaderName);
void ReportErrors(unsigned int Shaders,int Type);

class Shader
{
    public:
                Shader(void);
        void    CreateNewShader(const char* VertexShaderFile,const char* FragmentShaderFile,bool Source);
        void    SetVector4(const glm::vec4 Vector,const std::string Location);
        void    SetVector3(const glm::vec3 Vector,const std::string Location);
        void    SetVector2(const glm::vec2 Vector,const std::string Location);
        void    SetVector1(const float Vector,const std::string Location);
        void    SetMatrix4(const glm::mat4 Matrix,const std::string Location);
        void    ActivateShader(void)    const;
        void    DestroyShader(void)     const;
        GLuint  Program;
};


#endif