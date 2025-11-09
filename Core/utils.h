#pragma once

#include <iostream>
#include <cmath>

// Structură generică pentru poziție/vector
struct Vec2f {
    float x;
    float y;

    Vec2f operator+(const Vec2f& other) const { return {x + other.x, y + other.y}; }
    Vec2f operator-(const Vec2f& other) const { return {x - other.x, y - other.y}; }
    Vec2f operator*(float scalar) const { return {x * scalar, y * scalar}; }
    float magnitude() const { return std::sqrt(x*x + y*y); }
    Vec2f normalize() const { float mag = magnitude(); return {x / mag, y / mag}; }
};

inline std::ostream& operator<<(std::ostream& os, const Vec2f& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

// Structură generică pentru limite (bounding box)
struct Bounds {
    Vec2f position;
    float radius;

    bool findIntersection(const Bounds& other) const {
        float dx = position.x - other.position.x;
        float dy = position.y - other.position.y;
        float distance = std::sqrt(dx*dx + dy*dy);
        return distance < (radius + other.radius);
    }
};

// Enum pentru culori, fără legătură cu SFML
enum class Culoare {
    ROSU,
    VERDE,
    ALBASTRU,
    GALBEN,
    VIOLET,
    PORTOCALIU,
    UNKNOWN // Pentru erori sau valori implicite
};

inline std::ostream& operator<<(std::ostream& os, Culoare c) {
    switch (c) {
        case Culoare::ROSU: os << "ROSU"; break;
        case Culoare::VERDE: os << "VERDE"; break;
        case Culoare::ALBASTRU: os << "ALBASTRU"; break;
        case Culoare::GALBEN: os << "GALBEN"; break;
        case Culoare::VIOLET: os << "VIOLET"; break;
        case Culoare::PORTOCALIU: os << "PORTOCALIU"; break;
        case Culoare::UNKNOWN: os << "UNKNOWN"; break;
    }
    return os;
}

// Constante pentru fereastră
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;