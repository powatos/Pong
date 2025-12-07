#pragma once

#include <vector>

class Texture
{
    const char* c_tex;
    
public:
    Texture(const char* c_tex) : c_tex(c_tex) {}; 

    const std::vector<std::vector<char>> matrix () const;

};


namespace TEXTURES{

/*

********************
********************

*/
inline const Texture PONGER{
R"(********************
********************)"
};

/*

**
**

*/
inline const Texture BALL{
R"(())"
};


}