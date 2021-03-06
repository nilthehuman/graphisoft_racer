#pragma once

#include "map.h"
#include "race.h"

// ======== Necessary predeclarations ========

class IDriver;

namespace drivers {
    class NullDriver;
    class ManiacDriver;
    class CircleDriver;
    class NascarDriver;
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
    namespace manualDrivingTests
    {
        void staySafeStayHome();
        void roundAndRoundOnAnEmptyMap();
        void roundAndRoundOnDoughnutMap();
    }
}

// ======== Here comes the real stuff ========

// This is your avatar in the game world. It also implements most of the game logic
class Car
{
public:
    // Spawns a new car standing on the finish line
    Car(const Race& race, const IDriver& driver, char icon = '0');

    bool drive(); // true when the Car has completed the required number of laps
    size_t getRaceTime(); // Returns total number of ticks it took to complete the race

private:
    void accelerate();
    void decelerate();
    void bounceBack();
    void steerLeft();
    void steerRight();

    bool moveOnSurface(Surface surface); // true when the Car has completed the required number of laps

private:
    const Race& mRace;
    const IDriver& mDriver;

    const char mIcon; // The Car will appear as this character on the UI

    Vector2D mPrevSquare; // The last square the Car passed through, _not_ its position in the last tick!
    Vector2D mPosition;
    Vector2D mDirection; // Take care to keep this normalized
    double mSpeed;
    std::vector<Vector2D> mTrajectory; // Records positions at the beginning of each tick

    unsigned mCurrentLapTime = 0; // Number of ticks elapsed since the start of the latest lap
    std::vector<unsigned> mLapTimes; // Number of ticks each lap was completed in
    bool mLeftFinishLine = false; // Make sure we don't count standing on the finish line as several laps

    friend void Map::addCar(Car*, size_t) const;
    friend std::ostream& operator<<(std::ostream&, const Map&);
    friend std::ostream& operator<<(std::ostream&, const Car&);
    friend void unitTests::CarTests::createCar();
    friend void unitTests::CarTests::throttleAndBrake();
    friend void unitTests::CarTests::steer();
    friend void componentTests::manualDrivingTests::staySafeStayHome();
    friend void componentTests::manualDrivingTests::roundAndRoundOnAnEmptyMap();
    friend void componentTests::manualDrivingTests::roundAndRoundOnDoughnutMap();
    friend class drivers::NullDriver;
    friend class drivers::ManiacDriver;
    friend class drivers::CircleDriver;
    friend class drivers::NascarDriver;
};

std::ostream& operator<<(std::ostream&, const Car&);
