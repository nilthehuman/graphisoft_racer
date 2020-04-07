#include "map.h"

#include "car.h"

void Map::addCar(Car* car)
{
    car->mPosition = mFinishLine.mSquares[0];
    car->mVelocity = { 0, 0 };
    mCar = car;
}

Map sampleMaps::empty({}, Map::FinishLine{ {{0, 0, Surface::Track}}, {1, 0} });
