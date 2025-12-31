#pragma once
#include <cmath>
#include <iostream>

template <typename T>
struct Vector2D {
    T x, y;

    Vector2D() : x(0), y(0) {}
    Vector2D(T x, T y) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
    Vector2D operator*(T scalar) const { return Vector2D(x * scalar, y * scalar); }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    float magnitude() const {
        return std::sqrt(static_cast<float>(x * x + y * y));
    }

    Vector2D normalize() const {
        float mag = magnitude();
        if (mag == 0) return Vector2D(0, 0);
        return Vector2D(x / mag, y / mag);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2D<T>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

using Vec2f = Vector2D<float>;

struct Bounds {
    Vec2f center;
    float radius;

    bool findIntersection(const Bounds& other) const {
        float dist = (center - other.center).magnitude();
        return dist < (radius + other.radius);
    }
};

enum class Culoare { ROSU, VERDE, ALBASTRU, GALBEN, VIOLET, PORTOCALIU, UNKNOWN };

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;