#include "driving.h"

#include "car.h"

#include <cassert>

const Race* IDriver::sSelectedRace = nullptr;

double IDriver::fitness()
{
    assert(sSelectedRace);
    // Get a car and step on that throttle
    const size_t time = sSelectedRace->race(*this);
    return (double)(-1 * time); // Negate time to get a fitness score
}

// =================================================================

genetic::IOrganism* sampleDrivers::NullDriver::spawn(const genetic::Genome& car) const
{
    // No parameters to configure
    return new NullDriver;
}

DrivingAction sampleDrivers::NullDriver::drive(const Car&) const
{
    return DrivingAction::KeepGoing;
};

genetic::IOrganism* sampleDrivers::ManiacDriver::spawn(const genetic::Genome& genome) const
{
    assert(!genome.empty());
    ManiacDriver* driver = new ManiacDriver;
    driver->mMaxSpeed = genome[0];
    return driver;
}

DrivingAction sampleDrivers::ManiacDriver::drive(const Car& car) const
{
    if (car.mSpeed < mMaxSpeed)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
};

genetic::IOrganism* sampleDrivers::CircleDriver::spawn(const genetic::Genome& genome) const
{
    // No parameters to configure
    return new CircleDriver;
}

DrivingAction sampleDrivers::CircleDriver::drive(const Car& car) const
{
    // Prefer using the Car's state to keeping a bool in this class
    if (car.mSpeed < 0.001)
    {
        return DrivingAction::Accelerate;
    }
    // Steer right forever
    return DrivingAction::SteerRight;
};
