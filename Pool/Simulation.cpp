#include "Simulation.h"

// This function generates a random position to spawn the balls
Vector2 Simulation::GenerateRandomPositionOnTable() const
{
    float x = tableLeft + radius + rand() % (tableRight - tableLeft - 2 * radius);
    float y = tableTop + radius + rand() % (tableBottom - tableTop - 2 * radius);

    return Vector2(x, y);
}

// This function sets the board at the beginning of each round
void Simulation::SetBoard()
{
    // Generates a random seed
    srand(time(NULL));

    m_isSimulating = false;

    // Picks the first player
    if(m_currentPlayer == -1)
        m_currentPlayer = rand() % 2;

    m_isFirstHit = true;
    m_redRoundScore = 0;
    m_blueRoundScore = 0;

    // Places the balls far from each other
    for (int i = 0; i < m_numberOfBalls; i++)
    {
        Vector2 pos;

        bool found;
        do {
            found = true;
            pos = GenerateRandomPositionOnTable();
            for (Ball* ball : m_balls)
                if (Distance(pos, ball->GetPosition()) < 7 * radius)
                {
                    found = false;
                    break;
                }
        } while (found == false);

        m_balls.at(i)->SetTeam(0);
        m_balls.at(i)->SetPosition(pos);
    }

    // Sets the player balls
    m_balls.at(0)->SetTeam(1);
    m_balls.at(1)->SetTeam(2);

    PassTurn();
}

// This function renders the round and total scores
void Simulation::RenderScore(sf::RenderWindow& window)
{
    m_score->SetColour(sf::Color::Red);
    m_score->SetPosition(Vector2(300, -18));
    m_score->SetString("Red: " + to_string(m_redRoundScore));
    m_score->Render(window);

    m_score->SetColour(sf::Color::Red);
    m_score->SetPosition(Vector2(700, -18));
    m_score->SetString(to_string(m_redTotalScore));
    m_score->Render(window);

    m_score->SetColour(sf::Color::White);
    m_score->SetPosition(Vector2(750, -18));
    m_score->SetString("VS");
    m_score->Render(window);

    m_score->SetColour(sf::Color::Blue);
    m_score->SetPosition(Vector2(835, -18));
    m_score->SetString(to_string(m_blueTotalScore));
    m_score->Render(window);

    m_score->SetColour(sf::Color::Blue);
    m_score->SetPosition(Vector2(1100, -18));
    m_score->SetString("Blue: " + to_string(m_blueRoundScore));
    m_score->Render(window);
}

// This function reads the player input
void Simulation::GetPlayerInput()
{
    // A or Left arrow to aim left. Shift to aim slower
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
            m_cue->Rotate(-0.02f);
        else
            m_cue->Rotate(-0.3f);
    }

    // A or Right arrow to aim right. Shift to aim slower
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
            m_cue->Rotate(0.02f);
        else
            m_cue->Rotate(0.3f);
    }

    // W or Up arrow to increase cue power
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && m_isFirstHit == false)
        m_cue->AddToPower(1);

    // S or Down arrow to decrease cue power
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && m_isFirstHit == false)
        m_cue->AddToPower(-1);

    // Space to shoot
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_isFirstHit = false;

        m_cue->PlaySound();

        m_balls.at(m_currentPlayer)->SetVelocity(m_cue->GetCueVelocity());

        PassTurn();
    }
}

// This function changes the player 
void Simulation::PassTurn()
{
    m_isSimulating = true;

    if (m_currentPlayer == 0)
        m_currentPlayer = 1;
    else
        m_currentPlayer = 0;
}

// This function returns true when all balls have stopped moving
bool Simulation::IsIdle()
{
    for (Ball* ball : m_balls)
        if (ball->isMoving())
            return false;

    return true;
}

// This function calculates the score and decides if the round ends
void Simulation::CalculateScore()
{
    int red{ -1 }, blue{ -1 };
    for (Ball* ball : m_balls)
    {
        if (ball->GetTeam() == 1)
            red++;
        if (ball->GetTeam() == 2)
            blue++;
    }

    if (red > m_redRoundScore)
        m_redRoundScore = red;
    if (blue > m_blueRoundScore)
        m_blueRoundScore = blue;

    if (m_redRoundScore > (m_numberOfBalls - 2) / 2)
    {
        m_redTotalScore++;
        SetBoard();
    }

    if (m_blueRoundScore > (m_numberOfBalls - 2) / 2)
    {
        m_blueTotalScore++;
        SetBoard();
    }
}

Simulation::~Simulation()
{
    for (Ball* ball : m_balls)
        delete ball;

    delete m_cue;
    delete m_table;
    delete m_score;
    delete m_soundBuffer;
}

// The constructor loads the assets
void Simulation::Start()
{
    m_score = new UIText(70, "Assets\\Font\\PartyConfettiRegular.ttf");

    m_soundBuffer = new sf::SoundBuffer();
    if (!m_soundBuffer->loadFromFile("Assets\\ball_hit_sound_effect.ogg"))
        std::cout << "Sound not found: Assets\\ball_hit_sound_effect.ogg";

    m_table = new Image(Vector2(), screenWidth, screenHeight, "Assets\\Table.png");

    for (int i = 0; i < m_numberOfBalls; i++)
        m_balls.emplace_back(new Ball(Vector2(), m_soundBuffer));

    m_cue = new Cue(&m_balls);

    SetBoard();
}

// This function updates the simulation
void Simulation::Update()
{
    // When all balls have stopped working, it calculates the score and allows the player to take their turn
    if (IsIdle() && m_isSimulating)
    {
        m_isSimulating = false;
        CalculateScore();
        m_cue->Position(m_balls.at(m_currentPlayer));
    }
    
    // If the simulation is running, it updates the balls, else it gets player input
    if (m_isSimulating)
    {
        for (Ball* ball : m_balls)
            ball->Simulate(m_balls);
    }
    else
    {
        GetPlayerInput();
    }
}

// This function renders the graphics on the screen
void Simulation::Render(sf::RenderWindow& window)
{
    m_table->Render(window);

    for (Ball* ball : m_balls)
        ball->Render(window);

    m_cue->Render(window, m_isSimulating);

    RenderScore(window);
}
