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
class IDrivingStrategy : public genetic::IOrganism
{
public:
    // ======== This map will be used to measure driver fitness ========
    static const Race* sSelectedRace;
    // =================================================================

public:
    virtual double fitness() const override;
    virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override = 0;

    virtual DrivingAction run(const Car& car) const = 0;
};

// ======== Sample strategies, wheeeeee! ========

namespace sampleDrivingStrategies {

    class NullStrategy : public IDrivingStrategy
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction run(const Car&) const override;
    };

    class ManiacStrategy : public IDrivingStrategy
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction run(const Car& car) const override;

    private:
        double mMaxSpeed;
    };

    class CircleStrategy : public IDrivingStrategy
    {
    public:
        virtual genetic::IOrganism* spawn(const genetic::Genome& genome) const override;

        virtual DrivingAction run(const Car&) const override;
    };
}
