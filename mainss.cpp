#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>
#include <sstream>

void loadTexture(sf::Texture&, const std::string&);
void centerOrigin(sf::Sprite&);
void centerTextOrigin(sf::Text&);
void seed();

enum class side {LEFT, RIGHT, NONE};
const int SIZE = 6;
std::vector<sf::Sprite> spriteBranches(SIZE);
side branchPosition[SIZE];

void shiftBranch(int seed);

int main ()
{
    // call seed for random number generation
    seed();
    sf::VideoMode vm(1366, 768);
    sf::RenderWindow window(vm, "Timber++", sf::Style::Default);
    sf::View scale(sf::FloatRect(0, 0, 1920, 1080));               
    window.setView(scale);

    // create the background
    sf::Texture textureBackground;
    sf::Sprite spriteBackground;

    // set the texture
    loadTexture(textureBackground, "./graphics/background.png");

    // set spritebackround
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(sf::Vector2f(0, 0));

    // create the tree
    sf::Texture texureTree;
    sf::Sprite spriteTree;

    // set the tree
    loadTexture(texureTree, "./graphics/tree.png");

    // set tree sprite
    spriteTree.setTexture(texureTree);
    centerOrigin(spriteTree);
    spriteTree.setPosition(sf::Vector2f(1920 / 2.0f, 900 / 2.0f));

    // create bee.
    sf::Texture textureBee;
    sf::Sprite spriteBee;

    // set the bee
    loadTexture(textureBee, "./graphics/bee.png");

    // idk, i;m high
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(sf::Vector2f(2000, 750));

    // CLOCK
    sf::Clock clock;

    // boolean for bee
    bool isBeeActive = false;

    // bee  speed
    int beeSpeed = 0;

    // bee position variable
    int bee_Y = 1100.0f;

    // create texture for cloud and load it
    sf::Texture textureCloud;
    loadTexture(textureCloud, "./graphics/cloud.png");

    // create a vector for sprites and bool
    std::vector<bool> isCloudActive(3, 0);
    std::vector<sf::Sprite> clouds;
    std::vector<int> cloudSpeed(3, 0);

    // create 3 cloud sprites
    for (int i = 0; i < 3; i++)
        clouds.emplace_back();

    // set texture for cloud sprites
    for (auto &c : clouds)
    {
        c.setTexture(textureCloud);
        c.setPosition(sf::Vector2f(-200, 5));
    }

    // load the FONT
    sf::Font font;
    font.loadFromFile("./fonts/KOMIKAP_.ttf");

    sf::Text text;
    text.setFont(font);
    text.setPosition(500, 500);
    text.setCharacterSize(30);

    // is game paused
    bool isPaused = true;

    // score tracking
    int score = 0;
    sf::Text scoreTrack;
    scoreTrack.setFont(font);
    scoreTrack.setPosition(10, 7);
    scoreTrack.setCharacterSize(40);

    // timebar
    sf::RectangleShape timeBar;
    float timeBarX, timeBarY;
    timeBarX = 600.0f;
    timeBarY = 100.0f;
    timeBar.setFillColor(sf::Color(255, 0, 0));
    timeBar.setSize(sf::Vector2f(timeBarX, timeBarY));
    timeBar.setOrigin(timeBar.getLocalBounds().width / 2.0f,
                        timeBar.getLocalBounds().height / 2.0f);
    timeBar.setPosition(1920 / 2.0f, 1000);
    float timeRemaining = 70.0f;

    // branch
    sf::Texture textureBranch;
    loadTexture(textureBranch, "./graphics/branch.png");
    for (int i = 0; i <  SIZE; i++)
    {
        spriteBranches[i].setTexture(textureBranch);
        spriteBranches[i].setPosition(-2000, -2000);
        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        centerOrigin(spriteBranches[i]);
    }
    const float BRANCH_LEFT = 600.0f;
    const float BRANCH_RIGHT = 1330.0f;

    // bool for fair play
    bool acceptInput = false;

    // player
    sf::Texture texturePlayer;
    sf::Sprite spritePlayer;
    loadTexture(texturePlayer, "./graphics/player.png");
    spritePlayer.setTexture(texturePlayer);
    float player_LeftX = 580.0f, player_Y = 730.0f, player_RightX;
    spritePlayer.setPosition(player_LeftX, player_Y);
    side playerSide;
    bool isSquished = false;

    // axe
    sf::Texture textureAxe;
    sf::Sprite spriteAxe;
    loadTexture(textureAxe, "./graphics/axe.png");
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    const float AXE_POS_LEFT = 700;
    const float AXE_POS_RIGHT= 1075;

    // wood
    sf::Texture textureLog;
    loadTexture(textureLog, "./graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    centerOrigin(spriteLog);
    const float LOG_X = 1920/2.0f;
    const float LOG_Y = 800.0f;
    spriteLog.setPosition(sf::Vector2f(LOG_X, LOG_Y));
    bool isLogActive = false;

    // sound
    sf::SoundBuffer bufferChop;
    bufferChop.loadFromFile("./sound/chop.wav");
    sf::Sound chop;
    chop.setBuffer(bufferChop);
    sf::SoundBuffer bufferDeath;
    bufferDeath.loadFromFile("./sound/death.wav");
    sf::Sound death;
    death.setBuffer(bufferDeath);
    sf::SoundBuffer bufferTime;
    bufferTime.loadFromFile("./sound/out_of_time.wav");
    sf::Sound time;
    time.setBuffer(bufferTime);
    bool isPlayed = false;
    // game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyReleased && !isPaused)
            {
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
                acceptInput = true;
            }
                
            if (event.type == sf::Event::Closed)
                window.close();

        }
        std::stringstream ss;
        ss << "score = " << score;
        scoreTrack.setCharacterSize(80);
        scoreTrack.setString(ss.str());
        sf::Time dt = clock.getElapsedTime();
        clock.restart();

        // input on one can start chopping
        if (acceptInput)
        {
            // if presses left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                score++;
                timeBar.setSize({timeBar.getSize().x + 4.50f, timeBar.getSize().y});
                if (timeBar.getSize().x >= timeBarX)
                    timeBar.setSize({timeBarX, timeBarY});
                acceptInput = false;
                playerSide = side::LEFT;
                spritePlayer.setPosition(player_LeftX, player_Y);
                spriteAxe.setPosition(AXE_POS_LEFT, spriteAxe.getPosition().y);
                shiftBranch(score);
                isLogActive = true;
                spriteLog.setPosition(LOG_X, LOG_Y);
                chop.play();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                score++;
                timeBar.setSize({timeBar.getSize().x + 4.50f, timeBar.getSize().y});
                if (timeBar.getSize().x >= timeBarX)
                    timeBar.setSize({timeBarX, timeBarY});
                acceptInput = false;
                playerSide = side::RIGHT;
                spriteAxe.setPosition(AXE_POS_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, spritePlayer.getPosition().y);
                shiftBranch(score);
                isLogActive = true;
                spriteLog.setPosition(LOG_X, LOG_Y);
                chop.play();
            }
        }

        if (isPaused)
        {
            if (timeBar.getSize().x < 0.0f)
            {
                text.setString("Time Out!!!");
                text.setCharacterSize(text.getCharacterSize());
                text.setFillColor(sf::Color::Red);
                centerTextOrigin(text);
                text.setPosition(text.getPosition().x, text.getPosition().y);
                isBeeActive = false;
                float timeRemaining = 100.0f;
                spriteBee.setPosition(2000, 750);
                for (int i = 0; i < isCloudActive.size(); i++)
                {
                    isCloudActive[i] = false;
                    clouds[i].setPosition(-200, 5); //too high to explain ts
                }
                acceptInput = false;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    timeBar.setSize(sf::Vector2f(timeBarX, timeBarY)); // set only back when player presses enter
                score = 0; //reset score back to 0
            } else if (isSquished)
            {
                text.setString("squished");
                text.setCharacterSize(text.getCharacterSize());
                text.setFillColor(sf::Color::White);
                centerTextOrigin(text);
                text.setPosition(text.getPosition().x, text.getPosition().y);
                isBeeActive = false;
                float timeRemaining = 100.0f;
                spriteBee.setPosition(2000, 750);
                for (int i = 0; i < isCloudActive.size(); i++)
                {
                    isCloudActive[i] = false;
                    clouds[i].setPosition(-200, 5); //too caffeined to explain this
                }
                acceptInput = false;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
                    timeBar.setSize(sf::Vector2f(timeBarX, timeBarY)); // set only back when player presses enter
                score = 0; //reset score back to 0
            } else
            {
                text.setCharacterSize(60);
                text.setString("PRESS RETURN TO START!!!");
                centerTextOrigin(text);
                text.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                isPaused = false; // unpause
                text.setString("");
                acceptInput = true;

                for (auto &x : branchPosition)
                    x = side::NONE;

                spritePlayer.setPosition(player_LeftX, player_Y);
            }
        } else
        {
            // shift branches in real time
            for (int i = 0; i < SIZE; i++)
            {
                float height = i * 150;

                if (branchPosition[i] == side::LEFT)
                {
                    spriteBranches[i].setPosition(BRANCH_LEFT, height);
                    spriteBranches[i].setRotation(180);
                } else if (branchPosition[i] == side::RIGHT)
                {
                    spriteBranches[i].setPosition(BRANCH_RIGHT, height);
                    spriteBranches[i].setRotation(0);
                } else
                {        
                    spriteBranches[i].setPosition(3000, height);
                }
            }

            if (branchPosition[5] == playerSide )
            {
                isPaused = true;
                acceptInput = false;
                isSquished = true;
                death.play();
            }


            if (isLogActive)
            {
                if (playerSide == side::LEFT)
                {
                    int speed = 3700;
                    spriteLog.setPosition(spriteLog.getPosition().x + (speed * dt.asSeconds()), spriteLog.getPosition().y);
                    if (spriteLog.getPosition().x >= 3000)
                        isLogActive = false;
                } else
                {
                    int speed = 3700;
                    spriteLog.setPosition(spriteLog.getPosition().x - (speed * dt.asSeconds()), spriteLog.getPosition().y);
                    if (spriteLog.getPosition().x <= -3000)
                        isLogActive = false;
                }

            }

            timeRemaining -= dt.asSeconds();
            timeBar.setSize(sf::Vector2f(timeBar.getSize().x - (timeRemaining * dt.asSeconds()), timeBar.getSize().y));
            if (timeBar.getSize().x < 0.0f)
            {
                death.play();
                isPaused = true;
            }

            if (!isBeeActive)
            {
                beeSpeed = (rand() % 150) + 200;
                bee_Y = (rand() % 400) + (1080 / 2.0f);
                spriteBee.setPosition(spriteBee.getPosition().x, bee_Y);
                isBeeActive = true;
            } else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

                if (spriteBee.getPosition().x < 0)
                {
                    isBeeActive = false; // inactive the bee if out of screen
                    spriteBee.setPosition(2000, 750);
                }
            }

            for (int i = 0; i < 3; i++)
            {
                if (!isCloudActive[i])
                {
                    cloudSpeed[i] = (rand() % 100) + 200;
                    int Y = (rand() % 50) + 15;
                    clouds[i].setPosition(clouds[i].getPosition().x, Y);
                    isCloudActive[i] = 1;
                } else 
                {
                    clouds[i].setPosition(clouds[i].getPosition().x + (cloudSpeed[i] * dt.asSeconds()), clouds[i].getPosition().y);

                    if (clouds[i].getPosition().x > 2000)
                    {
                        isCloudActive[i] = 0;
                        clouds[i].setPosition(-200, 0);
                    }
                }
            }
            
        }
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(timeBar);
        window.draw(text);
        window.draw(scoreTrack);
        for (auto &i : clouds)
            window.draw(i);
        for (auto &j : spriteBranches)
            window.draw(j);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteBee);
        window.display();


    }

    return 0;
}

void shiftBranch(int seed) 
{
    for (int i = SIZE - 1; i > 0; i--)
    {
        branchPosition[i] = branchPosition[i-1];
    }

    srand((int)time(0)+seed);
    int ran = (rand() % 4);

    switch(ran)
    {
        case 0:
            branchPosition[0] = side::LEFT;
            break;
        case 1:
            branchPosition[0] = side::RIGHT;
            break;
        default:
            branchPosition[0] = side::NONE;
            break;
    }

    
}

void seed()
{
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    srand(seed);
}
void centerTextOrigin(sf::Text& text)
{
    sf::FloatRect bounds = text.getLocalBounds();


    text.setOrigin(bounds.left + bounds.width / 2.0f, 
                bounds.top + bounds.height / 2.0f);

}
void centerOrigin(sf::Sprite& sprite)
{
    sprite.setOrigin(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
}

void loadTexture(sf::Texture& texture, const std::string& file)
{
    if (!texture.loadFromFile(file))
    {
        exit(404);
        std::cerr << "failed to load " << file << "\n";
    }
}
