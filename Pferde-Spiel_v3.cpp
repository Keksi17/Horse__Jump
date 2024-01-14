#include "raylib.h"
#include "raylib_helpers.h"
#include <iostream >
using namespace std;

const int screenWidth = 800;
const int screenHeigth = 600;

struct Horse
{
    Vector2 center;
    Vector2 size;
    int deltaX;
    double deltaY;
    double gravity;
    bool isJumping;
    int jumpCount;
    Color color;
};

struct pictureHorse
{
    Vector2 center;       // Position auf dem Bildschirm
    Texture2D images[17]; // alle Bilder der Animation
    Texture2D imagesJ[9];
    int imagesSize; // Laenge des Arrays
    int imagesSizeJ;
    float animationIndex; // index des aktuell angezeigten Bilds
    float animationIndexJ;
    float animationSpeed; // Wert, um den der index im Game-Loop erhoeht wird
    float animationSpeedJ;
    int radius;
    int deltaX;
    double deltaY;
    double gravity;
    bool isJumping;
    int jumpCount;
    Color color;
};

struct pictureJumps
{
    Vector2 center;
    Texture2D images[1];
    int imagesSize;
    float animationIndex;
    float animationSpeed;

};

struct Hinderniss
{
    Vector2 center;
    Vector2 size;
    Color color;
};

struct GameState
{
    double survivedTime;
    bool gameOver;
    double record;
};


pictureJumps creatJumps(float x, float y, float animationSpeed)
{
    return
    {
        {x,y},
        LoadTexture("img/jumps/hedge.png"),
        1,
        0,
        animationSpeed,

    };
}


pictureHorse createHorse(float x, float y, float animationSpeed, int r, int dx, double dy, double g, bool ij, int jx, Color c)
{
    return {{x, y},
            {LoadTexture("img/horse/horse1.png"),
             LoadTexture("img/horse/horse2.png"),
             LoadTexture("img/horse/horse3.png"),
             LoadTexture("img/horse/horse4.png"),
             LoadTexture("img/horse/horse5.png"),
             LoadTexture("img/horse/horse6.png"),
             LoadTexture("img/horse/horse7.png"),
             LoadTexture("img/horse/horse8.png"),
             LoadTexture("img/horse/horse9.png"),
             LoadTexture("img/horse/horse10.png"),
             LoadTexture("img/horse/horse11.png"),
             LoadTexture("img/horse/horse12.png"),
             LoadTexture("img/horse/horse13.png"),
             LoadTexture("img/horse/horse14.png"),
             LoadTexture("img/horse/horse15.png"),
             LoadTexture("img/horse/horse16.png"),
             LoadTexture("img/horse/horse17.png")},

            {
                LoadTexture("img/horse/horse_jump1.png"),
                LoadTexture("img/horse/horse_jump2.png"),
                LoadTexture("img/horse/horse_jump3.png"),
                LoadTexture("img/horse/horse_jump4.png"),
                LoadTexture("img/horse/horse_jump5.png"),
                LoadTexture("img/horse/horse_jump6.png"),
                LoadTexture("img/horse/horse_jump7.png"),
                LoadTexture("img/horse/horse_jump8.png"),
                LoadTexture("img/horse/horse_jump9.png"),
            },
            17,
            9,
            0,
            0,
            animationSpeed,
            0.1,
            r,
            dx,
            dy,
            g,
            ij,
            jx,
            c};
}

void moveHorse(Horse *horse)
{
    if (IsKeyPressed(KEY_SPACE) && horse->jumpCount < 2)
    {
        horse->isJumping = true;
        horse->deltaY = 10;
        horse->jumpCount += 1;
    }

    if (horse->isJumping)
    {
        horse->center.y -= horse->deltaY;
        horse->deltaY -= horse->gravity;

        if (horse->center.y > 370)
        {
            horse->isJumping = false;
            horse->center.y = 370;
            horse->jumpCount = 0;
        }
    }
}

void movepHorse(pictureHorse *h)
{
    if (IsKeyPressed(KEY_SPACE) && h->jumpCount < 2)
    {
        h->isJumping = true;
        h->deltaY = 10;
        h->jumpCount += 1;
    }

    if (h->isJumping)
    {
        h->center.y -= h->deltaY;
        h->deltaY -= h->gravity;

        if (h->center.y > 360)
        {
            h->isJumping = false;
            h->center.y = 360;
            h->jumpCount = 0;
        }
    }
}

