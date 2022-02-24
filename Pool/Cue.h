#pragma once

#include "Ball.h"

/*
	Cue class implements behaviour for the cue
*/

class Cue
{
private:
	vector<Ball*>* m_balls{ nullptr };
	Ball* m_ball{ nullptr };
	Vector2 m_cue;
	int m_cuePower{ 0 };
	int m_cueMinPower{ 100 };
	int m_cueMaxPower{ 600 };

	Image* m_cuePowerRender{ nullptr };

	sf::Music* m_cueSound{ nullptr };

public:
	Cue() = default;
	Cue(vector<Ball*>* balls);
	~Cue();

	void AddToPower(int amount);
	int GetPowerPercentage();

	void Rotate(float angle);
	void Position(Ball* target);

	void PlaySound();

	Vector2 GetCueVelocity() const;

	void Render(sf::RenderWindow& window, bool isSimulating) const;
};

