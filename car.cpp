#include "car.h"

Car::Car(Map& map, const IDrivingStrategy* strategy) : mMap(map), mStrategy(strategy)
{
    map.addCar(this);
}

// TODO: implement all Car member function definitions
