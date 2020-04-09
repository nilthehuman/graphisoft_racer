#include "car.h"

#include "driving.h"

#include <cassert>

Car::Car(const Map& map, const IDrivingStrategy& strategy) : mMap(map), mStrategy(strategy)
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

    ++mCurrentLapTime;

    const Vector2D dir = mVelocity.normalized();
    const double speed = mVelocity.length();
    Vector2D topLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
    for (size_t i = 1; i < speed; ++i)
    {
        mPosition += dir;
        const Vector2D newTopLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
        if (topLeftCorner.mX != newTopLeftCorner.mX || topLeftCorner.mY != newTopLeftCorner.mY)
        {
            // Just entered a new square
            topLeftCorner = newTopLeftCorner;
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
    mPosition += mVelocity - (dir * (unsigned)speed);
    const Vector2D newTopLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
    if (topLeftCorner.mX != newTopLeftCorner.mX || topLeftCorner.mY != newTopLeftCorner.mY)
    {
        const Surface& arrivalSurface = mMap[mPosition];
        moveOnSurface(arrivalSurface);
    }
}

bool Car::moveOnSurface(Surface surface)
{
    switch (surface)
    {
    case Surface::Gravel:
        decelerate();
        // Attention: intentional fallthrough!
    case Surface::Track:
        if (mMap[mPrevSquare] == Surface::FinishLine)
        {
            mLeftFinishLine = true;
        }
        return true;
    case Surface::Wall:
        bounceBack();
        return false;
    case Surface::FinishLine:
        if (mLeftFinishLine && mMap[mPosition] == Surface::FinishLine && mVelocity * mMap.mDirection)
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
