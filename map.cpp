#include "map.h"

#include "car.h"

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

void Map::addCar(Car* car)
{
    car->mPosition = mFinishLine.mSquares[0];
    car->mVelocity = { 0, 0 };
    mCar = car;
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

Map sampleMaps::empty({}, Map::FinishLine{ {{0, 0}}, {1, 0} });
