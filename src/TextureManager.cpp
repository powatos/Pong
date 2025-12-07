#include "TextureManager.hpp"


const std::vector<std::vector<char>> Texture::matrix() const {

    std::vector<std::vector<char>> comp;
    int cur = 0;
    
    std::vector<char> row;
    while (c_tex[cur] != '\0') {
        
        if (c_tex[cur] == '\n'){
            comp.push_back(row);
            row.clear();
            cur++;
            continue;
        }
        
        row.push_back(c_tex[cur]);
        cur++;
    }
    
    if (!row.empty()) {
        comp.push_back(row);
    }
    
    return comp;
    
}