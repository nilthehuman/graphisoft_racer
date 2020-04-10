#include "car.h"
#include "driving.h"
#include "map.h"
#include "../test/test.h"

#include <iostream>
#include <fstream>

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
    Car myCar(map, sampleDrivingStrategies::NullStrategy());
    myCar.drive();

    return 0;
}
