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
    void steerLeft() { /*TODO implement*/ }
    void steerRight() { /*TODO implement*/ }

    void moveOnSurface(Surface surface) { /*TODO implement*/ }
    void checkFinishLine() { /*TODO implement*/ }

private:
    const Map& mMap;
    const IDrivingStrategy& mStrategy;

    Vector2D mPosition;
    Vector2D mVelocity;

    unsigned mTime; // Number of rounds elapsed since the start of the race
    unsigned mLaps; // Number of completed laps so far

    friend void Map::addCar(Car*) const;
    friend class sampleDrivingStrategies::NullStrategy;
    friend class sampleDrivingStrategies::ManiacStrategy;
};
