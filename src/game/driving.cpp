#include "driving.h"

#include "car.h"

#include <cassert>

genetic::IOrganism* sampleDrivingStrategies::NullStrategy::clone(const genetic::Genome& car) const
{
    // No parameters to configure
    return new NullStrategy;
}

DrivingAction sampleDrivingStrategies::NullStrategy::run(const Car&) const
{
    return DrivingAction::KeepGoing;
};

genetic::IOrganism* sampleDrivingStrategies::ManiacStrategy::clone(const genetic::Genome& genome) const
{
    assert(!genome.empty());
    ManiacStrategy* strategy = new ManiacStrategy;
    strategy->mMaxSpeed = genome[0];
    return strategy;
}

DrivingAction sampleDrivingStrategies::ManiacStrategy::run(const Car& car) const
{
    if (car.mSpeed < mMaxSpeed)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
};

genetic::IOrganism* sampleDrivingStrategies::CircleStrategy::clone(const genetic::Genome& genome) const
{
    // No parameters to configure
    return new CircleStrategy;
}

DrivingAction sampleDrivingStrategies::CircleStrategy::run(const Car& car) const
{
    // Prefer using the Car's state to keeping a bool in this class
    if (car.mSpeed < 0.001)
    {
        return DrivingAction::Accelerate;
    }
    // Steer right forever
    return DrivingAction::SteerRight;
};
