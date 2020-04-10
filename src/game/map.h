#pragma once

#include "vector.h"

#include <iostream>
#include <vector>

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

// An unbroken racing circuit defined on an integer grid
struct Map
{
    Map(const std::vector<Square>& squares);
	Map(std::istream& stream);

    void addCar(Car* car) const;
    Surface operator[](const Vector2D& position) const;

    // Draw a bounding box around the entire Map.
    // This is just for printing the Map to the CLI, the Car can still leave the bounding box
    const int mMinX;
    const int mMaxX;
    const int mMinY;
    const int mMaxY;
    // TODO: Squares could be stored as a std::unordered_map<Vector2D,Surface> for faster lookup
    const std::vector<Square> mSquares; // All unmentioned Squares are assumed to have the Track surface
    const Vector2D mFinishLineDirection; // You may only reach the finish line at a positive dot product with this vector
    const Car* mCar; // Single player for now
};

std::ostream& operator<<(std::ostream&, const Map&);

// Specifies the exact conditions of a race to measure the drivers' skill by
struct Race
{
    Race(const Map& map, size_t laps, size_t timeout);

    void race(Car& car) const;
    void raceWithUI(Car& car) const; // Useful for debugging a driver

    const Map& mMap; // The map the race takes place on
    const size_t mLaps; // Number of laps to be completed on the circuit
    const size_t mTimeout; // After this many ticks the driver will be disqualified
};
