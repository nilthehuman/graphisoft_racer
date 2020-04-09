#include "map.h"

#include "car.h"

#include <algorithm>
#include <cassert>

double Vector2D::length() const
{
    return sqrt(mX * mX + mY * mY);
}

Vector2D Vector2D::normalized() const
{
    const double l = length();
    return Vector2D{ mX / l, mY / l };
}

Vector2D Vector2D::operator+(const Vector2D& other) const
{
    return { mX + other.mX, mY + other.mY };
}

Vector2D Vector2D::operator-(const Vector2D& other) const
{
    return { mX - other.mX, mY - other.mY };
}

Vector2D& Vector2D::operator+=(const Vector2D& other)
{
    mX += other.mX;
    mY += other.mY;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other)
{
    mX -= other.mX;
    mY -= other.mY;
    return *this;
}

Vector2D Vector2D::operator*(double scale) const
{
    return { mX * scale, mY * scale };
}

Vector2D Vector2D::operator/(double scale) const
{
    return { mX / scale, mY / scale };
}

Vector2D& Vector2D::operator*=(double scale)
{
    mX *= scale;
    mY *= scale;
    return *this;
}

Vector2D& Vector2D::operator/=(double scale)
{
    mX /= scale;
    mY /= scale;
    return *this;
}

Map::Map(std::istream& stream)
{
    const_cast< Vector2D& >(mDirection) = { 0, -1 }; // Points upward

    assert(stream.good());
    size_t mapWidth, mapHeight;
    stream >> mapWidth;
    stream.ignore(); // skip the 'x'
    stream >> mapHeight;
    for (size_t y = 0; y < mapHeight; ++y)
    {
        stream.ignore(256, '\n');
        // Scan current line character by character
        for (size_t x = 0; x < mapWidth; ++x)
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
            // y axis points downward, maybe flip it later
            const_cast< std::vector<Square>& >(mSquares).emplace_back( (double)x, (double)y, surface );
        }
    }
}

void Map::addCar(Car* car) const
{
    const auto finishSquare = std::find_if(mSquares.cbegin(), mSquares.cend(), [](const Square& square) { return square.mSurface == Surface::FinishLine; });
    assert(finishSquare != mSquares.cend());
    car->mPosition = { finishSquare->mX, finishSquare->mY };
    car->mVelocity = { 0, 0 };
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
