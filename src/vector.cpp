#include "vector.h"

#include <cmath>
#include <tuple>

bool Vector2D::operator==(const Vector2D& other) const
{
    const double dX = std::abs(mX - other.mX);
    const double dY = std::abs(mY - other.mY);
    return dX < epsilon && dY < epsilon;
}

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

double Vector2D::operator*(const Vector2D& other) const
{
    return mX * other.mX + mY * other.mY;
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

Vector2D& Vector2D::rotate(double degrees)
{
    const double radian = pi * (degrees / 180.);
    const double sine = std::sin(radian);
    const double cosine = std::cos(radian);
    const double rotMatrix[2][2] = { {cosine, -sine}, {sine, cosine} };
    // I could use a tempX, but this is more expressive and elegant
    std::tie( mX, mY ) = std::pair( mX * rotMatrix[0][0] + mY * rotMatrix[0][1],
                                    mX * rotMatrix[1][0] + mY * rotMatrix[1][1] );
    return *this;
}
