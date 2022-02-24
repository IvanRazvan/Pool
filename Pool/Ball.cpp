#include "Ball.h"

// This function applies a linear function for friction
void Ball::ApplyFriction()
{
    if (m_velocity.Length() > 10.0f)
        m_velocity = m_velocity.Length() * 0.999 - friction;
    else
        m_velocity = m_velocity.Length() * 0.99;
}

// This function implements the collision with a wall
void Ball::CollisionWithWall(Vector2 point, Vector2 normal)
{
    Vector2 P = GetPosition() - point;
    float cosQ1 = CosOfAngle(normal, P);
    float Q1radians = acos(cosQ1);
    float Q1degrees = Q1radians * 180.0f / PI;

    if (Q1degrees <= 90)
    {
        float Q2degrees = 90 - Q1degrees;
        float Q2radians = Q2degrees * PI / 180.0f;
        float d = sin(Q2radians) * P.Length();
        float cosS = CosOfAngle(normal * (-1), m_velocity);

        float VcL = (d - radius) / cosS;

        if (VcL >= 0 && VcL<= m_velocity.Length() * attenuation)
        {
            Vector2 Vc = m_velocity.Normalize() * VcL;
            m_position = m_position + Vc;
            SetVelocity(m_velocity * 0.8f);

            if (normal.y == 0)
            {
                m_velocity.FlipX();
            }

            if (normal.x == 0)
            {
                m_velocity.FlipY();
            }
        }
    }
}

// This function implements the collision with another ball
bool Ball::IsCollidingWithBall(Ball* other)
{
    if (other->isMoving())
    {
        Vector2 V1 = m_velocity * attenuation;
        Vector2 V2 = other->m_velocity * attenuation;
        float Xv = V1.x - V2.x;
        float Yv = V1.y - V2.y;
        float Xp = GetPosition().x - other->GetPosition().x;
        float Yp = GetPosition().y - other->GetPosition().y;

        float A = pow(Xv, 2) + pow(Yv, 2);
        float B = 2.0f * Xp * Xv + 2.0f * Yp * Yv;
        float C = pow(Xp, 2) + pow(Yp, 2) - pow(2 * radius, 2);

        if (pow(B, 2) <= 4 * A * C)
        {
            return false;
        }
        else
        {
            float  t = 2;
            float t1 = (-B + sqrt(pow(B, 2) - 4 * A * C)) / (2 * A);
            float t2 = (-B - sqrt(pow(B, 2) - 4 * A * C)) / (2 * A);

            if (t1 >= 0 && t1 <= 1 && t1 < t)
                t = t1;

            if (t2 >= 0 && t2 <= 1 && t2 < t)
                t = t2;

            if (t != 2)
            {
                m_position = m_position + V1 * t;
                other->m_position = other->m_position + V2 * t;

                Vector2 S1 = GetPosition();
                Vector2 S2 = other->GetPosition();
                Vector2 V1 = m_velocity;
                Vector2 V2 = other->m_velocity;

                Vector2 Fd1 = (S2 - S1) / (S2 - S1).Length();
                Vector2 Fd2 = Fd1 * (-1);

                Vector2 F12 = Fd1 * (CosOfAngle((S2 - S1), V1) * V1.Length());
                Vector2 F21 = Fd2 * (CosOfAngle((S1 - S2), V2) * V2.Length());

                Vector2 Vr2 = V2 + F12 - F21;
                Vector2 Vr1 = V1 + V2 - Vr2;

                m_ballHit->setVolume(m_velocity.Length() / 50);

                SetVelocity(Vr1 * 0.9f);
                other->SetVelocity(Vr2 * 0.9f);
                
            }
            return t != 2;
        }
    }
    else
    {
        Vector2 A = other->GetPosition() - GetPosition();
        float cosQ = CosOfAngle(A, m_velocity);
        float Q = acos(cosQ);
        float sinQ = sin(Q);
        float d = sinQ * A.Length();

        if (d < 2 * radius)
        {
            float e = sqrt(pow(2 * radius, 2) - pow(d,2));
            float Vc = cosQ * A.Length() - e;

            if (Vc >= 0 && Vc <= m_velocity.Length() * attenuation)
            {
                m_position = m_position + m_velocity.Normalize() * (Vc - 0.05f);

                Vector2 S1 = GetPosition();
                Vector2 S2 = other->GetPosition();
                Vector2 V1 = m_velocity;

                Vector2 Fd = (S2 - S1).Normalize();

                float cosA = CosOfAngle(Fd, V1);
                Vector2 Vr2 = Fd * cosA * V1.Length();
                Vector2 Vr1 = V1 - Vr2;

                m_ballHit->setVolume(m_velocity.Length() / 50);

                SetVelocity(Vr1 * 0.9f);
                other->SetVelocity(Vr2 * 0.9f);

                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    
}

// This function checks collisions with all walls and balls
void Ball::CollisionCheck(vector<Ball*>& balls)
{
    CollisionWithWall(Vector2(tableLeft,tableTop), Vector2(0,1));
    CollisionWithWall(Vector2(tableLeft, tableBottom), Vector2(0, -1));
    CollisionWithWall(Vector2(tableLeft, tableTop), Vector2(1, 0));
    CollisionWithWall(Vector2(tableRight, tableTop), Vector2(-1, 0));
   
    for (Ball* ball : balls)
        if (ball != this && IsCollidingWithBall(ball))
        {
            m_ballHit->play();

            if (ball->GetTeam() == 0)
            {
                ball->SetTeam(m_team);
            }
        }
}

Ball::Ball(Vector2 pos, sf::SoundBuffer* soundBuffer)
    : m_position(pos)
{
    m_ballHit = new sf::Sound();
    m_ballHit->setBuffer(*soundBuffer);

    m_circle = new Image(m_position, 2 * radius, 2 * radius, "Assets\\whiteball.png");
}

Ball::~Ball()
{
    delete m_circle;
    delete m_ballHit;
}

int Ball::GetTeam() const
{
    return m_team;
}

Vector2 Ball::GetPosition() const
{
    return m_position;
}

void Ball::SetPosition(Vector2 pos)
{
    m_position = pos;
}

int Ball::GetRadius() const
{
    return radius;
}

void Ball::SetTeam(int team)
{
    m_team = team;

    switch (m_team) {
    case 0:
        m_circle->SetTexture("Assets\\whiteball.png");
        break;
    case 1:
        m_circle->SetTexture("Assets\\redball.png");
        break;
    case 2:
        m_circle->SetTexture("Assets\\blueball.png");
        break;
    }
}

void Ball::SetVelocity(Vector2 velocity)
{
    m_velocity = velocity;
}

// This function checks collisions and moves the ball accordingly
void Ball::Simulate(vector<Ball*>& balls)
{
    if (m_velocity.IsNull())
    {
        m_ballHit->stop();
        return;
    }

    ApplyFriction();
    CollisionCheck(balls);
    m_position = m_position + m_velocity * attenuation;
}

void Ball::Render(sf::RenderWindow& window)
{ 
    m_circle->SetPosition(Vector2(m_position.x - radius, m_position.y - radius));
    m_circle->Render(window);
}

bool Ball::isMoving()
{
    return m_velocity.IsNull()==false;
}
