#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
// Function declaration

void updateBranches(int seed);
const int NUM_BRANCHES = 5;

sf::Sprite branches[NUM_BRANCHES];

// Where is the player/branch?
// Left or Right
enum class side { LEFT, RIGHT, NONE };

side branchPositions[NUM_BRANCHES];
void updateBranches(int seed){
    // Move all the branches down one place
    for (int j = NUM_BRANCHES-1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }
    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0)+seed);
    int r = (rand() % 5);

    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}
int main()
{

    sf::VideoMode vm(1600, 900);
    sf::RenderWindow window(vm, "Timber !!!", sf::Style::Fullscreen);
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0,0);

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(710,-100);

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 700);
    // Is the bee currently moving?
    bool beeActive = false;
    // How fast can the bee fly
    float beeSpeed = 0.0f;

    // make 3 cloud sprites from 1 texture
    sf::Texture textureCloud;
    // Load 1 new texture
    textureCloud.loadFromFile("graphics/cloud.png");
    // 3 New sprites with the same texture
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    // Position the clouds on the left of the screen
    // at different heights
    spriteCloud1.setPosition(rand()%1600, 0);
    spriteCloud2.setPosition(rand()%1600, 250);
    spriteCloud3.setPosition(rand()%1600, 500);
    // Are the clouds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
        
    // How fast is each cloud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    sf::Clock clock; 

    bool paused = true;
    int score = 0;

    sf::Text messageText;
    sf::Text scoreText;

    sf::Font font;
    font.loadFromFile("fonts/KOMIKATR.ttf");
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press ENTER to start");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width/2.0f,
                            textRect.top + textRect.height/2.0f);
    messageText.setPosition(1600/2.0f, 900/2.0f);
    scoreText.setPosition(20, 20);

    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1600/2) - timeBarStartWidth / 2, 800);

    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220,20);
    }
    for (int i = 1; i < NUM_BRANCHES; i++) {
        updateBranches(i);
    }

    //Prepare ther player 
    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(550, 600);


    side playerSide = side::LEFT;
    //Prepare the gravestone
    sf::Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    sf::Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(550, 700);
    
    //Prepare the axe
    sf::Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(600, 675);

    const float AXE_POSITION_LEFT = 670;
    const float AXE_POSITION_RIGHT = 970;
    // Prepare the flying log
    sf::Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(710, 650);
    // Other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    
    bool acceptInput = false;
    //Prapare the sounds 
    sf::SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    sf::Sound chop;
    chop.setBuffer(chopBuffer);

    sf::SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    sf::Sound death;
    death.setBuffer(deathBuffer);

    sf::SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    sf::Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);
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
                if (event.type == sf::Event::KeyReleased && !paused)
                {
                // Listen for key presses again
                acceptInput = true;
                // hide the axe
                spriteAxe.setPosition(2000,
                spriteAxe.getPosition().y);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                paused = false;
                score = 0;
                timeRemaining = 6;
                for (int i = 1; i < NUM_BRANCHES; i++) {
                    branchPositions[i] = side::NONE;
                }
                spriteRIP.setPosition(675, 2000);
                spritePlayer.setPosition(580, 600);
                acceptInput = true;
            }

            if(acceptInput) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        playerSide = side::RIGHT;
                        score++;
                        timeRemaining += (2/score) * .15; 
                        spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                        spritePlayer.setPosition(1000, 600);
                        updateBranches(score);
                        spritePlayer.setPosition(1000, 600);
                        // Update the branches
                        updateBranches(score);
                        // Set the log flying to the left
                        spriteLog.setPosition(710, 650);
                        logSpeedX = -5000;
                        logActive = true;
                        acceptInput = false;
                        // Play a chop sound
                        chop.play();
                    }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        // Make sure the player is on the left
                        playerSide = side::LEFT;
                        score++;
                        // Add to the amount of time remaining
                        timeRemaining += (2 / score) + .15;
                        spriteAxe.setPosition(AXE_POSITION_LEFT,
                        spriteAxe.getPosition().y);
                        spritePlayer.setPosition(580, 600);
                        // update the branches
                        updateBranches(score);
                        // set the log flying
                        spriteLog.setPosition(710, 650);
                        logSpeedX = 5000;
                        logActive = true;
                        acceptInput = false;
                        // Play a chop sound
                        chop.play();

                    }
            }
            /*
            ****************************************
            Update the scene
            ****************************************
            */
            if (!paused) {
                sf::Time dt = clock.restart();
                //Setup th bee

                timeRemaining -= dt.asSeconds();
                timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond*timeRemaining,
                                         timeBarHeight));
                if (timeRemaining < 0.0f) {
                    paused = true;
                    messageText.setString("Out of time!!");
                    sf::FloatRect textRect = messageText.getLocalBounds();
                    messageText.setOrigin(textRect.left + textRect.width/2.0f,
                            textRect.top + textRect.height/2.0f);
                    messageText.setPosition(1600/2.0f, 900/2.0f);
                    // Play the out of time sound
                    outOfTime.play();
                }

                if (!beeActive) {
                    srand((int)time(0));
                    beeSpeed = (rand() % 150) + 150;
                    //Height of the bee
                    srand((int)time(0) * 10);
                    float height = (rand() % 400) + 400;
                    spriteBee.setPosition(1800, height);
                    beeActive = true;
                } else {
                    spriteBee.setPosition(
                            spriteBee.getPosition().x - 
                            (beeSpeed * dt.asSeconds()),
                            spriteBee.getPosition().y);
                    if (spriteBee.getPosition().x < -100) 
                    {
                        beeActive = false;
                    }
                }
                // Manage the clouds
    // Cloud 1
                if (!cloud1Active)
                {
                // How fast is the cloud
                    srand((int)time(0) * 10);
                    cloud1Speed = (rand() % 150);
                    // How high is the cloud
                    srand((int)time(0) * 10);
                    float height = (rand() % 100);
                    spriteCloud1.setPosition(-200, height);
                    cloud1Active = true;
                } else {
                    spriteCloud1.setPosition(
                        spriteCloud1.getPosition().x +
                        (cloud1Speed * dt.asSeconds()),
                        spriteCloud1.getPosition().y);
                    // Has the cloud reached the right hand edge of the screen?
                    if (spriteCloud1.getPosition().x > 1600)
                    {
                    // Set it up ready to be a whole new cloud next frame
                        cloud1Active = false;
                    }
                }
    // Cloud 2
                if (!cloud2Active)
                {
            // How fast is the cloud
                    srand((int)time(0) * 20);
                    cloud2Speed = (rand() % 150);
                    // How high is the cloud
                    srand((int)time(0) * 20);
                    float height = (rand() % 250) - 150;
                    spriteCloud2.setPosition(-200, height);
                    cloud2Active = true;
                }
                else
                {
                    spriteCloud2.setPosition(
                        spriteCloud2.getPosition().x +
                        (cloud2Speed * dt.asSeconds()),
                        spriteCloud2.getPosition().y);
                    // Has the cloud reached the right hand edge of the screen?
                    if (spriteCloud2.getPosition().x > 1600)
                    {
                    // Set it up ready to be a whole new cloud next frame
                        cloud2Active = false;
                    }
                }
                if (!cloud3Active)
                {
                    // How fast is the cloud
                    srand((int)time(0) * 30);
                    cloud3Speed = (rand() % 150);
                    // How high is the cloud
                    srand((int)time(0) * 30);
                    float height = (rand() % 350) - 120;
                    spriteCloud3.setPosition(-200, height);
                    cloud3Active = true;
                    }
                    else
                    {
                    spriteCloud3.setPosition(
                        spriteCloud3.getPosition().x +
                        (cloud3Speed * dt.asSeconds()),
                        spriteCloud3.getPosition().y);
                    // Has the cloud reached the right hand edge of the screen?
                    if (spriteCloud3.getPosition().x > 1600)
                    {
                    // Set it up ready to be a whole new cloud next frame
                        cloud3Active = false;
                    }
                }
                       if (rand()%100 == 1) {
                            std::stringstream ss;
                            ss << "Score = " << score;
                            scoreText.setString(ss.str());
                        } 
                // update the branch sprites
                for (int i = 0; i < NUM_BRANCHES; i++) {
                    float height = i*150 + 20;
                    if (branchPositions[i] == side::LEFT) {
                        branches[i].setPosition(530, height);
                        branches[i].setRotation(180);
                    } else if (branchPositions[i] == side::RIGHT) {
                        branches[i].setPosition(1130, height);
                        branches[i].setRotation(0);
                    } else {
                        branches[i].setPosition(3000, height);
                    }
                }
                // Handle a flying log
                    if (logActive)
                {
                    spriteLog.setPosition(
                    spriteLog.getPosition().x +
                    (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y +
                    (logSpeedY * dt.asSeconds()));
                    // Has the log reached the right hand edge?
                    if (spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000)
                    {
                    // Set it up ready to be a whole new log next frame
                    logActive = false;
                    spriteLog.setPosition(710, 650);
                    }
                }
            // has the player been squished by a branch?
                if (branchPositions[4] == playerSide)
                {
                // death
                    paused = true;
                    acceptInput = false;
                    // Draw the gravestone
                    spriteRIP.setPosition(525, 650);
                    // hide the player
                    spritePlayer.setPosition(2000, 660);
                    // Change the text of the message
                    messageText.setString("SQUISHED!!");
                    // Center it on the screen
                    sf::FloatRect textRect = messageText.getLocalBounds();
                    messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                    messageText.setPosition(1600 / 2.0f, 900 / 2.0f);
                    // Play the death sound
                    death.play();
                    }
            }
            /*
            ****************************************
            Draw the scene
            ****************************************
            */
            // Clear everything from the last frame
            window.clear();
            // Draw our game scene here
            window.draw(spriteBackground);
            // Draw the clouds
            window.draw(spriteCloud1);
            window.draw(spriteCloud2);
            window.draw(spriteCloud3);
            // Draw the tree
            window.draw(spriteTree);
            // Draw the insect
            
            // Draw the bee
            window.draw(spriteBee);
            window.draw(scoreText);
            window.draw(timeBar);
            // Draw the player 
            window.draw(spritePlayer);
                       // Draw the flying log
            window.draw(spriteLog);
            // Draw the gravestone
            window.draw(spriteRIP);

            for (int i = 0; i < NUM_BRANCHES; ++i) {
                window.draw(branches[i]);
            }
            if (paused) {
                window.draw(messageText);
            }
             // Draw the Axe
            window.draw(spriteAxe);

            // Show everything we just drew
            window.display();
        }
    return 0;
}
