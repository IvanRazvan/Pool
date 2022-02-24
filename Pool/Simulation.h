#pragma once

#include "Cue.h"
#include "Image.h"
#include "UIText.h"

/*
	Simulation class is the implementation of the game
*/

class Simulation
{
private:
	bool m_isSimulating{ false };

	int m_numberOfBalls{ 25 };
	int m_currentPlayer{ -1 };
	bool m_isFirstHit{ true };
	int m_redTotalScore{ 0 }, m_blueTotalScore{ 0 };
	int m_redRoundScore{ 0 }, m_blueRoundScore{ 0 };

	vector<Ball*> m_balls;
	Cue* m_cue{ nullptr };

	Image* m_table{ nullptr };
	UIText* m_score{ nullptr };

	sf::SoundBuffer* m_soundBuffer{ nullptr };
	

	Vector2 GenerateRandomPositionOnTable() const;
	void SetBoard();

	void RenderScore(sf::RenderWindow& window);

	void GetPlayerInput();
	void PassTurn();
	bool IsIdle();

	void CalculateScore();
	
public:
	Simulation() = default;
	~Simulation();

	void Start();
	void Update();
	void Render(sf::RenderWindow& window);
};

