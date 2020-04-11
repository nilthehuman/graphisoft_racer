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
    virtual double measureFitness() override;

    // This function makes the actual driving decisions
    virtual DrivingAction drive(const Car& car) const = 0;
};

// ======== Sample drivers, wheeeeee! ========

namespace drivers {

    // This one fell asleep at the start
    class NullDriver : public IDriver
    {
    public:
        NullDriver() = default;
        NullDriver(const genetic::Genome& genome);

        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car&) const override;
    };

    // This one just steps on the gas until it crashes
    class ManiacDriver : public IDriver
    {
    public:
        ManiacDriver(double maxSpeed);
        ManiacDriver(const genetic::Genome& genome);

        virtual const genetic::Genome getMinimumGenes() const override { return { /*mMaxSpeed =*/  1 }; }
        virtual const genetic::Genome getMaximumGenes() const override { return { /*mMaxSpeed =*/ 10 }; }
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car& car) const override;

    private:
        const double mMaxSpeed;
    };

    // This Driver will keep steering right and hope for the best
    class CircleDriver : public IDriver
    {
    public:
        CircleDriver() = default;
        CircleDriver(const genetic::Genome& genome);

        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car& car) const override;
    };

    // This guy will try and follow the left wall by steering right
    class NascarDriver : public IDriver
    {
    public:
        NascarDriver(double maxSpeed, double lookAhead);
        NascarDriver(const genetic::Genome& genome);

        virtual const genetic::Genome getMinimumGenes() const override { return { /*mMaxSpeed =*/  1,  /*mLookAhead =*/ 1 }; }
        virtual const genetic::Genome getMaximumGenes() const override { return { /*mMaxSpeed =*/ 10, /*mLookAhead =*/ 20 }; }

        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction drive(const Car& car) const override;

    private:
        const double mMaxSpeed;
        const double mLookAhead; // Start steering when it gets this close to a wall or gravel pit
    };
}
