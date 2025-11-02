#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <algorithm>
#include <cmath>
#include <format>
#include <string>

class Vector2 {
    public:
    Vector2() : x(0.0f), y(0.0f) {
    }
    Vector2(float x, float y) : x(x), y(y) {
    }

    operator std::string() const {
        return std::format("<{}, {}>", x, y);
    }

    /*
     * Vector2 linear algebra operations
     */

    float norm() const {
        return std::sqrt(x * x + y * y);
    }

    float dot(const Vector2& v) {
        return (x * v.x) + (y * v.y);
    }

    float cos(const Vector2& v) {
        return dot(v) / (this->norm() * v.norm());
    }

    float distance(const Vector2& v) {
        float d_x = x - v.x;
        float d_y = y - v.y;
        return std::sqrt(d_x * d_x + d_y * d_y);
    }

    float max() {
        return std::max(x, y);
    }

    /*
     * Vector2D with Vector2D operations
     */

    Vector2 operator+(const Vector2& v) {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2& operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2 operator-(const Vector2& v) {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2& operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    bool operator==(const Vector2& u) const  = default;
    auto operator<=>(const Vector2& v) const = default;

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

    float get_x() const {
        return x;
    }
    float get_y() const {
        return y;
    }

    void set_x(float x) {
        this->x = x;
    }
    void set_y(float y) {
        this->y = y;
    }

    private:
    float x;
    float y;
};

#endif // Vector2
