
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>

using namespace std;
using namespace sf;

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

const int emp = 0;
const int player = 1;
const int bullet = 2;
const int mushroom = 3;

int gameGrid[gameRows][gameColumns] = {};
int gameGrid1[gameRows][gameColumns][2] = {};

const int x = 0;
const int y = 1;
const int exists = 2;

void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite);
void moveBullet(float bullet[], Clock& bulletClock);
void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite);
void drawMushroom(RenderWindow& window, int row, int column, Sprite& mushroomSprite);
void drawdMushroom(RenderWindow& window, int row, int column, Sprite& damagedmushrom);
void placeMushrooms();
void checkBulletMushroomCollision(float bullet[], Music &);
void movePlayer(float player[]);
void shootBullet(float player[], float bullet[]);
bool isPlayerCollidingWithMushroom(float player[]);
void resetGame();
void initializeCentipede(float startX, float startY, int numSegments);

int main()
{
    srand((int)time(0));

    const int centseg = 12;
    float centipedeseg[12][2];

    RenderWindow window(VideoMode(resolutionX, resolutionY), "Mahad's Centipede", Style::Close | Style::Titlebar);

    window.setSize(Vector2u(900, 900));

    window.setPosition(Vector2i(290, 30));

    Music bgMusic;
    bgMusic.openFromFile("Music/bgmusic.mp3");
    bgMusic.play();
    bgMusic.setVolume(50);

    Texture backgroundTexture;
    Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(Color(255, 255, 255, 255 * 0.30));

    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (gameColumns * 3 / 4) * boxPixelsY;
    Texture playerTexture;
    Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false; // Initially, there's no bullet
    Clock bulletClock;
    Texture bulletTexture;
    Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    Texture mushroomTexture;
    Sprite mushroomSprite;
    mushroomTexture.loadFromFile("Textures/mushroom.png");
    mushroomSprite.setTexture(mushroomTexture);
    mushroomSprite.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    Texture damagedMushroomTexture;
    damagedMushroomTexture.loadFromFile("Textures/mushroom.png");
    Sprite damagedMushroom;
    damagedMushroom.setTexture(damagedMushroomTexture);
    damagedMushroom.setTextureRect(IntRect(boxPixelsX, 0, boxPixelsX, boxPixelsY));

    Texture centipedeBodyTexture;
    Sprite centipedeBody;
    centipedeBodyTexture.loadFromFile("Textures/cbody.png");
    centipedeBody.setTexture(centipedeBodyTexture);
    centipedeBody.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    Texture centipedeHeadTexture;
    centipedeHeadTexture.loadFromFile("Textures/chead.png");
    Sprite centipedeHead;
    centipedeHead.setTexture(centipedeHeadTexture);
    centipedeHead.setTextureRect(IntRect(0, 0, boxPixelsX, boxPixelsY));

    SoundBuffer bulletfire;
    bulletfire.loadFromFile("se/fire1.wav");

    Sound Shootbullet;
    Shootbullet.setBuffer(bulletfire);
   // Shootbullet.

    Music Mushhit;
    Mushhit.openFromFile("se/newBeat.wav");

           
    placeMushrooms();

    while (window.isOpen())
    {
       
        window.draw(backgroundSprite);

        drawPlayer(window, player, playerSprite);

        movePlayer(player);

        centipedeHead.setPosition(345, 0);
        window.draw(centipedeHead);
        int x1 = 0;
        for (int i = 0; i <= 10; i++)
        {
            window.draw(centipedeBody);
            centipedeBody.setPosition(x1, 0);
            x1 += 32;
        }


        if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet[exists])
        {
            shootBullet(player, bullet);
            Shootbullet.play();
        }
        
        if (bullet[exists])
        {
            moveBullet(bullet, bulletClock);
            drawBullet(window, bullet, bulletSprite);
            checkBulletMushroomCollision(bullet,Mushhit);
        }

        // Draw mushrooms
        for (int i = 0; i < gameRows; ++i)
        {
            for (int j = 0; j < gameColumns; ++j)
            {
                if (gameGrid1[i][j][0] == mushroom)
                {
                    drawMushroom(window, i, j, mushroomSprite);
                }
                 if (gameGrid1[i][j][0] == 1)
                    drawdMushroom(window, i, j, damagedMushroom);
            }
        }

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
        }
        window.display();
        window.clear();
    }

    return 0;
}

void drawPlayer(RenderWindow& window, float player[], Sprite& playerSprite)
{
    playerSprite.setPosition(player[x], player[y]);
    window.draw(playerSprite);
}

void moveBullet(float bullet[], Clock& bulletClock)
{
    if (bulletClock.getElapsedTime().asMilliseconds() < 8)
        return;

    bulletClock.restart();
    bullet[y] -= 10;
    if (bullet[y] < -32)
        bullet[exists] = false;
}

void drawBullet(RenderWindow& window, float bullet[], Sprite& bulletSprite)
{
    bulletSprite.setPosition(bullet[x], bullet[y]);
    window.draw(bulletSprite);
}

void drawMushroom(RenderWindow& window, int row, int column, Sprite& mushroomSprite)
{
    mushroomSprite.setPosition(column * boxPixelsX, row * boxPixelsY);
    window.draw(mushroomSprite);
}

void drawdMushroom(RenderWindow& window, int row, int column, Sprite& damagedmushroom)
{
    damagedmushroom.setPosition(column * boxPixelsX, row * boxPixelsY);
    window.draw(damagedmushroom);
}

void placeMushrooms()
{
    for (int i = 0; i < 25; ++i)
    {
        int row = rand() % 27 + 1;
        int column = rand() % 30;
        gameGrid1[row][column][0] = mushroom;
    }
}

void checkBulletMushroomCollision(float bullet[], Music & Mushhit)
{
    int row = bullet[y] / boxPixelsY;
    int column = bullet[x] / boxPixelsX;
    if (row >= 0 && row < gameRows && column >= 0 && column < gameColumns)
    {
        if (gameGrid1[row][column][0] == mushroom|| gameGrid1[row][column][0] == 1)
        {
            if (gameGrid1[row][column][0] == 1)
            {
                gameGrid1[row][column][0] = 0;
                bullet[exists] = false;
                Mushhit.play();
            }
            else
            {
                gameGrid1[row][column][0] = 1;
                bullet[exists] = false;
                Mushhit.play();
                
            }
        }
    }
}

void movePlayer(float player[])
{
    float movementSpeed = 0.15;
   
    if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && player[x] > 0)
    {
        player[x] -= movementSpeed;
    }

    if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && player[x] < resolutionX - boxPixelsX)
    {
        player[x] += movementSpeed;
    }

    if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && player[y] > resolutionY - 7 * boxPixelsY)
    {
        player[y] -= movementSpeed;
    }

    if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && player[y] < resolutionY - boxPixelsY)
    {
        player[y] += movementSpeed;
    }
}


void shootBullet(float player[], float bullet[])
{
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = true;
}

bool isPlayerCollidingWithMushroom(float player[])
{
    int row = player[y] / boxPixelsY;   
    int column = player[x] / boxPixelsX;

    return (row >= 0 && row < gameRows && column >= 0 && column < gameColumns &&
        gameGrid[row][column] == mushroom);
}

void resetGame()
{

    for (int i = 0; i < gameRows; ++i)
    {
        for (int j = 0; j < gameColumns; ++j)
        {
            gameGrid[i][j] = emp;
        }
    }
}


