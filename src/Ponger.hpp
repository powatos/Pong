
#include <vector>

#include "Vector2D.hpp"

struct Ponger {
    Vector2D Position;
    
    float velocity;
    
    static std::vector<std::vector<char>> texture;

    static int sizex;
    static int sizey;
    
    inline Ponger(Vector2D Position) : Position(Position) {}
    inline Ponger() : Position(Vector2D()) {}
    
    static void LoadTexture();
    
};