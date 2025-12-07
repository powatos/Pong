#include "Ball.hpp"

#include <vector>
#include <random>

#include "TextureManager.hpp"

int Ball::sizex;
int Ball::sizey;
std::vector<std::vector<char>> Ball::texture;

void Ball::LoadTexture() {
    texture = TEXTURES::BALL.matrix();
    
    sizey = texture.size();
    sizex = 0;
    
    for (std::vector<char> row : texture) {
        sizex = row.size() > sizex ? row.size() : sizex;
    }
    
    
}

void Ball::randomizeVelocity(float lower, float upper) {
    static std::mt19937 gen(std::random_device{}());

	std::uniform_real_distribution<float> dis{lower, upper};

    velocity.x = dis(gen);
    velocity.y = dis(gen);
    
    velocity = velocity.Normalized() * speed;

}