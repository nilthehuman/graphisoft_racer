#pragma once

#include <istream>
#include <vector>

// A pair of continuous Cartesian coordinates on a Euclidean plane
struct Vector2D
{
    double length() const;
    Vector2D normalized() const;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;

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
    Wall
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
    struct FinishLine
    {
        const std::vector<Vector2D> mSquares;
        const Vector2D mDirection; // You may only reach the finish line at a positive dot product with this vector
    };

    Map(const std::vector<Square>& squares, const FinishLine& finishLine) : mFinishLine(finishLine), mSquares(squares) {}
	Map(std::istream& stream);

    void addCar(Car* car) const;
    Surface operator[](const Vector2D& position) const;

    const std::vector<Square> mSquares; // All unmentioned Squares are assumed to have the Track surface
    const FinishLine mFinishLine;
    const Car* mCar; // Single player for now
};

// ======== Sample maps, wheeeeee! ========

namespace sampleMaps {

    // No walls, just a single square of finish line
    extern Map empty;

}
