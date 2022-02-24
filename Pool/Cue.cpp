#include "Cue.h"

Cue::Cue(vector<Ball*>* balls)
    :   m_balls(balls)
{
    m_cuePowerRender = new Image(Vector2(12,142), 49, 616, "Assets\\cuepower.png");

    m_cueSound = new sf::Music();
    if (!m_cueSound->openFromFile("Assets\\cue_hit_sound_effect.ogg"))
        std::cout << "Sound not found: Assets\\cue_hit_sound_effect.ogg";
}

Cue::~Cue()
{
    delete m_cuePowerRender;
    delete m_cueSound;
}

// This function adds to the power of the shot
void Cue::AddToPower(int amount)
{
    m_cuePower += amount;

    if (m_cuePower > m_cueMaxPower)
        m_cuePower = m_cueMaxPower;

    if (m_cuePower < m_cueMinPower)
        m_cuePower = m_cueMinPower;

    m_cuePowerRender->SetHeight(616 - (m_cuePower - m_cueMinPower) * 616 / (m_cueMaxPower - m_cueMinPower));
}

// This function returns the % of power for the shot
int Cue::GetPowerPercentage()
{
    return ((m_cuePower - m_cueMinPower) * 100) / (m_cueMaxPower - m_cueMinPower);
}

// This function rotates the cue around the player ball
void Cue::Rotate(float angle)
{
    m_cue.RotateAboutPoint(m_ball->GetPosition(), angle);
}

// This function moves the cue to the player ball
void Cue::Position(Ball* target)
{
    m_ball = target;

    m_cue.x = 0;
    m_cue.y = -10;
    m_cuePower = m_cueMinPower + (m_cueMaxPower - m_cueMinPower) / 2;
    m_cuePowerRender->SetHeight(616 - (m_cuePower - m_cueMinPower) * 616 / (m_cueMaxPower - m_cueMinPower));
    m_cue = m_cue + m_ball->GetPosition();
}

// This function plays the hit sound
void Cue::PlaySound()
{
    m_cueSound->stop();
    m_cueSound->setVolume(GetPowerPercentage());
    m_cueSound->play();
}

// This function gets the vector of the shot
Vector2 Cue::GetCueVelocity() const
{
    return (m_cue - m_ball->GetPosition()) * m_cuePower;
}

// This function renders the cue and predicts the shot outcome
void Cue::Render(sf::RenderWindow& window, bool isSimulating) const
{
    m_cuePowerRender->Render(window);
    
    if (isSimulating)
        return;

    Vector2 pos = m_ball->GetPosition();
    Vector2 playerPathDir = (m_cue - m_ball->GetPosition()).Normalize();
    vector<Vector2> playerPath;
    vector<Vector2> hitPath;
    playerPath.emplace_back(pos);

    // This loop simulates the first collision up to 250 units away from the player ball
    for (int i = 0; i < 250; i++)
    {
        // Checks the collision with walls
        if (pos.x - radius < tableLeft)
        {
            pos.x = tableLeft + radius;
            playerPath.emplace_back(pos);
            playerPathDir.FlipX();

            pos = pos + playerPathDir * (250 - i);
            break;
        }

        if (pos.y - radius < tableTop)
        {
            pos.y = tableTop + radius;
            playerPath.emplace_back(pos);
            playerPathDir.FlipY();

            pos = pos + playerPathDir * (250 - i);
            break;
        }

        if (pos.x + radius > tableRight)
        {
            pos.x = tableRight - radius;
            playerPath.emplace_back(pos);
            playerPathDir.FlipX();

            pos = pos + playerPathDir * (250 - i);
            break;
        }

        if (pos.y + radius > tableBottom)
        {
            pos.y = tableBottom - radius;
            playerPath.emplace_back(pos);
            playerPathDir.FlipY();

            pos = pos + playerPathDir * (250 - i);
            break;
        }

        // Checks the collision with balls
        for (Ball* ball : *m_balls)
            if (ball != m_ball && Distance(pos, ball->GetPosition()) < 2 * ball->GetRadius())
            {
                // If it will hit the ball, it calculates both trajectories
                playerPath.emplace_back(pos);
                hitPath.emplace_back(ball->GetPosition());

                Vector2 S1 = pos;
                Vector2 S2 = ball->GetPosition();
                Vector2 V1 = m_cue - m_ball->GetPosition();

                Vector2 Fd = (S2 - S1).Normalize();

                float cosA = CosOfAngle(Fd, V1);
                Vector2 Vr2 = Fd * cosA * V1.Length();
                Vector2 Vr1 = V1 - Vr2;

                playerPathDir = Vr1.Normalize();
                hitPath.emplace_back(ball->GetPosition() + Vr2.Normalize() * 100);

                pos = pos + playerPathDir * (250 - i);
                i = 250;
                break;
            }

        pos = pos + playerPathDir;
    }

    playerPath.emplace_back(pos);

    sf::VertexArray playerLine(sf::LineStrip, playerPath.size());

    // It draws the player path
    for (int i = 0; i < playerPath.size(); i++)
    {
        playerLine[i].position = sf::Vector2f(playerPath.at(i).x, playerPath.at(i).y);
        if (i != 0 && i != playerPath.size() - 1)
        {
            sf::CircleShape circle(m_ball->GetRadius());
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(1);
            circle.setOrigin(m_ball->GetRadius(), m_ball->GetRadius());
            circle.setPosition(sf::Vector2f(playerPath.at(i).x, playerPath.at(i).y));
            window.draw(circle);
        }
    }

    window.draw(playerLine);

    // It draws the hit ball path if there is any
    if (hitPath.size() != 0)
    {
        sf::VertexArray hitLine(sf::LineStrip, 2);
        hitLine[0].position = sf::Vector2f(hitPath.at(0).x, hitPath.at(0).y);
        hitLine[1].position = sf::Vector2f(hitPath.at(1).x, hitPath.at(1).y);

        window.draw(hitLine);
    }
    
}
