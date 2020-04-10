#include "game/car.h"
#include "game/driving.h"
#include "game/map.h"
#include "game/race.h"
#include "../test/test.h"

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
    const Race race(map, 10, 1000);
    IDriver::sSelectedRace = &race;
    race.raceWithUI(sampleDrivers::CircleDriver());

    return 0;
}
