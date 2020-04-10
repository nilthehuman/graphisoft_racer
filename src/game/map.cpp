#include "map.h"

#include "car.h"

#include <algorithm>
#include <cassert>
#include <cmath>

Map::Map(const std::vector<Square>& squares)
    : mMinX(0) // Attention: this is just to appease the compiler, the bounding box will be set in the body
    , mMaxX(0)
    , mMinY(0)
    , mMaxY(0)
    , mSquares(squares)
    , mFinishLineDirection{0, 1} // Points upward
{
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
    for (const Square& square : squares)
    {
        const Vector2D point = square.rounded();
        minX = std::min(minX, (int)point.mX);
        maxX = std::max(maxX, (int)point.mX);
        minY = std::min(minY, (int)point.mY);
        maxY = std::max(maxY, (int)point.mY);
    }
    const_cast< int& >(mMinX) = minX;
    const_cast< int& >(mMaxX) = maxX;
    const_cast< int& >(mMinY) = minY;
    const_cast< int& >(mMaxY) = maxY;
}

Map::Map(std::istream& stream)
    : mMinX(0)
    , mMaxX(0) // Attention: this is just to appease the compiler, the bounding box will be set in the body
    , mMinY(0)
    , mMaxY(0)
    , mFinishLineDirection{0, 1} // Points upward
{
    assert(stream.good());
    size_t width, height;
    stream >> width;
    stream.ignore(); // skip the 'x'
    stream >> height;
    const_cast< int& >(mMaxX) = (int)(width  - 1);
    const_cast< int& >(mMaxY) = (int)(height - 1);
    for (size_t y = 0; y < height; ++y)
    {
        stream.ignore(256, '\n');
        // Scan current line character by character
        for (size_t x = 0; x < width; ++x)
        {
            if (stream.eof())
            {
                // Stream ran out of characters :(
                assert(false);
                return;
            }
            Surface surface;
            switch (char c = stream.get())
            {
            case ' ':
                // Track Squares need not be represented explicitly, see comment at Map::mSquares
                continue;
            case '.':
                surface = Surface::Gravel;
                break;
            case 'X':
                surface = Surface::Wall;
                break;
            case '=':
                surface = Surface::FinishLine;
                break;
            default:
                // Unknown surface
                assert(false);
            }
            // y axis points upward now :)
            const_cast< std::vector<Square>& >(mSquares).emplace_back( (double)x, (double)(height - y - 1), surface );
        }
    }
}

void Map::addCar(Car* car) const
{
    const auto finishSquare = std::find_if(mSquares.cbegin(), mSquares.cend(), [](const Square& square) { return square.mSurface == Surface::FinishLine; });
    assert(finishSquare != mSquares.cend());
    car->mPrevSquare = { finishSquare->mX, finishSquare->mY };
    car->mPosition = { finishSquare->mX, finishSquare->mY };
    car->mDirection = mFinishLineDirection;
    car->mSpeed = 0;
    const_cast< Map& >(*this).mCar = car;
}

Surface Map::operator[](const Vector2D& position) const
{
    // Fuck std::find_if
    for (const auto& square : mSquares)
    {
        if (square.mX - 0.5 <= position.mX && position.mX < square.mX + 0.5 &&
            square.mY - 0.5 <= position.mY && position.mY < square.mY + 0.5)
        {
            return square.mSurface;
        }
    }
    return Surface::Track;
}

std::ostream& operator<<(std::ostream& stream, const Map& map)
{
    const Vector2D carPosition = map.mCar->mPosition.rounded();
    // Stretch bounding box to fit the Car in it
    const int minX = std::min(map.mMinX, (int)carPosition.mX);
    const int maxX = std::max(map.mMaxX, (int)carPosition.mX);
    const int minY = std::min(map.mMinY, (int)carPosition.mY);
    const int maxY = std::max(map.mMaxY, (int)carPosition.mY);
    // Scan and print the whole map to the stream
    for (int y = maxY; minY <= y; --y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            const Vector2D point((double)x, (double)y);
            if (point == carPosition)
            {
                stream << map.mCar->mIcon;
            }
            else
            {
                char c;
                switch (map[point])
                {
                case Surface::Track:
                    c = ' ';
                    break;
                case Surface::Gravel:
                    c = '.';
                    break;
                case Surface::Wall:
                    c = 'X';
                    break;
                case Surface::FinishLine:
                    c = '=';
                    break;
                default:
                    assert(false);
                }
                stream << c;
            }
        }
        stream << std::endl;
    }
    // Print Car info
    stream << *map.mCar;
    stream << std::endl;
    return stream;
}
