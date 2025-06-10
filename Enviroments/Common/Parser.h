#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include <map>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include "Console.h"
#include "../Dependancies/jsonimport/json.h"

typedef enum
{
    DT_IMAGE,
    DT_MODEL,
    DT_SHADER_FRAGMENT,
    DT_SHADER_VERTEX,
    DT_CANVAS,
    DT_VERTEX,
    DT_INDICES,
    DT_TRUETYPE,
    DT_SCRIPT
} DataType;

typedef struct
{
    std::string     LabelName;
    std::string     DataBytes;
    std::string     DataContents;
    DataType        TypeContent;
} ParserMap;

class Parser
{
    public:
                                        Parser(void);
        void                            FetchFromDatapack(const std::string FilePath);
        unsigned char*                  GetImageBuffer(const std::string ImagePath);
        unsigned char*                  GetGlobalImageBuffer(const std::string ImageName);
        unsigned int                    QueryDataTypeContent(const std::string FileName);
        std::string                     GetGlobalDatapackData(const std::string FileName);
        std::vector<unsigned char>      GetURIData(const std::string ModelDataContents);
    private:
        std::string                     GetDatapackData(const std::string FileName);
        std::vector<ParserMap>          ParserGroup;
        nlohmann::json                  Json;
};

#endif