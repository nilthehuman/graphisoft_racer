#include "race.h"

#include "car.h"
#include "map.h"
#include "driving.h"

#include <iostream>
#include <stdlib.h>

Race::Race(const Map& map, size_t laps, size_t timeout, size_t position)
    : mMap(map)
    , mLaps(laps)
    , mTimeout(timeout)
    , mPosition(position)
{
}

size_t Race::race(IDriver& driver) const
{
    Car car(*this, driver);
    for (size_t tick = 0; tick < mTimeout; ++tick)
    {
        if (car.drive())
        {
            // The car finished the race
            return car.getRaceTime();
        }
    }
    return car.getRaceTime();
}

size_t Race::raceWithUI(IDriver& driver) const
{
    Car car(*this, driver);
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
            return car.getRaceTime();
        }
        system("cls");
        std::cout << mMap;
        std::cout << "Please press Return or type Q to quit..." << std::endl;
        std::cout << "> ";
        const char input = std::getchar();
        if ('q' == input || 'Q' == input)
        {
            return car.getRaceTime();;
        }
    }
    std::cout << "Your car timed out. I'm sorry." << std::endl;
    std::cout << "Please type Q to quit." << std::endl;
    std::cout << "> ";
    char input;
    while ((input = std::getchar()) != 'q' && input != 'Q');
    return car.getRaceTime();
}
