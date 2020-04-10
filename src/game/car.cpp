#include "car.h"

#include "driving.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <ostream>

// ======== Car member functions ========

Car::Car(const Race& race, const IDriver& driver, char icon)
    : mRace(race)
    , mDriver(driver)
    , mIcon(icon)
{
    assert(icon != ' ' && icon != '.' && icon != 'X' && icon != '=');
    mRace.mMap.addCar(this);
}

bool Car::drive()
{
    if (mRace.mLaps == mLapTimes.size())
    {
        return true;
    }

    mTrajectory.push_back(mPosition);

    const DrivingAction action = mDriver.run(*this);
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
            const Surface& surface = mRace.mMap[mPosition];
            if (moveOnSurface(surface))
            {
                // Race completed!
                return true;
            }
            mPrevSquare = mPosition;
        }
    }
    // Apply the last little bit of velocity
    mPosition += mDirection * (mSpeed - (unsigned)mSpeed);
    newSquare = mPosition.rounded();
    if (currentSquare != newSquare)
    {
        const Surface& arrivalSurface = mRace.mMap[mPosition];
        return moveOnSurface(arrivalSurface);
    }
    return false;
}

size_t Car::getRaceTime()
{
    if (mLapTimes.size() < mRace.mLaps)
    {
        // Disqualified
        return mRace.mTimeout;
    }
    return std::accumulate(mLapTimes.cbegin(), mLapTimes.cend(), 0);
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
        if (mRace.mMap[mPrevSquare] == Surface::FinishLine && mDirection * mRace.mMap.mFinishLineDirection)
        {
            mLeftFinishLine = true;
        }
        return false;
    case Surface::Wall:
        bounceBack();
        return false;
    case Surface::FinishLine:
        if (mLeftFinishLine && mRace.mMap[mPosition] == Surface::FinishLine && mDirection * mRace.mMap.mFinishLineDirection)
        {
            // Wheeee!
            mLapTimes.emplace_back(mCurrentLapTime);
            mCurrentLapTime = 0;
            mLeftFinishLine = false;
            if (mRace.mLaps == mLapTimes.size())
            {
                // We win!
                return true;
            }
        }
        return false;
    default:
        assert(false);
    }
    return false;
}

std::ostream& operator<<(std::ostream& stream, const Car& car)
{
    // Print useful debug info
    stream << "Car '" << car.mIcon
        << "': Lap " << car.mLapTimes.size() + 1
        << ", Laptime: " << car.mCurrentLapTime
        << ", Position: " << car.mPosition
        << ", Direction: " << car.mDirection
        << ", Speed: " << car.mSpeed
        << std::endl;
    return stream;
}
