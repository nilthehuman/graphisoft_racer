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
