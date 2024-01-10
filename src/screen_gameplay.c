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

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;
static int isPlaying = 1;

static int rows = 50;
static int cols = 80;
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        // finishScreen = 1;
        PlaySound(fxCoin);
    }
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
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    DrawRectangle(0, 0, w, h, BLACK);
    Vector2 pos = {20, 10};
    DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize * 3.0f, 4, MAROON);
    // DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

    char isPlayingStr[] = "ISPLAYING: 1";
    sprintf(isPlayingStr, "ISPLAYING: %d", isPlaying);
    DrawText(isPlayingStr, w - 400, 30, 20, MAROON);
    DrawGameGrid();
}

void DrawGameGrid(void)
{
    int gap = 1;
    int borderThickness = 1;
    int paddingTop = 100;
    int paddingBottom = 50;
    int paddingLeft = 50;
    int paddingRight = 50;

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    // Display box
    int displayWidth = w - paddingLeft - paddingRight;
    int displayHeight = h - paddingTop - paddingBottom;
    int displayPosX = paddingLeft;
    int displayPosY = paddingTop;
    DrawRectangle(displayPosX, displayPosY, displayWidth, displayHeight, GRAY);

    int gapSpace = (rows - 1) * gap;

    // Cell size
    // Calculate cell size based the smaller side of display box
    int smallSide;
    if (displayWidth > displayHeight)
    {
        smallSide = displayHeight;
    }
    else
    {
        smallSide = displayWidth;
    }

    unsigned int cellWidth = (smallSide - gapSpace) / rows;
    unsigned int cellHeight = cellWidth;
    unsigned int cellInnerWidth = cellWidth - borderThickness * 2;
    unsigned int cellInnerHeight = cellHeight - borderThickness * 2;

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
    Color cellFill = WHITE;

    TraceLog(LOG_DEBUG, "GRID: Grid initialized");
    TraceLog(LOG_DEBUG, "\t> Display box center: %d, %d", centerX, centerY);
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
            // Draw outer rectangle
            int posX = gridPosX + col * (cellWidth + gap);
            int posY = gridPosY + row * cellHeight + row * gap;
            DrawRectangle(posX, posY, cellWidth, cellHeight, borderColor);

            // Draw inner rectangle
            int innerPosX = posX + borderThickness;
            int innerPosY = posY + borderThickness;
            DrawRectangle(innerPosX, innerPosY, cellInnerWidth, cellInnerHeight, cellFill);
        }
    }
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!

    // Pause the game when screen is unloaded
    isPlaying = 0;
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
