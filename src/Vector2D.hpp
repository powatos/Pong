#pragma once

#include <cmath>
#include <string>

struct Vector2D {
    float x;
    float y;
    
    inline Vector2D(float x, float y) : x(x), y(y) {}
    constexpr Vector2D() : x(0.f), y(0.f) {}
    
    inline static float Dot(Vector2D v1, Vector2D v2) {
        return (v1.x * v2.x) + (v1.y * v2.y);
    }
    
    inline Vector2D operator + (const Vector2D& other) const {
        return Vector2D{x + other.x, y + other.y};
    }
    
    inline Vector2D operator - (const Vector2D& other) const {
        return Vector2D{x - other.x, y - other.y};
    }
    
    inline Vector2D operator * (const float scale) const {
        return Vector2D{x * scale, y * scale};
    }
    
    inline Vector2D operator / (const float scale) const {
        return Vector2D{x / scale, y / scale};
    }
    
    inline Vector2D operator - () const {
        return Vector2D{-x, -y};
    }
    
    inline float getMagnitude() const {
        return std::sqrt(x*x + y*y);
    }
    
    inline Vector2D Normalized() const {
        float mag = getMagnitude();
        return mag == 0.f ? Vector2D{0.f, 0.f} : Vector2D{x / mag, y / mag};
    }
    
    inline std::string ToString() const {
        return '(' + std::to_string(x) + ',' + std::to_string(y) + ')';
    }

    
};


inline Vector2D operator * (float scale, const Vector2D& v1) {
    return v1 * scale;
}



