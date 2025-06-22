#include <sstream>
#include <SFML/Graphics.hpp>

#include <iostream>

void updateBranches(int);

const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

enum class side {LEFT, RIGHT, NONE};
side branchPos[NUM_BRANCHES];

int main ()
{
    sf::VideoMode vm(1366, 768);
    sf::View view(sf::FloatRect(0, 0, 1920, 1080));
    sf::RenderWindow window(vm, "timber!!");
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
    sf::Texture textureBranch;
    sf::Texture texturePlayer;
    sf::Sprite spritePlayer;
    sf::Texture textureRIP;
    sf::Sprite spriteRIP;
    sf::Texture textureAxe;
    sf::Sprite spriteAxe;
    sf::Texture textureLog;
    sf::Sprite spriteLog;


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
    textureBranch.loadFromFile("graphics/branch.png");
    texturePlayer.loadFromFile("./graphics/player.png");
    textureRIP.loadFromFile("./graphics/rip.png");
    textureAxe.loadFromFile("./graphics/axe.png");
    textureLog.loadFromFile("graphics/log.png");


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

    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // line axe with tree
    const float AXE_POS_LEFT = 700;
    const float AXE_POS_RIGHT= 1075;

    // player starts on the left
    side playerSide = side::LEFT;

    // is bee moving
    bool beeActive = false;

    // is clouds moving
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // log status
    bool logActive = false;

    // bee speed
    float beeSpeed = 0.0f;

    // cloud speed
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // log speed
    float logSpeedX = 1000;
    float logSpeedY = -1500;

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

    // Control the player input
    bool acceptInput = false;

    while (window.isOpen())
    {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
 
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyReleased && !isPaused)
            {
                // listen for keyboard input again
                acceptInput = true;

                // hide axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);

            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            isPaused = false;
            score = 0;
            timeRemaining = 6.0f;

            // branches vamos
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPos[i] = side::NONE;
            }

            // gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // player in position
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

    

        if (acceptInput)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POS_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);

                // update branch
                updateBranches(score);

                // log to left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;
            }

        

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POS_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                
                updateBranches(score);

                // log to right
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;
            }

        }      


        if (timeBar.getSize().x < 0.0f)
        {
            isPaused = true;
            messageText.setString("Game Over!");

            sf::FloatRect bounds = messageText.getLocalBounds();

            messageText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
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

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;

                if (branchPos[i] == side::LEFT)
                {
                    // move sprite to left side
                    branches[i].setPosition(610, height);

                    // rotate to the other side
                    branches[i].setRotation(180);
                } else if (branchPos[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);

                    branches[i].setRotation(0);
                } else 
                {
                    branches[i].setPosition(3000, height);
                }
            }

            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + logSpeedX * dt.asSeconds(),
                spriteLog.getPosition().y + logSpeedY * dt.asSeconds());

                if (spriteLog.getPosition().x > 2000 || spriteLog.getPosition().x < -100 )
                {
                    spriteLog.setPosition(810, 720);
                    logActive = false;
                }
            }
            if (branchPos[5] == playerSide)
            {
                // death
                isPaused = true;
                acceptInput = false;

                // rip tombstone
                spriteRIP.setPosition(525, 760);

                // hide player
                spritePlayer.setPosition(2000, 660);

                // change message
                messageText.setString("Squished!!!");

                messageText.setOrigin(messageText.getLocalBounds().left + messageText.getLocalBounds().width / 2.0f,
                                      messageText.getLocalBounds().top + messageText.getLocalBounds().height / 2.0f);

            }

        }
        
        window.clear();
        // draw background
        window.draw(spriteBackground);

        // draw tree
        window.draw(spriteTree);

        // Draw the player
        window.draw(spritePlayer);

        // Draw the axe
        window.draw(spriteAxe);

        // Draw the flying log
        window.draw(spriteLog);

        // draw score
        window.draw(scoreText);

        window.draw(spriteRIP);

        // draw message
        if (isPaused)
            window.draw(messageText);

        // draw bee
        window.draw(spriteBee);

        

        // draw clouds
        window.draw(spriteCloud1);

        window.draw(spriteCloud2);

        window.draw(spriteCloud3);

        for (int i = 0; i < NUM_BRANCHES; i++)
            window.draw(branches[i]);

        window.draw(timeBar);

        window.display();
    }
    return 0;
}

void updateBranches(int seed)
{
    // Move all the branches down one place
    for (int j = NUM_BRANCHES-1; j > 0; j--) {
    branchPos[j] = branchPos[j - 1];
    }

    //left, right or none
    srand((int)time(0)+seed);
    int r = (rand() % 5);

    if (r == 0)
        branchPos[0] = side::LEFT;
    else if (r == 1)
        branchPos[0] = side::RIGHT;
    else
        branchPos[0] = side::NONE;
}