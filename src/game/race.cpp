#include "race.h"

#include "car.h"
#include "map.h"

#include <iostream>
#include <stdlib.h>

Race::Race(const Map& map, size_t laps, size_t timeout)
    : mMap(map)
    , mLaps(laps)
    , mTimeout(timeout)
{
}

void Race::race(Car& car) const
{
    for (size_t tick = 0; tick < mTimeout; ++tick)
    {
        if (car.drive())
        {
            // The car finished the race
            return;
        }
    }
}

void Race::raceWithUI(Car& car) const
{
    std::cout << mMap;
    std::cout << "Ready... Set... Go! Please press Return." << std::endl;
    std::getchar();
    for (size_t tick = 0; tick < mTimeout; ++tick)
    {
        if (car.drive())
        {
            std::cout << "Congratulations! Your car has finished the race!" << std::endl;
            std::cout << "Please type Q to quit." << std::endl;
            std::cout << "> ";
            char input;
            while ((input = std::getchar()) != 'q' && input != 'Q');
            return;
        }
        system("cls");
        std::cout << mMap;
        std::cout << "Please press Return or type Q to quit..." << std::endl;
        std::cout << "> ";
        const char input = std::getchar();
        if ('q' == input || 'Q' == input)
        {
            return;
        }
    }
    std::cout << "Your car timed out. I'm sorry." << std::endl;
    std::cout << "Please type Q to quit." << std::endl;
    std::cout << "> ";
    char input;
    while ((input = std::getchar()) != 'q' && input != 'Q');
}
