#include "Simulation.h"

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Pool RTS - A0001151");
 
    Simulation pool;
    pool.Start();

    // Main loop that continues until we call window.close()
    while (window.isOpen())
    {
        // Handle any pending SFML events
        // These cover keyboard, mouse,joystick etc.
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            } 
        }
        
        pool.Update();
        
        // We must clear the window each time around the loop
        window.clear();

        pool.Render(window);

        // Get the window to display its contents
        window.display();

        sf::Clock c;
        while (c.getElapsedTime().asMilliseconds() < time_between_frames);
    }

    return 0;
}
