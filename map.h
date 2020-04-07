#pragma once

#include <vector>

// A pair of continuous Cartesian coordinates on a Euclidean plane
struct Vector2D
{
    double length()
    {
        return sqrt(mX * mX + mY * mY);
    };

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
        const std::vector<Square> mSquares;
        Vector2D mDirection; // You may only reach the finish line at a positive dot product with this vector
    };

    Map(const std::vector<Square>& squares, const FinishLine& finishLine) : mFinishLine(finishLine), mSquares(squares) {}

    void addCar(Car* car);

    const std::vector<Square> mSquares; // All unmentioned Squares are assumed to have the Track surface
    const FinishLine mFinishLine;
    const Car* mCar; // Single player for the time being
};

// ======== Sample maps, wheeeeee! ========

namespace sampleMaps {

    // No walls, just a single square of finish line
    extern Map empty;

}
