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
    void accelerate() { /*TODO implement*/ }
    void decelerate() { /*TODO implement*/ }
    void bounceBack() { /*TODO implement*/ }
    void steerLeft() { /*TODO implement*/ }
    void steerRight() { /*TODO implement*/ }

    bool moveOnSurface(Surface surface); // false means you hit a wall

private:
    const Map& mMap;
    const IDrivingStrategy& mStrategy;

    Vector2D mPrevSquare; // The last square the Car passed through, _not_ its position in the last round!
    Vector2D mPosition;
    Vector2D mVelocity;

    unsigned mCurrentLapTime; // Number of rounds elapsed since the start of the latest lap
    std::vector<unsigned> mLapTimes; // Number of rounds each lap was completed in
    bool mLeftFinishLine; // Make sure we don't count standing on the finish line as several laps

    friend void Map::addCar(Car*) const;
    friend class sampleDrivingStrategies::NullStrategy;
    friend class sampleDrivingStrategies::ManiacStrategy;
};
