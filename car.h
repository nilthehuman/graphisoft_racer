#pragma once

#include "map.h"

class IDrivingStrategy;

namespace sampleDrivingStrategies {
    class NullStrategy;
    class ManiacStrategy;
}

// This is your avatar in the game world. It also implements most of the game logic
class Car
{
public:
    // Spawns a new car standing on the finish line
    Car(const Map& map, const IDrivingStrategy& strategy);

    void drive();

private:
    static constexpr double pi = 3.1415927;

    void accelerate();
    void decelerate();
    void bounceBack();
    void steerLeft();
    void steerRight();

    bool moveOnSurface(Surface surface); // false means you hit a wall

private:
    const Map& mMap;
    const IDrivingStrategy& mStrategy;

    Vector2D mPrevSquare; // The last square the Car passed through, _not_ its position in the last round!
    Vector2D mPosition;
    Vector2D mDirection; // Take care to keep this normalized
    double mSpeed;

    unsigned mCurrentLapTime; // Number of rounds elapsed since the start of the latest lap
    std::vector<unsigned> mLapTimes; // Number of rounds each lap was completed in
    bool mLeftFinishLine; // Make sure we don't count standing on the finish line as several laps

    friend void Map::addCar(Car*) const;
    friend class sampleDrivingStrategies::NullStrategy;
    friend class sampleDrivingStrategies::ManiacStrategy;
};
