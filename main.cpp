#include "car.h"
#include "map.h"

#include <iostream>

int main()
{
    Map& map = sampleMaps::empty;
    Car myCar(map, new sampleDrivingStrategies::NullStrategy);
    myCar.drive();

    return 0;
}
