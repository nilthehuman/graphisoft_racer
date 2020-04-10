#include "driving.h"

#include "car.h"

DrivingAction sampleDrivingStrategies::NullStrategy::run(const Car&) const
{
    return DrivingAction::KeepGoing;
};

DrivingAction sampleDrivingStrategies::ManiacStrategy::run(const Car& car) const
{
    if (car.mSpeed < 1000)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
};

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
