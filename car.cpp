#include "car.h"

Car::Car(Map& map, const IDrivingStrategy* strategy) : mMap(map), mStrategy(strategy)
{
    map.addCar(this);
}

// TODO: implement all Car member function definitions

DrivingAction sampleDrivingStrategies::NullStrategy::run(const Car&)
{
    return DrivingAction::KeepGoing;
};

DrivingAction sampleDrivingStrategies::ManiacStrategy::run(const Car& car)
{
    if (car.mVelocity.length() < 1000)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
};
