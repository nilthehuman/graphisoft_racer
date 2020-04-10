#pragma once

#include "car.h"
#include "driving.h"
#include "map.h"

#include <cassert>
#include <fstream>
#include <vector>

constexpr double epsilon = 0.0001;

// ======== Unit tests ========

namespace unitTests
{
    namespace Vector2DTests
    {

    }

    namespace MapTests
    {
        void createAndCloneMap()
        {
            std::vector<Square> customSquares;
            customSquares.emplace_back(0, 0, Surface::FinishLine);
            customSquares.emplace_back(1, 0, Surface::FinishLine);
            customSquares.emplace_back(2, 0, Surface::FinishLine);
            customSquares.emplace_back(3, 0, Surface::FinishLine);

            Map mapFromSquares(customSquares);
            assert(mapFromSquares.mSquares.size() == 4);
            for (size_t i = 0; i < 4; ++i)
            {
                assert(mapFromSquares.mSquares[i].mX == i);
                assert(mapFromSquares.mSquares[i].mY == 0);
                assert(mapFromSquares.mSquares[i].mSurface == Surface::FinishLine);
                assert(mapFromSquares[Vector2D((double)i, 0)] == Surface::FinishLine);
            }
            assert(mapFromSquares.mDirection == Vector2D(0, -1));

            Map mapFromMap(mapFromSquares.mSquares);
            assert(mapFromMap.mSquares.size() == 4);
            for (size_t i = 0; i < 4; ++i)
            {
                assert(mapFromMap.mSquares[i].mX == i);
                assert(mapFromMap.mSquares[i].mY == 0);
                assert(mapFromMap.mSquares[i].mSurface == Surface::FinishLine);
                assert(mapFromMap[Vector2D((double)i, 0)] == Surface::FinishLine);
            }
            assert(mapFromMap.mDirection == Vector2D(0, -1));
        }

        void loadEmptyMapFromFile()
        {
            std::ifstream mapStream("C:/work/graphisoft_racer/maps/empty.map"); // Adam, relative path please
            const Map mapFromFile(mapStream);
            assert(mapFromFile.mSquares.size() == 4);
            for (size_t i = 0; i < 4; ++i)
            {
                assert(mapFromFile.mSquares[i].mX == i);
                assert(mapFromFile.mSquares[i].mY == 0);
                assert(mapFromFile.mSquares[i].mSurface == Surface::FinishLine);
                assert(mapFromFile[Vector2D((double)i, 0)] == Surface::FinishLine);
            }
            assert(mapFromFile.mDirection == Vector2D(0, -1));
        }

        void loadDoughnutMapFromFile()
        {
            std::ifstream mapStream("C:/work/graphisoft_racer/maps/doughnut.map"); // Adam, relative path please
            const Map mapFromFile(mapStream);
            assert(mapFromFile.mSquares.size() == 240); // The rest is Track squares
            assert(mapFromFile[Vector2D(2, 1)] == Surface::Wall);
            assert(mapFromFile[Vector2D(5, 6)] == Surface::Track);
            assert(mapFromFile[Vector2D(8, 13)] == Surface::Gravel);
            assert(mapFromFile[Vector2D(3, 10)] == Surface::FinishLine);
            assert(mapFromFile.mDirection == Vector2D(0, -1));
        }

        void runAll()
        {
            createAndCloneMap();
            loadEmptyMapFromFile();
            loadDoughnutMapFromFile();
        }
    }

    void runAll()
    {
        MapTests::runAll();
    }
}

// ======== Component tests ========

namespace componentTests
{
    void runAll()
    {
    }
}
