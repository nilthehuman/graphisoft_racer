#pragma once

#include "car.h"
#include "driving.h"
#include "map.h"
#include "vector.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>

const char* emptyMapFilePath = "C:/work/graphisoft_racer/maps/empty.map"; // Adam, relative path please
const char* doughnutMapFilePath = "C:/work/graphisoft_racer/maps/doughnut.map"; // Adam, relative path please

// ======== Unit tests ========

namespace unitTests
{
    namespace Vector2DTests
    {
        void equalityAfterAddition()
        {
            Vector2D vec(0, 1);
            assert(vec == vec);
            Vector2D sumVec(0, 0);
            sumVec += Vector2D( 30,  2);
            sumVec += Vector2D(-30, -1);
            assert(vec == sumVec);
            sumVec = Vector2D(30, 2) + Vector2D(-30, -1);
            assert(vec == sumVec);
        }

        void equalityAfterSubtraction()
        {
            Vector2D vec(0, 1);
            assert(vec == vec);
            Vector2D diffVec;
            diffVec += Vector2D(30, 2);
            diffVec -= Vector2D(30, 1);
            assert(vec == diffVec);
            diffVec = Vector2D(30, 2) - Vector2D(30, 1);
            assert(vec == diffVec);
        }

        void lengthGrowsLinearly()
        {
            Vector2D vec(1, 1);
            assert(vec.length() == sqrt(2));
            vec *= 2;
            assert(vec.length() == 2*sqrt(2));
            vec *= 5;
            assert(vec.length() == 10*sqrt(2));
            vec /= 20;
            assert(vec.length() == 0.5*sqrt(2));
        }

        void normalizationDoesNotChangeDirection()
        {
            Vector2D vec(25, 44.4);
            Vector2D normal = vec.normalized();
            assert(std::abs(normal.length() - 1) < epsilon);
            assert(std::abs(vec * normal - vec.length() * normal.length()) < epsilon);
        }

        void rotatesCancelOut()
        {
            Vector2D vec(0, 1);
            vec.rotate(120);
            vec.rotate(-30);
            vec.rotate(-90);
            assert(vec == Vector2D(0, 1));
        }

        void rotateBackAround()
        {
            Vector2D vec(0, 1);
            vec.rotate(120);
            vec.rotate(120);
            vec.rotate(120);
            assert(vec == Vector2D(0, 1));
        }

        void runAll()
        {
            equalityAfterAddition();
            equalityAfterSubtraction();
            lengthGrowsLinearly();
            normalizationDoesNotChangeDirection();
            rotatesCancelOut();
            rotateBackAround();
        }
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
            assert(mapFromSquares.mDirection == Vector2D(0, 1));

            Map mapFromMap(mapFromSquares.mSquares);
            assert(mapFromMap.mSquares.size() == 4);
            for (size_t i = 0; i < 4; ++i)
            {
                assert(mapFromMap.mSquares[i].mX == i);
                assert(mapFromMap.mSquares[i].mY == 0);
                assert(mapFromMap.mSquares[i].mSurface == Surface::FinishLine);
                assert(mapFromMap[Vector2D((double)i, 0)] == Surface::FinishLine);
            }
            assert(mapFromMap.mDirection == Vector2D(0, 1));
        }

        void loadEmptyMapFromFile()
        {
            std::ifstream mapStream(emptyMapFilePath);
            const Map mapFromFile(mapStream);
            assert(mapFromFile.mSquares.size() == 4);
            for (size_t i = 0; i < 4; ++i)
            {
                assert(mapFromFile.mSquares[i].mX == i);
                assert(mapFromFile.mSquares[i].mY == 0);
                assert(mapFromFile.mSquares[i].mSurface == Surface::FinishLine);
                assert(mapFromFile[Vector2D((double)i, 0)] == Surface::FinishLine);
            }
            assert(mapFromFile.mDirection == Vector2D(0, 1));
        }

