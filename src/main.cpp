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
    const Race race(map, 3, 1000, 1);
    IDriver::sSelectedRace = &race;
    IDriver& driver = drivers::NascarDriver(2, 5);
    race.raceWithUI(driver);

    return 0;
}
