#include "driving.h"

#include "car.h"

#include <cassert>

const Race* IDrivingStrategy::sSelectedRace = nullptr;

double IDrivingStrategy::fitness() const
{
    assert(sSelectedRace);
    // Get a car and step on that throttle
    Car myCar(*sSelectedRace, *this);
    sSelectedRace->race(myCar);
    const size_t time = myCar.getRaceTime();
    return (double)(-1 * time); // Negate time to get a fitness score
}

// =================================================================

genetic::IOrganism* sampleDrivingStrategies::NullStrategy::spawn(const genetic::Genome& car) const
{
    // No parameters to configure
    return new NullStrategy;
}

DrivingAction sampleDrivingStrategies::NullStrategy::run(const Car&) const
{
    return DrivingAction::KeepGoing;
};

genetic::IOrganism* sampleDrivingStrategies::ManiacStrategy::spawn(const genetic::Genome& genome) const
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

genetic::IOrganism* sampleDrivingStrategies::CircleStrategy::spawn(const genetic::Genome& genome) const
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
