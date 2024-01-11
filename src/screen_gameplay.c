/**********************************************************************************************
 *
 *   raylib - Advance Game template
 *
 *   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
 *
 *   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
 *
 *   This software is provided "as-is", without any express or implied warranty. In no event
 *   will the authors be held liable for any damages arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose, including commercial
 *   applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not claim that you
 *     wrote the original software. If you use this software in a product, an acknowledgment
 *     in the product documentation would be appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *     as being the original software.
 *
 *     3. This notice may not be removed or altered from any source distribution.
 *
 **********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Magic numbers
//----------------------------------------------------------------------------------
#define MIN_GAMESPEED 1
#define MAX_GAMESPEED 20
const int TARGET_FPS = 60;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
// Game controll
static int framesCounter = 0;
static int finishScreen = 0;
static int isPlaying = 1;
static int gameSpeed = 10;

// Grid and cells
const int rows = 50;
const int cols = 80;
static int gap = 1;
static int borderThickness = 2;
static int paddingTop = 100;
static int paddingBottom = 50;
static int paddingLeft = 50;
static int paddingRight = 50;

typedef enum CellStatus { DEAD = 0, ALIVE } CellStatus;
struct Cell
{
    CellStatus status;
    struct Rectangle outerRec;
    struct Rectangle innerRec;
    int border;
};

static struct Cell *GridOfLife[50][80];
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    int row, col;
    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            struct Cell *cell = malloc(sizeof(struct Cell));
            if (cell == NULL)
            {
                TraceLog(LOG_FATAL, "Unable to allocate memory for Cell of Life");
            }
            cell->status = DEAD;
            cell->border = 0;
            GridOfLife[row][col] = cell;
        }
    }
}

// Increase game speed and keep it in limit
void IncreaseGameSpeed()
{
    if (gameSpeed + 1 >= MAX_GAMESPEED)
    {
        gameSpeed = MAX_GAMESPEED;
    }
    else
    {
        gameSpeed += 1;
    }
}

// Descrease game speed
void DescreaseGameSpeed()
{
    if (gameSpeed - 1 <= MIN_GAMESPEED)
    {
        gameSpeed = MIN_GAMESPEED;
    }
    else
    {
        gameSpeed -= 1;
    }
}

// Decide whether to run next life cycle
bool ShouldStartNextCycle()
{
    float rate = (float) gameSpeed / 10;
    int frameLimit = TARGET_FPS * 1 / rate;
    TraceLog(LOG_DEBUG, "Frame limit: %d", frameLimit);
    if (framesCounter >= frameLimit)
    {
        return true;
    }
    return false;
}

// Count alive cells surrounding given cell position
int AdjacentAliveCells(int cellRow, int cellCol)
{
    int aliveCells = 0;
    struct PositionOffset
    {
        int x;
        int y;
    };
    struct PositionOffset OffsetArray[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    int i;
    for (i = 0; i < 8; i++)
    {
        int offsetRow = cellRow + OffsetArray[i].x;
        int offsetCol = cellCol + OffsetArray[i].y;
        if (offsetRow >= 0 && offsetRow < rows && offsetCol >= 0 && offsetCol < cols)
        {
            // TraceLog(LOG_DEBUG, "Offset x: %d, offset y: %d", offsetRow, offsetCol);
            struct Cell *cell = GridOfLife[offsetRow][offsetCol];
            if (cell->status == ALIVE)
            {
                aliveCells++;
            }
        }
    }
    return aliveCells;
}

// The check every cell in the GridOfLife and apply Rules of life to it
void CyleOfLife()
{
    int row, col;
    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            struct Cell *cell = GridOfLife[row][col];
            int surroundingLife = AdjacentAliveCells(row, col);
            switch (cell->status)
            {
                case DEAD:
                    if (surroundingLife == 3)
                    {
                        cell->status = ALIVE;
                    }
                    break;
                case ALIVE:
                    if (surroundingLife <= 1)
                    {
                        cell->status = DEAD;
                    }
                    else if (surroundingLife > 4)
                    {
                        cell->status = DEAD;
                    }
                    else if (surroundingLife >= 2 && surroundingLife <= 3)
                    {
                        // it continue to live
                    }
                    break;
                default:
                    break;
            }
        }
    }
    framesCounter = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    framesCounter++;

    if (IsKeyPressed(KEY_P))
    {
        if (isPlaying == 1)
        {
            isPlaying = 0;
        }
        else
        {
            isPlaying = 1;
        }
    }

    if (IsKeyPressedRepeat(KEY_UP) || IsKeyPressed(KEY_UP))
    {
        IncreaseGameSpeed();
    }
    else if (IsKeyPressedRepeat(KEY_DOWN) || IsKeyPressed(KEY_DOWN))
    {
        DescreaseGameSpeed();
    }

    if (ShouldStartNextCycle())
    {
        CyleOfLife();
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    DrawRectangle(0, 0, w, h, BLACK);
    Vector2 pos = {20, 10};
    char gameSpeedText[80] = "";
    sprintf(gameSpeedText, "Gamespeed: %d", gameSpeed);
    DrawTextEx(font, gameSpeedText, pos, font.baseSize * 2.0f, 4, MAROON);

    char isPlayingStr[] = "ISPLAYING: 1";
    sprintf(isPlayingStr, "ISPLAYING: %d", isPlaying);
    DrawText(isPlayingStr, w - 400, 30, 20, MAROON);
    DrawGameGrid();
}

void OnCellClick(struct Cell *cell)
{
    int newValue = cell->status + 1;
    if (newValue > 1)
    {
        newValue = 0;
    }
    cell->status = newValue;
    PlaySound(fxCoin);
}

void DrawGameGrid(void)
{

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    struct Vector2 mousePos = GetMousePosition();
    bool mouseClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // Display box
    int displayWidth = w - paddingLeft - paddingRight;
    int displayHeight = h - paddingTop - paddingBottom;
    int displayPosX = paddingLeft;
    int displayPosY = paddingTop;
    DrawRectangle(displayPosX, displayPosY, displayWidth, displayHeight, GRAY);

    int gapSpaceX = (cols - 1) * gap;
    int gapSpaceY = (rows - 1) * gap;

    // Cell size
    // Calculate cell size based the smaller side of display box
    unsigned int cellWidth, cellHeight;
    if (displayWidth > displayHeight)
    {
        cellHeight = (displayHeight - gapSpaceY) / rows;
        cellWidth = cellHeight;
    }
    else
    {
        cellWidth = (displayWidth - gapSpaceX) / cols;
        cellHeight = cellWidth;
    }

    // GRID
    int centerX = displayPosX + displayWidth / 2;
    int centerY = displayPosY + displayHeight / 2;
    DrawCircle(centerX, centerY, 10.0, RED);

    int gridWidth = cols * cellWidth + (cols - 1) * gap;
    int gridHeight = rows * cellHeight + (rows - 1) * gap;

    int gridPosX = centerX - gridWidth / 2;
    int gridPosY = centerY - gridHeight / 2;
    DrawCircle(gridPosX, gridPosY, 10.0, BLUE);

    // Colors
    Color borderColor = MAROON;

    TraceLog(LOG_DEBUG, "GRID: Grid initialized");
    TraceLog(LOG_DEBUG, "\t> Display box center: %d, %d", centerX, centerY);
    TraceLog(LOG_DEBUG, "\t> Display box size: %dx%d", displayWidth, displayHeight);
    TraceLog(LOG_DEBUG, "\t> Grid size: %dx%d", rows, cols);
    TraceLog(LOG_DEBUG, "\t> Grid position: %d, %d", gridPosX, gridPosY);
    TraceLog(LOG_DEBUG, "\t> Padding: %d %d %d %d", paddingTop, paddingRight, paddingBottom, paddingLeft);
    TraceLog(LOG_DEBUG, "\t> Border: %d", borderThickness);
    TraceLog(LOG_DEBUG, "\t> Cell size: %dx%d", cellWidth, cellHeight);

    // Draw grid and cells

    int row, col;
    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            Color cellFill;
            struct Cell *cell = GridOfLife[row][col];
            if (CheckCollisionPointRec(mousePos, cell->outerRec))
            {
                cell->border = borderThickness;
                if (mouseClicked)
                {
                    OnCellClick(cell);
                }
            }
            else
            {
                cell->border = 0;
            }
            if (cell->status == DEAD)
            {
                cellFill = BLACK;
            }
            else
            {
                cellFill = WHITE;
            }
            // Draw outer rectangle
            int posX = gridPosX + col * (cellWidth + gap);
            int posY = gridPosY + row * cellHeight + row * gap;
            struct Rectangle outerRec = {(float) posX, (float) posY, (float) cellWidth, (float) cellHeight};
            cell->outerRec = outerRec;
            DrawRectangleRec(cell->outerRec, borderColor);

            // Draw inner rectangle
            unsigned int cellInnerWidth = cellWidth - cell->border * 2;
            unsigned int cellInnerHeight = cellHeight - cell->border * 2;
            int innerPosX = posX + cell->border;
            int innerPosY = posY + cell->border;
            struct Rectangle innerRec = {(float) innerPosX, (float) innerPosY, cellInnerWidth, cellInnerHeight};
            cell->innerRec = innerRec;
            DrawRectangleRec(cell->innerRec, cellFill);
        }
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!

    // Pause the game when screen is unloaded
    isPlaying = 0;

    TraceLog(LOG_DEBUG, "Freeing Cells of Life memory");
    int row, col;
    for (row = 0; row < rows; row++)
    {
        for (col = 0; col < cols; col++)
        {
            free(GridOfLife[row][col]);
        }
    }
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
