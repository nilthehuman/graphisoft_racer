#pragma once

struct Map;
class IDriver;

// Specifies the exact conditions of a race to measure the drivers' skill by
struct Race
{
    Race(const Map& map, size_t laps, size_t timeout, size_t position = 0);

    size_t race(IDriver& driver) const;
    size_t raceWithUI(IDriver& driver) const; // Useful for debugging a driver

    const Map& mMap; // The map the race takes place on
    const size_t mLaps; // Number of laps to be completed on the circuit
    const size_t mTimeout; // After this many ticks the driver will be disqualified
    const size_t mPosition; // Which position on the finish line the Car starts on, indexed from 0
};
