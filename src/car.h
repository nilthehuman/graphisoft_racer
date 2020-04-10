#pragma once

#include "map.h"

// ======== Necessary predeclarations ========

class IDrivingStrategy;

namespace sampleDrivingStrategies {
    class NullStrategy;
    class ManiacStrategy;
    class CircleStrategy;
}

namespace unitTests
{
    namespace CarTests
    {
        void createCar();
        void throttleAndBrake();
        void steer();
    }
}

namespace componentTests
{
    void staySafeStayHome();
}

// ======== Here comes the real stuff ========

// This is your avatar in the game world. It also implements most of the game logic
class Car
{
public:
    // Spawns a new car standing on the finish line
    Car(const Map& map, const IDrivingStrategy& strategy);

    void drive();

private:
    void accelerate();
    void decelerate();
    void bounceBack();
    void steerLeft();
    void steerRight();

    bool moveOnSurface(Surface surface); // false means you hit a wall

private:
    const Map& mMap;
    const IDrivingStrategy& mStrategy;

    Vector2D mPrevSquare; // The last square the Car passed through, _not_ its position in the last round!
    Vector2D mPosition;
    Vector2D mDirection; // Take care to keep this normalized
    double mSpeed;
    std::vector<Vector2D> mTrajectory; // Records positions at the beginning of each round

    unsigned mCurrentLapTime = 0; // Number of rounds elapsed since the start of the latest lap
    std::vector<unsigned> mLapTimes; // Number of rounds each lap was completed in
    bool mLeftFinishLine = false; // Make sure we don't count standing on the finish line as several laps

    friend void Map::addCar(Car*) const;
    friend void unitTests::CarTests::createCar();
    friend void unitTests::CarTests::throttleAndBrake();
    friend void unitTests::CarTests::steer();
    friend void componentTests::staySafeStayHome();
    friend class sampleDrivingStrategies::NullStrategy;
    friend class sampleDrivingStrategies::ManiacStrategy;
    friend class sampleDrivingStrategies::CircleStrategy;
};
