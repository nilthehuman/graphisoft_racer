#include "car.h"

#include <cassert>

Car::Car(Map& map, const IDrivingStrategy& strategy) : mMap(map), mStrategy(strategy)
{
    map.addCar(this);
}

void Car::drive()
{
    const DrivingAction action = mStrategy.run(*this);
    switch (action)
    {
    case DrivingAction::KeepGoing:
        break;
    case DrivingAction::Accelerate:
        accelerate();
        break;
    case DrivingAction::Decelerate:
        decelerate();
        break;
    case DrivingAction::SteerLeft:
        steerLeft();
        break;
    case DrivingAction::SteerRight:
        steerRight();
        break;
    default:
        assert(false);
    }

    ++mTime;

    const Vector2D dir = mVelocity.normalized();
    const double speed = mVelocity.length();
    Vector2D topLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
    for (size_t i = 1; i < speed; ++i)
    {
        mPosition += dir;
        const Vector2D newTopLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
        if (topLeftCorner.mX != newTopLeftCorner.mX || topLeftCorner.mY != newTopLeftCorner.mY)
        {
            topLeftCorner = newTopLeftCorner;
            const Surface& surface = mMap[mPosition];
            moveOnSurface(surface);
            checkFinishLine();
        }
    }
    mPosition += mVelocity - (dir * (unsigned)speed);
    const Vector2D newTopLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
    if (topLeftCorner.mX != newTopLeftCorner.mX || topLeftCorner.mY != newTopLeftCorner.mY)
    {
        const Surface& arrivalSurface = mMap[mPosition];
        moveOnSurface(arrivalSurface);
        checkFinishLine();
    }
}

// TODO: implement the remaining Car member function definitions

DrivingAction sampleDrivingStrategies::NullStrategy::run(const Car&) const
{
    return DrivingAction::KeepGoing;
};

DrivingAction sampleDrivingStrategies::ManiacStrategy::run(const Car& car) const
{
    if (car.mVelocity.length() < 1000)
    {
        return DrivingAction::Accelerate;
    }
    return DrivingAction::KeepGoing;
};
