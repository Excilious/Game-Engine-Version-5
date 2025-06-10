#ifndef __TERRAIN_HEADER__
#define __TERRAIN_HEADER__

#define TERRAIN_MAP 256

#include <vector>
#include <cmath>
#include "../../Common/Parser.h"
#include "../../Common/Shader.h"
#include "../../Rendering/Camera.h"
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/stb/stb_perlin.h"
#include "../../Rendering/Texture.h"

struct TerrainVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 UV;
};

class Terrain
{
    public:
                                        Terrain(void);
        glm::vec3                       GetNextPlayerPosition(glm::vec3 PlayerPosition);
        void                            GenerateTerrainMesh(Parser* EngineParser);
        void                            GenerateHeightMap(void);
        void                            GenerateTerrainBuffers(void);
        void                            RenderTerrain(Camera* EngineCamera);
        void                            Destroy(void);

    private:
        float                           CacheLastPosition;
        float                           Scale               = 0.05f;
        float                           HeightMultiplier    = 15.0f;
        float                           Persistance         = 0.5f;
        float                           Lacunarity          = 2.0f;
        float                           NoiseValue          = 0.0f;
        float                           Amplitude           = 1.0f;
        float                           Frequency           = 1.0f;
        int                             Octaves             = 4;

        glm::mat4                       ModelMatrix;
        float                           TerrainHeightMap[TERRAIN_MAP][TERRAIN_MAP];
        std::vector<TerrainVertex>      TerrainVertices;
        std::vector<unsigned int>       TerrainIndices;

        Texture                         TerrainTexture;
        Shader                          TerrainShader;
        GLuint                          VertexArrayObject;
        GLuint                          VertexBufferObject;
        GLuint                          ElementBufferObject;
};


#endif