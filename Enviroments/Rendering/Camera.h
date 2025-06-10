#ifndef __CAMERA_HEADER__
#define __CAMERA_HEADER__
#define GLM_ENABLE_EXPERIMENTAL

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtx/rotate_vector.hpp"
#include "../Dependancies/glm/gtx/vector_angle.hpp"
#include "../Common/Shader.h"

class Camera
{
    public:
                        Camera(void);
        void            InitiliseCamera(int WindowWidth,int WindowHeight,glm::vec3 CameraPosition);
        void            UpdateMatrix(float FieldOfView,float NearPlane,float FarPlane);
        void            SetMatrix(Shader* ShaderInstance,const char* Uniform);
        void            MoveCamera(void);
        void            UpdateCameraPosition(double XPosition,double YPosition);

        glm::vec3       Position;
        glm::vec3       UpPosition        = glm::vec3(0.0f,1.0f,0.0f);
        glm::vec3       Orientation       = glm::vec3(0.0f,0.0f,-1.0f);
        glm::mat4       CameraMatrix      = glm::mat4(1.0f);
        glm::mat4       ViewMatrix        = glm::mat4(1.0f);
        glm::mat4       ProjectionMatrix  = glm::mat4(1.0f);
    private:

        float           LastX                 = 0;
        float           LastY                 = 0;
        float           RotateX               = 0;
        float           RotateY               = 0;
        float           XOffset               = 0;
        float           YOffset               = 0;
        float           Sensitivity           = 0.1f;
        bool            FirstMouse            = true;

        int             Width;
        int             Height;
};

#endif