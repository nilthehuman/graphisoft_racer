#include "car.h"

#include "driving.h"

#include <algorithm>
#include <cassert>

Car::Car(const Map& map, const IDrivingStrategy& strategy) : mMap(map), mStrategy(strategy)
{
    map.addCar(this);
}

void Car::drive()
{
    mTrajectory.push_back(mPosition);

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

    ++mCurrentLapTime;

    Vector2D currentSquare = mPosition.rounded();
    Vector2D newSquare;
    for (size_t i = 1; i <= mSpeed; ++i)
    {
        mPosition += mDirection;
        newSquare = mPosition.rounded();
        if (currentSquare != newSquare)
        {
            // Just entered a new square
            currentSquare = newSquare;
            const Surface& surface = mMap[mPosition];
            if (!moveOnSurface(surface))
            {
                // Hit a wall and bounced back, don't go any further
                return;
            }
            mPrevSquare = mPosition;
        }
    }
    // Apply the last little bit of velocity
    mPosition += mDirection * (mSpeed - (unsigned)mSpeed);
    newSquare = mPosition.rounded();
    if (currentSquare != newSquare)
    {
        const Surface& arrivalSurface = mMap[mPosition];
        moveOnSurface(arrivalSurface);
    }
}

void Car::accelerate()
{
    mSpeed += 1;
}

void Car::decelerate()
{
    mSpeed -= 1;
    mSpeed = std::max(0.0, mSpeed);
}

void Car::bounceBack()
{
    mPosition = mPrevSquare;
    mSpeed = 0;
}

void Car::steerLeft()
{
    // Right now steering is even allowed when standing still
    mDirection.rotate(15); // Use an increment of 15 degrees, this can be tuned later
}

void Car::steerRight()
{
    // Right now steering is even allowed when standing still
    mDirection.rotate(-15); // Use an increment of 15 degrees, this can be tuned later
}

bool Car::moveOnSurface(Surface surface)
{
    switch (surface)
    {
    case Surface::Gravel:
        decelerate();
        // Attention: intentional fallthrough!
    case Surface::Track:
        if (mMap[mPrevSquare] == Surface::FinishLine && mDirection * mMap.mFinishLineDirection)
        {
            mLeftFinishLine = true;
        }
        return true;
    case Surface::Wall:
        bounceBack();
        return false;
    case Surface::FinishLine:
        if (mLeftFinishLine && mMap[mPosition] == Surface::FinishLine && mDirection * mMap.mFinishLineDirection)
        {
            // Wheeee!
            mLapTimes.emplace_back(mCurrentLapTime);
            mCurrentLapTime = 0;
            mLeftFinishLine = false;
        }
        return true;
    default:
        assert(false);
    }
    return true;
}

// TODO: implement the remaining Car member function definitions
