#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <string>
#include <sstream>

class Vector2 {
public:
    float x;
    float y;

    Vector2(): x(0.0f), y(0.0f) {}
    Vector2(float x, float y): x(x), y(y) {}

    operator std::string() const {
        std::stringstream s;
        s << "<" << x << ", " << y <<">";
        return s.str();
    }

    /*
     * Vector2 linear algebra operations
     */

    float norm() const {
        return std::sqrt(x * x + y * y);
    }

    float dot(const Vector2 &v) {
        return (x * v.x) + (y * v.y);
    }

    float cos(const Vector2 &v) {
        return dot(v) / (this->norm() * v.norm());
    }

    float distance(const Vector2 &v) {
        float d_x = x - v.x;
        float d_y = y - v.y;
        return std::sqrt(d_x * d_x + d_y * d_y);
    }

    /*
     * Vector2D with Vector2D operations
     */

    Vector2 operator+(const Vector2 &v) {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2& operator+=(const Vector2 &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2 operator-(const Vector2 &v) {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2& operator-=(const Vector2 &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    bool operator==(const Vector2 &u) {
        return u.x == x && u.y == y;
    }

    /*
     * Vector2D with scalar operations
     */

    Vector2 operator*(float scalar) {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2 operator/(float scalar) {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vector2 operator-(float scalar) {
        return Vector2(x - scalar, y - scalar);
    }

    Vector2& operator-=(float scalar) {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    Vector2 operator+(float scalar) {
        return Vector2(x + scalar, y + scalar);
    }

    Vector2& operator+=(float scalar) {
        x += scalar;
        y += scalar;
        return *this;
    }

};

#endif // Vector2
