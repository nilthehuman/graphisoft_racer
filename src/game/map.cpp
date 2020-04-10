#include "map.h"

#include "car.h"

#include <algorithm>
#include <cassert>
#include <cmath>

Map::Map(const std::vector<Square>& squares)
    : mWidth(0) // Attention: this is just to appease the compiler, mWidth and mHeight will be set in the body
    , mHeight(0)
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
    const_cast< size_t& >(mWidth)  = maxX - minX;
    const_cast< size_t& >(mHeight) = maxY - minY;
}

Map::Map(std::istream& stream)
    : mWidth(0) // Attention: this is just to appease the compiler, mWidth and mHeight will be set in the body
    , mHeight(0)
    , mFinishLineDirection{0, 1} // Points upward
{
    assert(stream.good());
    stream >> const_cast< size_t& >(mWidth);
    stream.ignore(); // skip the 'x'
    stream >> const_cast< size_t& >(mHeight);
    for (size_t y = 0; y < mHeight; ++y)
    {
        stream.ignore(256, '\n');
        // Scan current line character by character
        for (size_t x = 0; x < mWidth; ++x)
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
            const_cast< std::vector<Square>& >(mSquares).emplace_back( (double)x, (double)(mHeight - y - 1), surface );
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
    for (int y = map.mHeight - 1; 0 <= y; --y)
    {
        for (int x = 0; x < map.mWidth; ++x)
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
    stream << *map.mCar;
    stream << std::endl;
    return stream;
}
