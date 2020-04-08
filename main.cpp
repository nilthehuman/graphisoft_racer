#include "car.h"
#include "map.h"

#include <iostream>

int main()
{
    Map& map = sampleMaps::empty;
    Car myCar(map, sampleDrivingStrategies::NullStrategy());
    myCar.drive();

    return 0;
}