        void loadDoughnutMapFromFile()
        {
            std::ifstream mapStream(doughnutMapFilePath);
            const Map mapFromFile(mapStream);
            assert(mapFromFile.mSquares.size() == 240); // The rest is Track squares
            assert(mapFromFile[Vector2D(2, 1)] == Surface::Wall);
            assert(mapFromFile[Vector2D(5, 6)] == Surface::Track);
            assert(mapFromFile[Vector2D(8, 13)] == Surface::Gravel);
            assert(mapFromFile[Vector2D(3, 9)] == Surface::FinishLine);
            assert(mapFromFile.mDirection == Vector2D(0, 1));
        }

        void runAll()
        {
            createAndCloneMap();
            loadEmptyMapFromFile();
            loadDoughnutMapFromFile();
        }
    }

    namespace CarTests
    {
        void createCar()
        {
            const Map minimalMap({ Square(0, 0, Surface::FinishLine) });
            const IDrivingStrategy& nullStrategy = sampleDrivingStrategies::NullStrategy();
            Car car(minimalMap, nullStrategy);
            assert(car.mPrevSquare == Vector2D(0, 0));
            assert(car.mPosition == Vector2D(0, 0));
            assert(car.mDirection == minimalMap.mDirection);
            assert(std::abs(car.mSpeed) < epsilon);
        }

        void throttleAndBrake()
        {
            const Map minimalMap({ Square(0, 0, Surface::FinishLine) });
            const IDrivingStrategy& nullStrategy = sampleDrivingStrategies::NullStrategy();
            Car car(minimalMap, nullStrategy);
            car.accelerate();
            assert(std::abs(car.mSpeed - 1) < epsilon);
            car.accelerate();
            assert(std::abs(car.mSpeed - 2) < epsilon);
            car.decelerate();
            assert(std::abs(car.mSpeed - 1) < epsilon);
            car.decelerate();
            assert(std::abs(car.mSpeed - 0) < epsilon);
        }

        void steer()
        {
            const Map minimalMap({ Square(0, 0, Surface::FinishLine) });
            const IDrivingStrategy& nullStrategy = sampleDrivingStrategies::NullStrategy();
            Car car(minimalMap, nullStrategy);
            car.steerLeft();
            car.steerRight();
            assert(car.mDirection == minimalMap.mDirection);
            // 6 * 15 = 90 degrees total
            for (size_t i = 0; i < 6; ++i)
            {
                car.steerLeft();
            }
            assert(car.mDirection * minimalMap.mDirection < epsilon);
            // 12 * (-15) = -180 degrees total
            for (size_t i = 0; i < 12; ++i)
            {
                car.steerRight();
            }
            assert(car.mDirection * minimalMap.mDirection < epsilon);
        }

        void runAll()
        {
            createCar();
            throttleAndBrake();
            steer();
        }
    }

    void runAll()
    {
        Vector2DTests::runAll();
        MapTests::runAll();
        CarTests::runAll();
    }
}

// ======== Component tests ========

namespace componentTests
{
    void staySafeStayHome()
    {
        std::ifstream mapStream(emptyMapFilePath);
        const Map emptyMap(mapStream);
        const IDrivingStrategy& nullStrategy = sampleDrivingStrategies::NullStrategy();
        Car car(emptyMap, nullStrategy);
        // Going nowhere at all
        car.drive();
        car.drive();
        car.drive();
        assert(car.mPrevSquare == Vector2D(0, 0));
        assert(car.mPosition == Vector2D(0, 0));
        assert(car.mDirection == emptyMap.mDirection);
        assert(std::abs(car.mSpeed) < epsilon);
        for (const auto& footprint : car.mTrajectory)
        {
            assert(footprint == Vector2D(0, 0));
        }
        assert(car.mCurrentLapTime == 3);
        assert(car.mLapTimes.empty());
        assert(!car.mLeftFinishLine);
    }

    void runAll()
    {
        staySafeStayHome();
    }
}
