#pragma once

#include "Defines.h"

/*
	Vector2 class implements functionality for 2D vectors and points
*/

class Vector2 {
private:

public:
	float x{ 0.0f }, y{ 0.0f };

	Vector2() = default;
	Vector2(float X, float Y);
	Vector2(const Vector2& p);

	bool IsNull();
	float Length();
	Vector2 Normalize();
	Vector2 GetNormal();

	Vector2 operator+(const Vector2& other) const;
	Vector2 operator-(const Vector2& other) const;
	Vector2 operator*(const float& other) const;
	Vector2 operator/(const float& other) const;

	void operator=(const Vector2& other);
	void operator=(const float& other);

	bool operator==(const Vector2& rhs) const;
	bool operator!=(const Vector2& rhs) const;

	void RotateAboutOrigin(float Angle);
	void RotateAboutPoint(const Vector2& P, float Angle);

	void Flip();
	void FlipX();
	void FlipY();
};

float Distance(Vector2 A, Vector2 B);
float DotProduct(Vector2 A, Vector2 B);
float CosOfAngle(Vector2 A, Vector2 B);

