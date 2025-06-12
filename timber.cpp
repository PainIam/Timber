#include <sstream>
#include <SFML/Graphics.hpp>

#include <iostream>

int main ()
{
    sf::VideoMode vm(1366, 768);
    sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    sf::RenderWindow window(vm, "timber!", sf::Style::Resize);
    window.setView(view);

    sf::Texture textureBackground;
    sf::Sprite spriteBackground;
    sf::Texture textureTree;
    sf::Sprite spriteTree;
    sf::Texture textureBee;
    sf::Sprite spriteBee;
    sf::Texture textureCloud;
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;


    // positions
    const float BACKGROUND_X = 0.0f;
    const float BACKGROUND_Y = 0.0f;
    const float TREE_HORIZONTAL_POSITION = 810.0f;
    const float TREE_VERTICAL_POSITION = 0.0f;
    const float BEE_X = 0.0f;
    const float BEE_Y = 800.0f;
    const float CLOUD1_X = 0.0f;
    const float CLOUD2_X = 0.0f;
    const float CLOUD3_X = 0.0f;
    const float CLOUD1_Y = 0.0f;
    const float CLOUD2_Y = 250.0f;
    const float CLOUD3_Y = 500.0f;




    // load textures
    textureBackground.loadFromFile("./graphics/background.png");
    textureTree.loadFromFile("./graphics/tree.png");
    textureBee.loadFromFile("./graphics/bee.png");
    textureCloud.loadFromFile("./graphics/cloud.png");

    // set textures to sprites and uniform or initial positions
    spriteBackground.setTexture(textureBackground);
    spriteTree.setTexture(textureTree);
    spriteBee.setTexture(textureBee);
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteBackground.setPosition(sf::Vector2f(BACKGROUND_X, BACKGROUND_Y));
    spriteTree.setPosition(sf::Vector2f( TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION));
    spriteBee.setPosition(sf::Vector2f(BEE_X, BEE_Y));
    spriteCloud1.setPosition(sf::Vector2f(CLOUD1_X, CLOUD1_Y));
    spriteCloud2.setPosition(sf::Vector2f(CLOUD2_X, CLOUD2_Y));
    spriteCloud3.setPosition(sf::Vector2f(CLOUD3_X, CLOUD3_Y));

    // is bee moving
    bool beeActive = false;

    // is clouds moving
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // bee speed
    float beeSpeed = 0.0f;

    // cloud speed
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // clock
    sf::Clock clock;

    // time bar
    sf::RectangleShape timeBar;
    float timeBarIniX = 400.0f;
    float timeBarIniY = 80.0f;
    timeBar.setSize(sf::Vector2f(timeBarIniX, timeBarIniY));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarIniX / 2, 980);

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarIniX / timeRemaining;

    // tmp
    int flap = 5;
    int score = 0;


    // text font
    sf::Font myfont;

    // load font
    if (!myfont.loadFromFile("./fonts/KOMIKAP_.ttf"))
    {
        std::cout << "could not load file\n";
        exit(-1);
    }

    // text
    sf::Text messageText;
    sf::Text scoreText;

    // set font
    messageText.setFont(myfont);
    scoreText.setFont(myfont);

    // assign message
    messageText.setString("Press Enter to Start");
    scoreText.setString("Score = 0");

    // size
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // color
    messageText.setFillColor(sf::Color(255, 255, 255));
    scoreText.setFillColor(sf::Color(255, 255, 255));

    sf::FloatRect textRect = messageText.getLocalBounds();

    // set origin to center
    messageText.setOrigin(
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
    
    );

    // set position
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // pause variable
    bool isPaused = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        } 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            isPaused = false;
            score = 0;
            timeRemaining = 6.0f;
        }
        
        if (!isPaused)
        {
            sf::Time dt = clock.restart();
            timeRemaining -= dt.asSeconds(); //remove by real time elapsed
            timeBar.setSize(sf::Vector2f((timeBarWidthPerSecond * timeRemaining), timeBarIniY));
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;
                // How high is the bee
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;

            }
            else // Move the bee
            {

                spriteBee.setPosition(
                spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y + (flap*=-1));

                // Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                    // Set it up ready to be a whole new bee next frame
                    beeActive = false;
                
            }

            if (!cloud1Active)
            {
                srand((int) time(0) * 10);
                cloud1Speed = (rand() % 100);
                srand((int)time(0) * 10);
                float height = (rand()  % 100);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = 1;
            } else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x +
                (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1920)
                    cloud1Active = 0;
            }
            // cloud 2
            if (!cloud2Active)
            {
                srand((int) time(0) * 20);
                cloud2Speed = (rand() % 100);
                srand((int)time(0) * 20);
                float height = (rand()  % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = 1;
            } else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x +
                (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

                if (spriteCloud2.getPosition().x > 1920)
                    cloud2Active = 0;
            }

            // cloud 3
            if (!cloud3Active)
            {
                srand((int) time(0) * 30);
                cloud3Speed = (rand() % 100);
                srand((int)time(0) * 30);
                float height = (rand()  % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = 1;
            } else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x +
                (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

                if (spriteCloud3.getPosition().x > 1920)
                    cloud2Active = 0;
            }

            std::stringstream ss;
            ss <<"score = " << score;
            scoreText.setString(ss.str());
        }
        
        window.clear();
        // draw background
        window.draw(spriteBackground);

        // draw tree
        window.draw(spriteTree);

        // draw score
        window.draw(scoreText);

        // draw message
        if (isPaused)
            window.draw(messageText);

        // draw bee
        window.draw(spriteBee);

        // draw clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(timeBar);
        window.display();
    }
    return 0;
}