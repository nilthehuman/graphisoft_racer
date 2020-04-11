#include "driving.h"

#include "car.h"

#include <cassert>

const Race* IDriver::sSelectedRace = nullptr;

double IDriver::measureFitness()
{
    assert(sSelectedRace);
    // Get a car and step on that throttle
    // Substitute sSelectedRace->raceWithUI(*this) here if you want to see what's happening
    const size_t time = sSelectedRace->race(*this);
    mFitness = -1.0 * time; // Negate time to get a fitness score
    return mFitness;
}

// =================================================================

drivers::NullDriver::NullDriver(const genetic::Genome& genome)
    : NullDriver()
{
    const_cast<genetic::Genome&>(mGenome) = genome;
}

genetic::IOrganism* drivers::NullDriver::spawn(const genetic::Genome& genome) const
{
    // No parameters to configure
    return new NullDriver(genome);
}

DrivingAction drivers::NullDriver::drive(const Car&) const
{
    return DrivingAction::KeepGoing;
};

// =================================================================

drivers::ManiacDriver::ManiacDriver(double maxSpeed)
    : mMaxSpeed(maxSpeed)
{
    assert(0 < mMaxSpeed);
}

drivers::ManiacDriver::ManiacDriver(const genetic::Genome& genome)
    : ManiacDriver(genome[0])
{
    const_cast<genetic::Genome&>(mGenome) = genome;
}

genetic::IOrganism* drivers::ManiacDriver::spawn(const genetic::Genome& genome) const
{
    assert(!genome.empty());
    ManiacDriver* driver = new ManiacDriver(genome);
    assert(0 < driver->mMaxSpeed);
    return driver;
}

DrivingAction drivers::ManiacDriver::drive(const Car& car) const
{
    if (car.mSpeed < mMaxSpeed)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
};

// =================================================================

drivers::CircleDriver::CircleDriver(const genetic::Genome& genome)
    : CircleDriver()
{
    const_cast<genetic::Genome&>(mGenome) = genome;
}

genetic::IOrganism* drivers::CircleDriver::spawn(const genetic::Genome& genome) const
{
    // No parameters to configure
    return new CircleDriver(genome);
}

DrivingAction drivers::CircleDriver::drive(const Car& car) const
{
    // Prefer using the Car's state to keeping a bool in this class
    if (car.mSpeed < epsilon)
    {
        return DrivingAction::Accelerate;
    }
    // Steer right forever
    return DrivingAction::SteerRight;
};

// =================================================================

drivers::NascarDriver::NascarDriver(double maxSpeed, double lookAhead)
    : mMaxSpeed(maxSpeed)
    , mLookAhead(lookAhead)
{
    assert(0 < mMaxSpeed);
    assert(0 < mLookAhead);
}

drivers::NascarDriver::NascarDriver(const genetic::Genome& genome)
    : NascarDriver(genome[0], genome[1])
{
    const_cast< genetic::Genome& >(mGenome) = genome;
}

genetic::IOrganism* drivers::NascarDriver::spawn(const genetic::Genome& genome) const
{
    assert(2 <= genome.size());
    NascarDriver* driver = new NascarDriver(genome);
    assert(0 < driver->mMaxSpeed);
    assert(0 < driver->mLookAhead);
    return driver;
}

DrivingAction drivers::NascarDriver::drive(const Car& car) const
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
