#pragma once

constexpr double epsilon = 0.0001;
constexpr double pi      = 3.1415927;

// A pair of continuous Cartesian coordinates on a Euclidean plane
struct Vector2D
{
    Vector2D() : mX(0), mY(0) {}
    Vector2D(double x, double y) : mX(x), mY(y) {}

    // The usual stuff
    double length() const;
    Vector2D normalized() const;
    Vector2D rounded() const; // Returns the closest point with integer coordinates

    bool operator==(const Vector2D& other) const;
    bool operator!=(const Vector2D& other) const;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;

    double operator*(const Vector2D& other) const;

    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);

    Vector2D operator*(double scale) const;
    Vector2D operator/(double scale) const;

    Vector2D& operator*=(double scale);
    Vector2D& operator/=(double scale);

    // Rotate in anticlockwise direction. Attention: you're passing degrees!
    Vector2D& rotate(double degrees);

    double mX, mY;
};
