#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP
#include <cmath>
#include <string>
#include <sstream>

class Vector2D {
public:
    float x;
    float y;

    Vector2D(float x, float y): x(x), y(y) {}

    operator std::string() const {
        std::stringstream s;
        s << "<" << x << ", " << y <<">";
        return s.str();
    }

    /*
     * Vector2D linear algebra operations
     */

    float norm() const {
        return sqrt(x * x + y * y);
    }

    float dot(const Vector2D &v) {
        return (x * v.x) + (y * v.y);
    }

    float cos(const Vector2D &v) {
        return dot(v) / (this->norm() * v.norm());
    }


    /*
     * Vector2D with Vector2D operations
     */

    Vector2D operator+(const Vector2D &v) {
        return Vector2D(x + v.x, y + v.y);
    }

    Vector2D& operator+=(const Vector2D &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2D operator-(const Vector2D &v) {
        return Vector2D(x - v.x, y - v.y);
    }

    Vector2D& operator-=(const Vector2D &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    bool operator==(const Vector2D &u) {
        return u.x == x && u.y == y;
    }

    /*
     * Vector2D with scalar operations
     */

    Vector2D operator*(float scalar) {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D operator/(float scalar) {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vector2D operator-(float scalar) {
        return Vector2D(x - scalar, y - scalar);
    }

    Vector2D& operator-=(float scalar) {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    Vector2D operator+(float scalar) {
        return Vector2D(x + scalar, y + scalar);
    }

    Vector2D& operator+=(float scalar) {
        x += scalar;
        y += scalar;
        return *this;
    }

};

#endif
