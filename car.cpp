#include "car.h"

#include "driving.h"

#include <algorithm>
#include <cassert>
#include <cmath>

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

    Vector2D topLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
    for (size_t i = 1; i < mSpeed; ++i)
    {
        mPosition += mDirection;
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
    mPosition += mDirection * (mSpeed - (unsigned)mSpeed);
    const Vector2D newTopLeftCorner{ (double)(unsigned)mPosition.mX, (double)(unsigned)mPosition.mY };
    if (topLeftCorner.mX != newTopLeftCorner.mX || topLeftCorner.mY != newTopLeftCorner.mY)
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
    // Use an increment of 15 degrees, this can be tuned later
    const double deltaRadian = pi * (15 / 180);
    const double sine   = std::sin(deltaRadian);
    const double cosine = std::cos(deltaRadian);
    const double rotMatrix[2][2] = { {cosine, -sine}, {sine, cosine} };
    mDirection = { mDirection.mX * rotMatrix[0][0] + mDirection.mY * rotMatrix[0][1], mDirection.mX * rotMatrix[1][0] + mDirection.mY * rotMatrix[1][1] };
}

void Car::steerRight()
{
    // Use an increment of 15 degrees, this can be tuned later
    const double deltaRadian = pi * (15 / 180);
    const double sine   = std::sin(deltaRadian);
    const double cosine = std::cos(deltaRadian);
    const double rotMatrix[2][2] = { {-cosine, sine}, {-sine, -cosine} };
    mDirection = { mDirection.mX * rotMatrix[0][0] + mDirection.mY * rotMatrix[0][1], mDirection.mX * rotMatrix[1][0] + mDirection.mY * rotMatrix[1][1] };
}

bool Car::moveOnSurface(Surface surface)
{
    switch (surface)
    {
    case Surface::Gravel:
        decelerate();
        // Attention: intentional fallthrough!
    case Surface::Track:
        if (mMap[mPrevSquare] == Surface::FinishLine && mDirection * mMap.mDirection)
        {
            mLeftFinishLine = true;
        }
        return true;
    case Surface::Wall:
        bounceBack();
        return false;
    case Surface::FinishLine:
        if (mLeftFinishLine && mMap[mPosition] == Surface::FinishLine && mDirection * mMap.mDirection)
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
