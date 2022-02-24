#pragma once

#include "Vector2.h"
#include "Image.h"

/*
	Ball class implements behaviour for the ball
*/

class Ball
{
	int m_team{ 0 };
	Vector2 m_position;
	Vector2 m_velocity;

	sf::Sound* m_ballHit;

	Image* m_circle{ nullptr };

	void ApplyFriction();

	void CollisionWithWall(Vector2 point, Vector2 normal);
	bool IsCollidingWithBall(Ball* other);
	void CollisionCheck(vector<Ball*>& balls);

public:
	Ball() = default;
	Ball(Vector2 pos, sf::SoundBuffer* soundBuffer);
	~Ball();

	int GetTeam() const;
	void SetTeam(int team);

	Vector2 GetPosition() const;
	void SetPosition(Vector2 pos);
	int GetRadius() const;

	void SetVelocity(Vector2 velocity);
	void Simulate(vector<Ball*>& balls);

	void Render(sf::RenderWindow& window);

	bool isMoving();
};



