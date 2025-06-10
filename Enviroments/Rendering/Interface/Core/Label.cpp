#include "Label.h"

Label::Label(void) {}

void Label::CreateLabel(std::string Text,glm::vec2 TextPosition,float TextScale,glm::vec3 TextColour)
{
    if (!gltInit()) {glfwTerminate();}

    this->TextContent   = Text;
    this->Scale         = TextScale;
    this->TextColour    = TextColour;
    this->TextPosition  = TextPosition;
    this->TextInstance  = gltCreateText();
    gltSetText(this->TextInstance,this->TextContent.c_str());
}

void Label::SetNewText(std::string Text)
{
    this->TextContent = Text;
    gltSetText(this->TextInstance,this->TextContent.c_str());
}

void Label::RenderText(void)
{
    gltBeginDraw();
    gltColor(this->TextColour.x,this->TextColour.y,this->TextColour.z,1.0f);
    gltDrawText2D(this->TextInstance,this->TextPosition.x,this->TextPosition.y,this->Scale);
}

void Label::DestroyText(void)
{
    gltDeleteText(this->TextInstance);
    gltTerminate();
}