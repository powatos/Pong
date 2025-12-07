#include "Ponger.hpp"

#include <vector>
#include "TextureManager.hpp"

int Ponger::sizex;
int Ponger::sizey;
std::vector<std::vector<char>> Ponger::texture;

void Ponger::LoadTexture() {
    texture = TEXTURES::PONGER.matrix();
    
    sizey = texture.size();
    sizex = 0;
    
    for (std::vector<char> row : texture) {
        sizex = row.size() > sizex ? row.size() : sizex;
    }
    
}