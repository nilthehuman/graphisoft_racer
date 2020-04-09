#include "car.h"
#include "map.h"

#include <iostream>
#include <fstream>

int main()
{
    std::ifstream mapStream("C:/work/graphisoft_racer/maps/doughnut.map");
    const Map map(mapStream);
    Car myCar(map, sampleDrivingStrategies::NullStrategy());
    myCar.drive();

    return 0;
}
