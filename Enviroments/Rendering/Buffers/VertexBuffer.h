#ifndef __VERTEX_BUFFER_HEADER__
#define __VERTEX_BUFFER_HEADER__

#include <vector>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"

struct SceneVertex
{
    glm::vec3 Positions;
    glm::vec3 Normals;
    glm::vec3 Colour;
    glm::vec2 TextureUV;
};

class VertexBufferObject
{
    public:
                    VertexBufferObject(std::vector<SceneVertex>& Vertices);
                    VertexBufferObject(std::vector<float>& Vertices);
                    VertexBufferObject(std::vector<glm::vec3>& Vertices);
        void        Bind(void);
        void        Unbind(void);
        void        Delete(void);
        GLuint      VertexBuffer;
};

#endif
