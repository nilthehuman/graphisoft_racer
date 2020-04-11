#pragma once

#include "../genetic/evolver.h"

enum class DrivingAction
{
    KeepGoing,
    Accelerate,
    Decelerate,
    SteerLeft,
    SteerRight
};

struct Race;
class Car;

// An algorithm that picks a DrivingAction based on the current state of the world
class IDriver : public genetic::IOrganism
{
public:
    // ======== This map will be used to measure driver fitness ========
    static const Race* sSelectedRace;
    // =================================================================

public:
    virtual double fitness() override;
    virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override = 0;

    // This function makes the actual driving decisions
    virtual DrivingAction drive(const Car& car) const = 0;
};

// ======== Sample drivers, wheeeeee! ========

namespace drivers {

    // This one fell asleep at the start
    class NullDriver : public IDriver
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car&) const override;
    };

    // This one just steps on the gas until it crashes
    class ManiacDriver : public IDriver
    {
    public:
        ManiacDriver(double maxSpeed) : mMaxSpeed(maxSpeed) {}

        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car& car) const override;

    private:
        double mMaxSpeed;
    };

    // This Driver will keep steering right and hope for the best
    class CircleDriver : public IDriver
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car& car) const override;
    };

    // This guy will try and follow the left wall by steering right
    class NascarDriver : public IDriver
    {
    public:
        NascarDriver(double maxSpeed, double lookAhead) : mMaxSpeed(maxSpeed), mLookAhead(lookAhead) {}

        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car& car) const override;

    private:
        double mMaxSpeed;
        double mLookAhead; // Start steering when it gets this close to a wall or gravel pit
    };
}