void upadateGamestate(Horse *horse, Hinderniss alleH[], GameState *gamestate, pictureHorse *h)
{
    if (!gamestate->gameOver)
    {

        if (!horse->isJumping)
        {
            DrawTextureV(h->images[int(h->animationIndex)], h->center, WHITE);
        }
        else if (horse->isJumping)
        {
            DrawTextureV(h->imagesJ[int(h->animationIndexJ)], h->center, WHITE);
        }

        gamestate->survivedTime += GetFrameTime();
        DrawDouble(gamestate->survivedTime, 370, 30, 30, WHITE);
        for (int i = 0; i < 4; i++)
        {
            if (CheckCollisionRecs({horse->center.x, horse->center.y, horse->size.x, horse->size.y}, {alleH[i].center.x, alleH[i].center.y, alleH[i].size.x, alleH[i].center.y}))
            {
                gamestate->gameOver = true;
                if (gamestate->record < gamestate->survivedTime)
                {
                    gamestate->record = gamestate->survivedTime;
                    cout << "Dein Rekord beträgt " << gamestate->record << " Sekunden" << endl;
                }
                gamestate->survivedTime = 0;
            }
        }
    }

    if (gamestate->gameOver)
    {
        DrawRectangle(0, 250, screenWidth, 50, WHITE);
        DrawText("Du hast Verloren, drücke Leertaste um erneut zu spielen", 10, 270, 25, BLACK);
    
        
    
        DrawTextureV(LoadTexture("img/jumps/hedge.png"), {alleH[0].center.x, alleH[0].center.y}, WHITE);
        DrawTextureV(LoadTexture("img/jumps/Steil.png"), {alleH[1].center.x - 5, alleH[1].center.y}, WHITE);
        DrawTextureV(LoadTexture("img/jumps/oxer.png"), {alleH[2].center.x - 5, alleH[2].center.y}, WHITE);
        DrawTextureV(LoadTexture("img/jumps/triplebarre.png"), {alleH[3].center.x - 5, alleH[3].center.y}, WHITE);
        
    

    }

    if (gamestate->gameOver && IsKeyPressed(KEY_SPACE))
    {
        gamestate->gameOver = false;
        int center[] = {800, 1100, 1320, 1600};
        for (int i = 0; i < 4; i++)
        {
            alleH[i].center.x = center[i];
        }
    }
}

int maxX(Hinderniss alleH[])
{
    int max = alleH[0].center.x;

    for (int i = 0; i < 4; i++)
    {
        if (alleH[i].center.x > max)
        {
            max = alleH[i].center.x;
        }
    }

    return max;
}

void movejumps(Hinderniss alleH[], pictureJumps *j)
{
    for (int i = 0; i < 4; i++)
    {
        
        DrawTextureV(j->images[0], j->center, WHITE);
        if(alleH[i].size.y == 80)
        {
            DrawTextureV(LoadTexture("img/jumps/hedge.png"), {alleH[i].center.x, alleH[i].center.y}, WHITE);
        }else if(alleH[i].size.y == 70)
        {
            DrawTextureV(LoadTexture("img/jumps/Steil.png"), {alleH[i].center.x - 5, alleH[i].center.y}, WHITE);
        }else if(alleH[i].size.y == 85)
        {
            DrawTextureV(LoadTexture("img/jumps/oxer.png"), {alleH[i].center.x - 5, alleH[i].center.y}, WHITE);
        }else if(alleH[i].size.y == 60)
        {
            DrawTextureV(LoadTexture("img/jumps/triplebarre.png"), {alleH[i].center.x - 5, alleH[i].center.y}, WHITE);
        }
        
        
    
        alleH[i].center.x -= 3;

        Color colors[] = {DARKGREEN, YELLOW, BLACK, RED};
        Vector2 sizes[] = {{40, 80}, {10, 70}, {40, 85}, {50, 60}};

        if (alleH[i].center.x < -50)
        {

            alleH[i].center.x = maxX(alleH) + GetRandomValue(200, 500);
            int rand = GetRandomValue(0, 3);
            alleH[i].color = colors[rand];
            alleH[i].size = sizes[rand];
            alleH[i].center.y = 420 - sizes[rand].y;
        }
    }
}

void updateGame(Horse *horse, Hinderniss alleH[], GameState *gamestate, pictureHorse *h, pictureJumps *j)
{

    if (!gamestate->gameOver)
    {
        moveHorse(horse);
        movepHorse(h);
        movejumps(alleH, j);

        if (!horse->isJumping)
        {
            h->animationIndex += h->animationSpeed;
        }
        else
        {
            h->animationIndexJ += h->animationSpeedJ;
        }

        if (h->animationIndex >= h->imagesSize)
        {
            h->animationIndex = 0;
        }
        if (h->animationIndexJ >= h->imagesSizeJ || !horse->isJumping)
        {
            h->animationIndexJ = 0;
        }
    }
    upadateGamestate(horse, alleH, gamestate, h);
}

int main()
{
    InitWindow(screenWidth, screenHeigth, "Horse Jump");
    SetTargetFPS(120);
    Horse horse1 = {{150, 367}, {70, 50}, 1, 0, 0.3, false, 0, BROWN};
    Hinderniss Hecke = {{800, 340}, {40, 80}, DARKGREEN};
    Hinderniss Steil = {{1200, 350}, {10, 70}, YELLOW};
    Hinderniss Oxer = {{1420, 335}, {40, 85}, BLACK};
    Hinderniss Triplebarre = {{1600, 360}, {50, 60}, RED};

    GameState gamestate = {0, false};
    gamestate.record = 0;

    Hinderniss alleH[] = {Hecke, Steil, Oxer, Triplebarre};

    pictureHorse h = createHorse(130, 360, 0.2, 20, 1, 0, 0.3, false, 0, BROWN);
    pictureJumps j = creatJumps(800, 340, 1);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        DrawRectangle(0, 400, screenWidth, 200, LIME);

        updateGame(&horse1, alleH, &gamestate, &h, &j);

        EndDrawing();
    }
}
