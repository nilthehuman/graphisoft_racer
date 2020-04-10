#pragma once

enum class DrivingAction
{
    KeepGoing,
    Accelerate,
    Decelerate,
    SteerLeft,
    SteerRight
};

class Car;

// An algorithm that picks a DrivingAction based on the current state of the world
class IDrivingStrategy
{
public:
    virtual DrivingAction run(const Car& car) const = 0;
};

// ======== Sample strategies, wheeeeee! ========

namespace sampleDrivingStrategies {

    class NullStrategy : public IDrivingStrategy
    {
        virtual DrivingAction run(const Car&) const override;
    };

    class ManiacStrategy : public IDrivingStrategy
    {
        virtual DrivingAction run(const Car& car) const override;
    };

    class CircleStrategy : public IDrivingStrategy
    {
        virtual DrivingAction run(const Car&) const override;
    };
}
