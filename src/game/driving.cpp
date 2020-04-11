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
    ManiacDriver* driver = new ManiacDriver(genome[0]);
    assert(0 < driver->mMaxSpeed);
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
    if (car.mSpeed < epsilon)
    {
        return DrivingAction::Accelerate;
    }
    // Steer right forever
    return DrivingAction::SteerRight;
};

genetic::IOrganism* sampleDrivers::NascarDriver::spawn(const genetic::Genome& genome) const
{
    assert(2 <= genome.size());
    NascarDriver* driver = new NascarDriver(genome[0], genome[1]);
    assert(0 < driver->mMaxSpeed);
    assert(0 < driver->mLookAhead);
    return driver;
}

DrivingAction sampleDrivers::NascarDriver::drive(const Car& car) const
{
    // Our most advanced driving logic so far...
    for (double i = 1; i < mLookAhead; ++i)
    {
        const Vector2D point = car.mPosition + car.mDirection * i;
        const Surface& surfaceAhead = car.mRace.mMap[point];
        if (surfaceAhead == Surface::Wall || surfaceAhead == Surface::Gravel)
        {
            // We just assume we're seeing the left wall ahead, no guarantee
            return DrivingAction::SteerRight;
        }
    }
    if (car.mSpeed < mMaxSpeed)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
}
