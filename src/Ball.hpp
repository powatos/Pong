
#include <vector>

#include "Vector2D.hpp"

const Vector2D initialVelocity(10.f, 0.f);

struct Ball {
    Vector2D Position;
    Vector2D velocity;
    float speed;
    
    static std::vector<std::vector<char>> texture;

    static int sizex;
    static int sizey;
    
    void randomizeVelocity(float lower, float upper);
    
    inline Ball(Vector2D Position, float speed) : Position(Position), speed(speed) {
        randomizeVelocity(-1.f, 1.f);
    }
    inline Ball() : Position(Vector2D()) {
        randomizeVelocity(-1.f, 1.f);
    }
    
    static void LoadTexture();

    
};