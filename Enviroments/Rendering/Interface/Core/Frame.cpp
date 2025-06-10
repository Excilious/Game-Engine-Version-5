#include "Frame.h"

Frame::Frame(): OverrideFrameRendering(false) {}

glm::vec3 Frame::NormaliseColourValues(glm::vec3 ColourValue) 
{return glm::vec3(ColourValue.x/255.0f,ColourValue.y/255.0f,ColourValue.z/255.0f);}

void Frame::CreateFrame(glm::vec2 Position,glm::vec2 Scale,int Width,int Height,glm::vec3 Colour,float Rotation,std::string FrameVertex,std::string FrameFragment)
{
    float Vertices[] = { 
        -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,   0.0f, 0.0f, 
        -0.5f, -0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f,  0.5f,   0.0f, 0.0f, 
         0.5f, -0.5f,   0.0f, 0.0f  
    };

    this->Rotation              = Rotation;
    this->Scale                 = Scale;
    this->Position              = Position;
    this->WindowWidth           = Width;
    this->WindowHeight          = Height;
    this->CornerActive          = false;
    this->Colour                = this->NormaliseColourValues(Colour);
    this->TransparencyActive    = 0;
    this->NewShader.SetVector1(0,"IsImage");
    this->NewShader.SetVector1(0,"Corner");
    this->NewShader.CreateNewShader(FrameVertex.c_str(), FrameFragment.c_str(), true);

    this->FrameVertexArray.CreateVertexArray();
    this->FrameVertexArray.Bind();

    glGenBuffers(1, &this->FrameVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->FrameVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->FrameVertexArray.Unbind();
}

void Frame::AddTransparancy(float Transparency)
{
    this->TransparencyActive    = 1;
    this->Transparency          = Transparency;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void Frame::AddCorners(float Radius)
{
    this->CornerActive  = true;
    this->Radius        = Radius;
}

void Frame::RenderInstance(void)
{
    this->ModelMatrix       = glm::mat4(1.0f);
    this->ModelMatrix       = glm::translate(this->ModelMatrix, glm::vec3(this->Position, 0.0f));
    this->ModelMatrix       = glm::rotate(this->ModelMatrix, glm::radians(this->Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    this->ModelMatrix       = glm::scale(this->ModelMatrix, glm::vec3(this->Scale, 1.0f));
    this->ProjectionMatrix  = glm::ortho(0.0f, (float)this->WindowWidth, (float)this->WindowHeight, 0.0f, -1.0f, 1.0f);

    this->NewShader.ActivateShader();
    this->NewShader.SetMatrix4(this->ModelMatrix,"model");
    this->NewShader.SetMatrix4(this->ProjectionMatrix,"projection");
    this->NewShader.SetVector3(this->Colour,"spriteColor");

    if (this->CornerActive)
    {
        this->NewShader.SetVector2(this->Position,"u_position");
        this->NewShader.SetVector2(this->Scale,"u_size");
        this->NewShader.SetVector1(this->Radius,"u_radius");
        this->NewShader.SetVector1(1.0f,"Corner");
        this->NewShader.SetVector2(glm::vec2(this->WindowWidth,this->WindowHeight),"u_resolution");
    }
    
    if (this->OverrideFrameRendering) {this->NewShader.SetVector1(1,"IsImage");}
    if (this->TransparencyActive == 1)
    {
        this->NewShader.SetVector1((float)this->TransparencyActive,"IsTransparancy");
        this->NewShader.SetVector1(this->Transparency,"Transparency");
    }

    this->FrameVertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->FrameVertexArray.Unbind();
}

void Frame::DestroyInstance(void)
{
    this->FrameVertexArray.Delete();
    this->NewShader.DestroyShader();
}
