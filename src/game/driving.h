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
    virtual double fitness() const override;
    virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override = 0;

    // This function is responsible for the actual driving logic
    virtual DrivingAction run(const Car& car) const = 0;
};

// ======== Sample drivers, wheeeeee! ========

namespace sampleDrivers {

    // This one fell asleep at the start
    class NullDriver : public IDriver
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction run(const Car&) const override;
    };

    // This one just steps on the gas until it crashes
    class ManiacDriver : public IDriver
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction run(const Car& car) const override;

    private:
        double mMaxSpeed;
    };

    // This Driver will keep steering right and hope for the best
    class CircleDriver : public IDriver
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction run(const Car&) const override;
    };
}
