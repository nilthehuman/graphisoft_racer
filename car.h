#pragma once

#include "map.h"

enum class DrivingAction
{
    KeepGoing,
    Accelerate,
    Decelerate,
    SteerLeft,
    SteerRight
};

class Car;

// An algorithm that picks a DrivingAction based on the current state of the world
class IDrivingStrategy
{
public:
    virtual DrivingAction run(const Car& car) = 0;
};

namespace sampleDrivingStrategies {
    class NullStrategy;
    class ManiacStrategy;
}

// This is your avatar in the game world. It also implements most of the game logic
class Car
{
public:
    // Spawns a new car standing on the finish line
    Car(Map& map, const IDrivingStrategy* strategy);

    void drive() { /*TODO implement*/ }

private:
    void accelerate() { /*TODO implement*/ }
    void decelerate() { /*TODO implement*/ }
    void steerLeft() { /*TODO implement*/ }
    void steerRight() { /*TODO implement*/ }

    void moveOnSurface(Surface surface) { /*TODO implement*/ }

private:
    const Map& mMap;
    const IDrivingStrategy* mStrategy;

    Vector2D mPosition;
    Vector2D mVelocity;

    unsigned mTime; // Number of rounds elapsed since the start of the race
    unsigned mLaps; // Number of completed laps so far

    friend void Map::addCar(Car*);
    friend class sampleDrivingStrategies::NullStrategy;
    friend class sampleDrivingStrategies::ManiacStrategy;
};

// ======== Sample strategies, wheeeeee! ========

namespace sampleDrivingStrategies {

    class NullStrategy : public IDrivingStrategy
    {
        virtual DrivingAction run(const Car&) override;
    };

    class ManiacStrategy : public IDrivingStrategy
    {
        virtual DrivingAction run(const Car& car) override;
    };
}
