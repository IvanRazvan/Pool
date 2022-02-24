#include "Vector2.h"

Vector2::Vector2(float X, float Y) : x(X), y(Y) {}

Vector2::Vector2(const Vector2& p)
{
	x = p.x;
	y = p.y;
}

// This function returns true when the vector's length is too small to produce visible changes
bool Vector2::IsNull()
{
	return Length() < 0.5f;
}

float Vector2::Length()
{
	return sqrt(x * x + y * y);
}

// This function normalizes the vector. If the vector is too small, it returns the null vector
Vector2 Vector2::Normalize()
{
	if (IsNull() == false)
	{
		float L = Length();
		return Vector2(x / L, y / L);
	}
	else
		return Vector2();
}

Vector2 Vector2::GetNormal()
{
	return Vector2(-y,x);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const float& other) const
{
	return Vector2(x * other, y * other);
}

Vector2 Vector2::operator/(const float& other) const
{
	return Vector2(x / other, y / other);
}

void Vector2::operator=(const Vector2& other)
{
	x = other.x;
	y = other.y;
}

// This operator sets the length of a vector to a value
void Vector2::operator=(const float& other)
{
	*this = Normalize() * other;
}

bool Vector2::operator==(const Vector2& rhs) const
{
	return x == rhs.x && y && rhs.y;
}

bool Vector2::operator!=(const Vector2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

void Vector2::RotateAboutOrigin(float Angle)
{
	Angle = Angle * PI / 180;

	float sinAngle{ sin(Angle) }, cosAngle{ cos(Angle) };

	Vector2 R( (x * cosAngle) - (y * sinAngle) ,(x * sinAngle) + (y * cosAngle) );

	*this = R;
}

void Vector2::RotateAboutPoint(const Vector2& P, float Angle)
{
	Vector2 tmp = *this - P;
	tmp.RotateAboutOrigin(Angle);
	*this = tmp + P;
}

void Vector2::Flip()
{
	FlipX();
	FlipY();
}

void Vector2::FlipX()
{
	x = -x;
}

void Vector2::FlipY()
{
	y = -y;
}

float Distance(Vector2 A, Vector2 B)
{
	return (A-B).Length();
}

float DotProduct(Vector2 A, Vector2 B)
{
	return A.x * B.x + A.y * B.y;
}

float CosOfAngle(Vector2 A, Vector2 B)
{
	return DotProduct(A, B) / (A.Length() * B.Length());
}
