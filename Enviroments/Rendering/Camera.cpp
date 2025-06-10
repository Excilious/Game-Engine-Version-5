#include "Camera.h"
#include "../Common/Console.h"

Camera::Camera(void): Position(glm::vec3(0.0f,0.0f,0.0f)) {}

void Camera::InitiliseCamera(int WindowWidth,int WindowHeight,glm::vec3 CameraPosition)
{
    this->Width         = WindowWidth;
    this->Height        = WindowHeight;
    this->Position      = CameraPosition;
}

void Camera::UpdateMatrix(float FieldOfView,float NearPlane,float FarPlane)
{
    this->ProjectionMatrix  = glm::perspective(glm::radians(FieldOfView),(float)(this->Width/this->Height),NearPlane,FarPlane);
    this->ViewMatrix        = glm::lookAt(this->Position,this->Position+this->Orientation,this->UpPosition);
    this->CameraMatrix      = ProjectionMatrix * ViewMatrix;
}

void Camera::SetMatrix(Shader* ShaderInstance,const char* Uniform)
{ShaderInstance->SetMatrix4(this->CameraMatrix,Uniform);};

void Camera::MoveCamera(void)
{
    if (ConsoleOpen) {return;}
    float RotationX;
    float RotationY;
    double MouseX;
    double MouseY;
    RotationX = this->Sensitivity * (float)(MouseY - (this->Height/2)) / this->Height;
    RotationY = this->Sensitivity * (float)(MouseX - (this->Width/2)) / this->Width;

    glm::vec3 NewOrientation = glm::rotate(this->Orientation,glm::radians(-RotationX),glm::normalize(glm::cross(this->Orientation,this->UpPosition)));

    if (!((glm::angle(NewOrientation,UpPosition) <= glm::radians(5.0f)) or (glm::angle(NewOrientation,-UpPosition) <= glm::radians(5.0f))))
    {this->Orientation  = NewOrientation;}
    this->Orientation   = glm::rotate(this->Orientation,glm::radians(-RotationY),this->UpPosition);
}

void Camera::UpdateCameraPosition(double XPosition,double YPosition)
{
    if (ConsoleOpen) {this->FirstMouse = true; return;}
    if (this->FirstMouse)
    {
        this->LastX         = XPosition;
        this->LastY         = YPosition;
        this->FirstMouse    = false;
    }

    this->XOffset   = XPosition - this->LastX;
    this->YOffset   = this->LastY - YPosition;
    this->LastX     = XPosition;
    this->LastY     = YPosition;

    this->XOffset *= this->Sensitivity;
    this->YOffset *= this->Sensitivity;
    this->RotateX += this->XOffset;
    this->RotateY += this->YOffset;

    if (this->RotateY > 89.0f)  {this->RotateY = 89.0f;}
    if (this->RotateY < -89.0f) {this->RotateY = -89.0f;}

    this->Orientation.x = cos(glm::radians(this->RotateX)) * cos(glm::radians(this->RotateY));
    this->Orientation.y = sin(glm::radians(this->RotateY));
    this->Orientation.z = sin(glm::radians(this->RotateX));
    this->Orientation = glm::normalize(this->Orientation);
}