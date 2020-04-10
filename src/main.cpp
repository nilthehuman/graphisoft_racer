#include "game/car.h"
#include "game/driving.h"
#include "game/map.h"
#include "../test/test.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

int main()
{
    if (true)
    {
        // Run all available tests
        unitTests::runAll();
        componentTests::runAll();
    }

    std::ifstream mapStream("C:/work/graphisoft_racer/maps/doughnut.map");
    const Map map(mapStream);
    IDrivingStrategy::sSelectedRace = new Race(map, 10, 1000);
    Car myCar(*IDrivingStrategy::sSelectedRace, sampleDrivingStrategies::CircleStrategy(), 'A');
    std::cout << map;
    while (true)
    {
        myCar.drive();
        system("cls");
        std::cout << map;
        std::cout << "Please press Return or type Q to quit...";
        const char input = std::getchar();
        if ('q' == input || 'Q' == input)
        {
            break;
        }
    }

    return 0;
}
