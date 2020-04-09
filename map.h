#pragma once

#include <istream>
#include <vector>

// A pair of continuous Cartesian coordinates on a Euclidean plane
struct Vector2D
{
    Vector2D() : mX(0), mY(0) {}
    Vector2D(double x, double y) : mX(x), mY(y) {}

    double length() const;
    Vector2D normalized() const;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;

    double operator*(const Vector2D& other) const;

    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);

    Vector2D operator*(double scale) const;
    Vector2D operator/(double scale) const;

    Vector2D& operator*=(double scale);
    Vector2D& operator/=(double scale);

    double mX, mY;
};

// Types of map surfaces
// A single map square has a single surface on it
enum class Surface
{
    Track, // <- This is the default
    Gravel,
    Wall,
    FinishLine // Note that a FinishLine Square is implicitly taken to be a Track Square as well
};

// A 1x1 square shaped element of the map, centered around (mX, mY)
struct Square : public Vector2D
{
    Square(double x, double y, Surface surface) : Vector2D{x, y}, mSurface(surface) {}

    const Surface mSurface;
};

class Car;

struct Map
{
    Map(const std::vector<Square>& squares) : mSquares(squares) {}
	Map(std::istream& stream);

    void addCar(Car* car) const;
    Surface operator[](const Vector2D& position) const;

    // TODO: Squares could be stored as a std::unordered_map<Vector2D,Surface> for faster lookup
    const std::vector<Square> mSquares; // All unmentioned Squares are assumed to have the Track surface
    const Vector2D mDirection; // You may only reach the finish line at a positive dot product with this vector
    const Car* mCar; // Single player for now
};
